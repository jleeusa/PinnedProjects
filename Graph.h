#pragma once
#include <iostream>
#include <stack>
#include "Node.h"
#include "Rule.h"

using namespace std;

class Graph {

private:

    map<int,Node> nodes;
    map<int, Node> reverseNodes;
    map<int, int> ruleToSCCMapping;

public:
    vector<Rule> rules;

    Graph(const vector<Rule>& rules) {
        this->rules = rules;
    }

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++) {
            nodes[nodeID] = Node();
            reverseNodes[nodeID] = Node();
        }
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
        reverseNodes[toNodeID].addEdge(fromNodeID);
    }

    const set<int>& getEdges(int nodeID) const {
        return nodes.at(nodeID).getEdges();
    }

    string toString() const{
        ostringstream out;
        out << "Dependency Graph" << endl;
        for (const auto& pair: nodes) {
            int nodeID = pair.first;
            const set<int>& edges = pair.second.getEdges();

            out << "R" << nodeID << ":";
            if (!edges.empty()) {
                vector<int> sortedEdges(edges.begin(), edges.end());
                sort(sortedEdges.begin(), sortedEdges.end());

                for (size_t i = 0; i < sortedEdges.size(); ++i) {
                    out << "R" << sortedEdges[i];
                    if (i < sortedEdges.size() - 1) {
                        out << ",";
                    }
                }
            }
            out << endl;
        }
        out << endl;
        return out.str();
    }

    Graph reverse() const {
        Graph reversed(nodes.size());
        for (const auto& pair : nodes) {
            int nodeID = pair.first;
            reversed.nodes[nodeID] = Node();
            reversed.reverseNodes[nodeID] = Node();
        }
        for (const auto& pair : nodes) {
            int from = pair.first;
            const set<int>& edges = pair.second.getEdges();
            for (int to : edges) {
                reversed.addEdge(to, from);
            }
        }
        return reversed;
    }

    void DFSForestPostOrder(stack<int>& postOrderStack) const {
        set<int> visited;
        vector<int> nodeIDs;

        for (const auto& pair : nodes) {
            nodeIDs.push_back(pair.first);
        }

        sort(nodeIDs.begin(), nodeIDs.end());

        for (int nodeID : nodeIDs) {
            if (visited.find(nodeID) == visited.end()) {
                DFSUtil(nodeID, visited, postOrderStack);
            }
        }
    }

    void performDFS(stack<int>& postOrderStack) {
        set<int> visited;

        vector<int> nodeIDs;
        for (const auto& pair : nodes) {
            nodeIDs.push_back(pair.first);
        }
        sort(nodeIDs.begin(), nodeIDs.end());

        for (int nodeID : nodeIDs) {
            if (visited.find(nodeID) == visited.end()) {
                DFSUtil(nodeID, visited, postOrderStack);
            }
        }
    }

    void DFSVisit(int nodeID, set<int>& visited, stack<int>& postOrderStack, const vector<Rule>& rules) {
        visited.insert(nodeID);

        for (size_t j = 0; j < rules.size(); ++j) {
            if (nodeID != j) {
                if (visited.find(j) == visited.end()) {
                    DFSVisit(j, visited, postOrderStack, rules);
                }
            }
        }

        postOrderStack.push(nodeID);
    }


    void DFSUtil(int nodeID, set<int>& visited, stack<int>& postOrderStack) const {
        visited.insert(nodeID);

        for (int neighbor : nodes.at(nodeID).getEdges()) {
            if (visited.find(neighbor) == visited.end()) {
                DFSUtil(neighbor, visited, postOrderStack);
            }
        }

        postOrderStack.push(nodeID);
    }

    void DFSForestSCC(stack<int>& postOrderStack, vector<set<int>>& SCCs) {
        set<int> visited;
        Graph reversedGraph = this->reverse();
        //Graph graph = *this;

        while (!postOrderStack.empty()) {
            int nodeID = postOrderStack.top();
            postOrderStack.pop();

            if (visited.find(nodeID) == visited.end()) {
                set<int> currentSCC;
                DFSReverse(nodeID, visited, currentSCC, reversedGraph);
                SCCs.push_back(currentSCC);
            }
        }

        for (size_t sccIndex = 0; sccIndex < SCCs.size(); ++sccIndex) {
            set<int>& scc = SCCs[sccIndex];
            for (int nodeID : scc) {
                ruleToSCCMapping[nodeID] = sccIndex;
            }
        }
    }

    void DFSCollect(int nodeID, set<int>& visited, vector<int>& currentSCC) {
        visited.insert(nodeID);
        currentSCC.push_back(nodeID);

        for (int neighbor : nodes.at(nodeID).getEdges()) {
            if (visited.find(neighbor) == visited.end()) {
                DFSCollect(neighbor, visited, currentSCC);
            }
        }
    }

    void DFSReverse(int nodeID, set<int>& visited, set<int>& scc, const Graph& reversedGraph) {
        visited.insert(nodeID);
        scc.insert(nodeID);

        for (int neighbor : reversedGraph.reverseNodes.at(nodeID).getEdges()) {
            if (visited.find(neighbor) == visited.end()) {
                DFSReverse(neighbor, visited, scc, reversedGraph);
            }
        }
    }

    bool hasSelfLoop(int nodeID) const{
        const Node& node = nodes.at(nodeID);
        return node.hasEdgeTo(nodeID);
    }

};