#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Tuple {

private:
    vector<string> values;

public:

    Tuple(vector<string> values) : values(values) { }

    string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (size_t i = 0; i < scheme.size(); i++) {
            out << scheme.at(i) << " = " << tuple.at(i);
            if (i < scheme.size() - 1) {
                out << ", ";
            }
        }
        return out.str();
    }

    unsigned size() const {
        return values.size();
    }

    const string& at(int index) const {
        return values.at(index);
    }

    bool operator<(const Tuple& t) const {
        return this->values < t.values;
    }

    bool operator==(const Tuple& other) const {
        return values == other.values;
    }

    vector<string> getValues() const {
        return values;
    }

};