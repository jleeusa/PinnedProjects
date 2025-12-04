#pragma once
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include "Scheme.h"
#include "Tuple.h"

using namespace std;

class Relation {

private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:

    Relation() : name(""), scheme(Scheme({})) {}

    Relation(const string& name, const Scheme& scheme)
      : name(name), scheme(scheme) {}

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    int getTupleCount() {
        return tuples.size();
    }

    bool containsTuple(const Tuple& t) const {
        return tuples.find(t) != tuples.end();
    }

    vector<string> getSchemeAttributes() {
        return scheme.getAttributes();
    }

    const Scheme& getScheme() const {
        return scheme;
    }

    vector<Tuple> getSortedTuples() {
        vector<Tuple> sortedTuples(tuples.begin(), tuples.end());
        return sortedTuples;
    }

    vector<Tuple> getTuples() const {
        return vector<Tuple>(tuples.begin(), tuples.end());
    }

    bool contains(const Tuple& t) const {
        return tuples.find(t) != tuples.end();
    }

    string toString() const {
        stringstream out;
        for (auto& tupleValue : tuples) {
            Tuple tuple(tupleValue);
            cout << tuple.toString(scheme) << endl;
        }
        return out.str();
    }

    void print() const {
        cout << "Relation: " << name << endl;
        if (tuples.empty()) {
            cout << "  (No facts found)" << endl;
        } else {
            for (const auto& tuple : tuples) {
                cout << "  (";
                for (size_t i = 0; i < tuple.size(); i++) {
                    cout << tuple.at(i);
                    if (i < tuple.size() - 1) cout << ", ";
                }
                cout << ")" << endl;
            }
        }
    }

    Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (auto& tupleValue : tuples) {
            if (tupleValue.at(index) == value) {
                result.addTuple(tupleValue);
            }
        }
        return result;
    }

    Relation selectTwo(int indexOne, int indexTwo) const {
        Relation result(name, scheme);
        for (auto& tupleValue : tuples) {
          if (tupleValue.at(indexOne) == tupleValue.at(indexTwo)) {
            result.addTuple(tupleValue);
          }
        }
        return result;
    }

    Relation project(list<int> indexes) const {
        vector<string> newHeaders;
        for (size_t i = 0; i < indexes.size(); i++) {
            newHeaders.push_back(scheme.at(i));
        }
        Scheme newScheme = Scheme(newHeaders);
        Relation result(name, newScheme);
        for (auto& tupleValue : tuples) {
            vector<string> newTuple;
            for (auto& index : indexes) {
                newTuple.push_back(tupleValue.at(index));
          }
          result.addTuple(newTuple);
        }
        return result;
    }

    Relation rename(Scheme renameScheme) {
        scheme = renameScheme;
        Relation result(name, scheme);
        for (auto& tupleValue : tuples) {
            result.addTuple(tupleValue);
        }
        return result;
    }

    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
               const Tuple& leftTuple, const Tuple& rightTuple) {
        // add code to test whether the tuples are joinable
        for (size_t i = 0; i < leftScheme.getAttributes().size(); i++) {
            for (size_t j = 0; j < rightScheme.getAttributes().size(); j++) {
                if (leftScheme.getAttributes()[i] == rightScheme.getAttributes()[j]) {
                    if (leftTuple.at(i) != rightTuple.at(j)) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    Relation join(const Relation& right) {
        const Relation& left = *this;

        vector<string> combinedScheme = left.scheme.getAttributes();
        vector<int> rightIndexes;
        vector<int> leftIndexes;

        for (size_t i = 0; i < right.scheme.getAttributes().size(); i++) {
            string attr = right.scheme.getAttributes()[i];
            if (std::find(combinedScheme.begin(), combinedScheme.end(), attr) == combinedScheme.end()) {
                combinedScheme.push_back(attr);
                rightIndexes.push_back(i);
            }
        }

        Relation result(left.name + "_JOIN_" + right.name, Scheme(combinedScheme));

        for (const Tuple& leftTuple : left.tuples) {
            //cout << "left tuple: " << leftTuple.toString(left.scheme) << endl;

            for (const Tuple& rightTuple : right.tuples) {
                //cout << "right tuple: " << rightTuple.toString(right.scheme) << endl;

                if (joinable(left.scheme, right.scheme, leftTuple, rightTuple)) {
                    vector<string> combinedTuple = leftTuple.getValues();

                    for (int index : rightIndexes) {
                        combinedTuple.push_back(rightTuple.at(index));
                    }
                    result.addTuple(Tuple(combinedTuple));
                }
            }
        }

        return result;
    }


};