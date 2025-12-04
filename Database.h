#include <iostream>
#include <map>
#include "Relation.h"

using namespace std;

class Database {
  private:

    map<string, Relation> relations;

  public:

    Database() {}

    Database(map<string, Relation>);

  void addRelation(string name, Relation relation) {
    relations[name] = relation;
  }

  Relation& getRelation(const string& name) {
    return relations.at(name);
  }

  bool hasRelation(string name) {
    return relations.find(name) != relations.end();
  }

  void addTuple(string name, Tuple tuple) {
    relations[name].addTuple(tuple);
  }

  void print() const {
    cout << "Database Contents:" << endl;
    for (const auto& pair : relations) {
      pair.second.print();
    }
  }

};