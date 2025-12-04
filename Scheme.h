#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Scheme {

private:
    vector<string> names;

public:

    Scheme(vector<string> names) : names(names) { }

    const string& at(int index) const {
        return names.at(index);
    }

    unsigned size() const {
        return names.size();
    }

    int getIndex(const string& attr) const {
        for (size_t i = 0; i < names.size(); i++) {
            if (names[i] == attr) {
                return i;
            }
        }
        return -1;
    }

    vector<string> getAttributes() const{
        return names;
    }

    string getAttribute(int index) const {
        return names[index];
    }

    string toString() const {
        stringstream ss;
        for (size_t i = 0; i < names.size(); i++) {
            ss << names[i];
            if (i < names.size() - 1) {
                ss << ", ";
            }
        }
        return ss.str();
    }

    void rename(const vector<string>& newNames) {
        names = newNames;
    }

};