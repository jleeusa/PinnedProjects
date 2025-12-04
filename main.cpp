#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include "DatalogProgram.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Scanner.h"
#include "Graph.h"

using namespace std;

int main(int argc, char* argv[]) {
    string file = argv[1];

    ifstream input(file);
    if (!input) {
        cerr << "Error: Could not open file " << file << endl;
        return 1;
    }

    stringstream ss;
    ss << input.rdbuf();
    string inputFile = ss.str();
    input.close();


    try {

        Scanner s = Scanner(inputFile);
        vector<Token> tokens;

        while (true) {
            Token temp = s.scanToken();
            if (temp.getType() != COMMENT) {
                tokens.push_back(temp);
            }
            if (temp.getType() == END) {
                break;
            }
        }

        Parser p = Parser(tokens);
        DatalogProgram datalog = p.getDatalog();
        vector<Rule> rules = datalog.getRules();

        Graph graph = Interpreter::makeGraph(rules);
        cout << graph.toString();

        Graph reverseGraph = graph.reverse();
        reverseGraph.toString();

        stack<int> postOrderStack;
        reverseGraph.DFSForestPostOrder(postOrderStack);

        vector<set<int>> SCCs;
        graph.DFSForestSCC(postOrderStack, SCCs);

        Interpreter interpreter(datalog);
        interpreter.evaluate();

    }
    catch (...) {
        return EXIT_FAILURE;
    }

    return 0;

}

