#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Parameter.h"

using namespace std;

class Predicate {
public:
    Predicate(string predicateName, vector<Parameter> parameters) :  name(predicateName), parameters(parameters) { }

    string name;
    vector<Parameter> parameters;

    string toString() const {
        stringstream out;
        out << name << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters[i].toString();
            if (i < parameters.size() - 1) {
                out << ",";
            }
        }
        out << ")";
        return out.str();
    }

    string getName() const {
        return name;
    }

    vector<string> getParameters() const {
        vector<string> paramStrings;
        for (const Parameter& param : parameters) {
            paramStrings.push_back(param.getValue());
        }
        return paramStrings;
    }

    vector<Parameter> getParameterList() const {
        return parameters;
    }

};