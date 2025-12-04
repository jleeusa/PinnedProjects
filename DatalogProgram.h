#pragma once
#include <iostream>
#include <set>
#include <string>
#include "Predicate.h"
#include "Rule.h"

using namespace std;

class DatalogProgram {
private:

    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Predicate> queries;
    vector<Rule> rules;
    set<string> domain;

public:
    DatalogProgram(vector<Predicate> schemes, vector<Predicate> facts, vector<Predicate> queries, vector<Rule> rules,
        set<string> domain) : schemes(schemes), facts(facts), queries(queries), rules(rules), domain(domain) {};

    DatalogProgram() {};

    vector<Predicate> getQueries() const{
        return queries;
    }

    vector<Rule> getRules() const {
        return rules;
    }

    vector<Predicate> getSchemes() const {
        return schemes;
    }

    vector<Predicate> getFacts() const {
        return facts;
    }

     string toString() const {
          ostringstream out;

         //Print schemes
         out << "Success!" << endl << "Schemes(" << schemes.size() << "):" << endl;
         for (size_t i = 0; i < schemes.size(); i++) {
             out << "  " << schemes[i].toString() << endl;
         }

         //Print facts
         out << "Facts(" << facts.size() << "):" << endl;
        for (size_t i = 0; i < facts.size(); i++) {
            out << "  " << facts[i].toString() << "." << endl;
        }

         //Print rules
         out << "Rules(" << rules.size() << "):" << endl;
         for (size_t i = 0; i < rules.size(); i++) {
             out << "  " << rules[i].toString() << endl;
         }

        //Print queries
        out << "Queries(" << queries.size() << "):" << endl;
         for (size_t i = 0; i < queries.size(); i++) {
             out << "  " << queries[i].toString() << "?" << endl;
         }

         //Print domain
         out << "Domain(" << domain.size() << "):" << endl;
         for (const string & value : domain) {
             out << "  " << value << endl;
         }

          return out.str();
     }

};