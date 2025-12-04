#pragma once
#include <iostream>

using namespace std;

class Node {

private:

    set<int> adjacentNodeIDs;

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    const set<int>& getEdges() const {
        return adjacentNodeIDs;
    }

    bool hasEdgeTo(int toNodeID) const {
        return adjacentNodeIDs.find(toNodeID) != adjacentNodeIDs.end();
    }

    string toString() {
        ostringstream out;
        auto it = adjacentNodeIDs.begin();
        while (it != adjacentNodeIDs.end()) {
            out << "R" << *it;
            ++it;
            if (it != adjacentNodeIDs.end()) {
                out << ",";
            }
        }

        return out.str();
    }

};