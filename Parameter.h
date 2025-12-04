#pragma once
#include <iostream>

using namespace std;

class Parameter {
public:
    string value;
    bool isConstant;

    Parameter(string val) : value(val) {
        isConstant = (val.front() == '\'' && val.back() == '\'');
    }

    string toString() const {
        return value;
    }

    string getValue() const{
        return value;
    }

    bool isConstantValue() const {
        return isConstant;
    }

};