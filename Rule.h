#pragma once
#include <iostream>
#include "Token.h"
#include "Predicate.h"

using namespace std;

class Rule {
public:
    Rule(Predicate head, vector<Predicate> predVec) : headPredicate(head), bodyPredicates(predVec) {}

    Predicate headPredicate;
    vector<Predicate> bodyPredicates;

    string toString() const {
        ostringstream out;
        out << headPredicate.toString() << " :- ";
        for (size_t i = 0; i < bodyPredicates.size(); i++) {
            out << bodyPredicates[i].toString();
            if (i < bodyPredicates.size() - 1) {
                out << ",";
            }
        }
        out << ".";
        return out.str();
    }

    Predicate getHeadPredicate() const {
        return headPredicate;
    }

    vector<Predicate> getBodyPredicates() const {
        return bodyPredicates;
    }

    void addBodyPredicate(Predicate p) {
        bodyPredicates.push_back(p);
    }
};