#pragma once
#include <iostream>
#include <algorithm>
#include "Database.h"
#include "Relation.h"
#include "Tuple.h"
#include "Rule.h"
#include "Graph.h"

using namespace std;

class Interpreter {
private:
  DatalogProgram datalog;
  Database database;
  Graph graph;

public:
  Interpreter(const DatalogProgram& datalog) : datalog(datalog), graph(makeGraph(datalog.getRules())) {}

  void evaluate() {
    interpretSchemes();
    interpretFacts();
    Graph reversedGraph = graph.reverse();
    stack<int> postOrderStack;
    reversedGraph.performDFS(postOrderStack);

    vector<set<int>> SCCs;
    graph.DFSForestSCC(postOrderStack, SCCs);
    evaluateAllRules(SCCs);
    evaluateAllQueries();
  }

  vector<Relation> evaluatePredicates(const vector<Predicate>& bodyPredicates) {
    vector<Relation> results;

    for (const Predicate& predicate : bodyPredicates) {
      Relation result = evaluateQuery(predicate);
      results.push_back(result);
    }

    return results;
  }

  void interpretSchemes() {
    vector<Predicate> schemes = datalog.getSchemes();

    for (const auto& scheme : schemes) {
      string name = scheme.getName();
      vector<string> attributes = scheme.getParameters();

      Relation relation(name, attributes);
      cout << relation.toString();
      database.addRelation(name, relation);
    }
  }

  void interpretFacts() {
    vector<Predicate> facts = datalog.getFacts();

    for (const auto& fact : facts) {
      string name = fact.getName();
      vector<string> values = fact.getParameters();

      Tuple tuple(values);

      if (database.hasRelation(name)) {
        database.addTuple(name, tuple);
      }
    }
  }

  bool evaluateRules(Rule rule) {
    bool newTuplesAdded = false;
    Predicate head = rule.getHeadPredicate();
    vector<Predicate> bodyPredicates = rule.getBodyPredicates();

    vector<Relation> results = evaluatePredicates(bodyPredicates);

    Relation joinedResult = results[0];
    for (size_t i = 1; i < results.size(); i++) {
      joinedResult = joinedResult.join(results[i]);
    }

    vector<Parameter> headAttributes = head.getParameterList();
    vector<string> attributeNames;
    list<int> projectIndexes;

    for (const Parameter& param : headAttributes) {
      attributeNames.push_back(param.getValue());
    }

    for (const string& attr : attributeNames) {
      int index = joinedResult.getScheme().getIndex(attr);
      projectIndexes.push_back(index);
    }

    joinedResult = joinedResult.project(projectIndexes);

    vector<string> renameScheme;
    for (const Parameter& param : headAttributes) {
      renameScheme.push_back(param.getValue());
    }

    joinedResult = joinedResult.rename(Scheme(renameScheme));

    string relationName = head.getName();
    Relation& databaseRelation = database.getRelation(relationName);
    int beforeSize = databaseRelation.getTupleCount();
    vector<Tuple> oldTuples = databaseRelation.getTuples();

    for (const Tuple& t : joinedResult.getTuples()) {
      if (!databaseRelation.containsTuple(t)) {
        databaseRelation.addTuple(t);
      }
    }

    int afterSize = databaseRelation.getTupleCount();

    if (beforeSize != afterSize) {
      newTuplesAdded = true;

      vector<Tuple> newTuples = databaseRelation.getTuples();
      for (const Tuple& t : newTuples) {
        if (std::find(oldTuples.begin(), oldTuples.end(), t) == oldTuples.end()) {
          cout << "  ";
          for (size_t i = 0; i < t.size(); i++) {
            cout << databaseRelation.getScheme().at(i) << "=" << t.at(i);
            if (i < t.size() - 1) {
              cout << ", ";
            }
          }
          cout << endl;
        }
      }
    }


    return newTuplesAdded;
  }

  void evaluateAllRules(const vector<set<int>>& SCCs) {
    cout << "Rule Evaluation" << endl;
    vector<Rule> rules = datalog.getRules();

    for (const auto& scc : SCCs) {
      cout << "SCC: ";
      int i = 0;
      for (int node : scc) {
        cout << "R" << node;

        if (i != scc.size() - 1) {
          cout << ",";
        }
        i++;
      }
      cout << endl;

      int iterationCount = 0;
      bool newTuplesAdded = false;

      bool isSelfDependent = false;
      if (scc.size() == 1) {
        int ruleID = *scc.begin();  // only rule
        const set<int>& edges = graph.getEdges(ruleID);
        if (edges.find(ruleID) != edges.end()) {
          isSelfDependent = true;
        }
      }

      do {
        newTuplesAdded = false;
        iterationCount++;

        for (int ruleID : scc) {
          Rule& rule = rules[ruleID];
          cout << rule.toString() << endl;

          if (evaluateRules(rule)) {
            newTuplesAdded = true;
          }
        }

        if (scc.size() == 1 && !isSelfDependent) {
          break;
        }

      } while (newTuplesAdded);

      cout << iterationCount << " passes: ";
      int j = 0;
      for (int rules : scc) {
        cout << "R" << rules;
        if (j != scc.size() - 1) {
          cout << ",";
        }
        j++;
      }


      cout << endl;
    }
    cout << endl;
  }


  Relation evaluateQuery(Predicate p) {
    string relationName = p.getName();
    Relation result = database.getRelation(relationName);

    vector<string> parameters = p.getParameters();
    map<string, int> variableIndex;
    list<int> projectIndexes;
    vector<string> renameScheme;

    for (size_t i = 0; i < parameters.size(); i++) {
      string value = p.parameters[i].getValue();

      if (p.getParameterList()[i].isConstantValue()) {
        result = result.select(i, value);
      }
      else {
        if (variableIndex.find(value) != variableIndex.end()) {
          result = result.selectTwo(variableIndex[value], i);
        }
        else {
          variableIndex[value] = i;
          projectIndexes.push_back(i);
          renameScheme.push_back(value);
        }
      }
    }

    result = result.project(projectIndexes);
    result = result.rename(Scheme(renameScheme));
    return result;
  }

  void evaluateAllQueries() {
    cout << "Query Evaluation" << endl;
    vector<Predicate> queries = datalog.getQueries();

    for (Predicate query : queries) {
      cout << query.toString() << "? ";

      Relation result = evaluateQuery(query);

      if (result.getTupleCount() == 0) {
        cout << "No" << endl;
      }
      else {
        cout << "Yes(" << result.getTupleCount() << ")" << endl;
        vector<Tuple> sortedTuples = result.getSortedTuples();

        for (const auto& tuple : sortedTuples) {

          vector<string> schemeNames = result.getSchemeAttributes();
          cout << " ";
          for (size_t i = 0; i < tuple.size(); i++) {
            cout << " " << schemeNames[i] << "=" << tuple.at(i);
            if (i < tuple.size() - 1) {
              cout << ",";
            }
          }
          cout << endl;
        }
      }
    }
  }

  static Graph makeGraph(const vector<Rule>& rules) {
    Graph graph(rules.size());

    for (size_t i = 0; i < rules.size(); i++) {
      const Rule& rule = rules[i];

      for (const Predicate& bodyPredicate : rule.getBodyPredicates()) {
        string bodyName = bodyPredicate.getName();

        for (size_t j = 0; j < rules.size(); j++) {
          const Rule& toRule = rules[j];
          string headName = toRule.getHeadPredicate().getName();

          if (bodyName == headName) {
            graph.addEdge(i, j);
          }
        }
      }
    }

    return graph;

  }

};