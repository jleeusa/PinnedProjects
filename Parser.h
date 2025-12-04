#pragma once
#include <iostream>
#include <vector>
#include <set>
#include "Token.h"
#include "DatalogProgram.h"

using namespace std;

class Parser {
private:
    DatalogProgram db = DatalogProgram();

public:
    Parser(const vector<Token> tokens) : tokens(tokens) {
        try {
            datalogProgram();
        }
        catch (Token a) {
            cout << "Failure!" << endl <<  "  " << a.toString() << endl;
            throw;
        }
    }

    vector<Token> tokens;
    vector<Predicate> schemeVec;
    vector<Predicate> predicateVec;
    vector<Predicate> factVec;
    vector<Predicate> queryVec;
    vector<Rule> ruleVec;
    set<string> domainSet;
    vector<Parameter> parameterVec;
    string place;

    TokenType tokenType() const {
        return tokens.at(0).getType();
    }

    DatalogProgram getDatalog() {
        return db;
    }

    void advanceToken() {
        tokens.erase(tokens.begin());
    }

    void throwError() {
        if (tokens.empty()) {
            cout << "Error: Attempted to throw an error, but token list is empty." << endl;
            exit(EXIT_FAILURE);
        }
        throw tokens.at(0);
    }

    void match(TokenType t) {
        // add code for matching token type t
        if (tokens.at(0).getType() == END) {

        }
        else if (tokens.at(0).getType() == t) {
            advanceToken();
        }
        else {
            throwError();
        }
    }

    //LISTS
    DatalogProgram datalogProgram() {
        match(SCHEMES);
        match(COLON);
        scheme();
        schemeList();
        schemeVec = predicateVec;
        predicateVec.clear();
        predicateVec.shrink_to_fit();

        match(FACTS);
        match(COLON);
        place = "facts";
        factList();
        factVec = predicateVec;
        predicateVec.clear();
        predicateVec.shrink_to_fit();

        match(RULES);
        match(COLON);
        ruleList();
        predicateVec.clear();
        predicateVec.shrink_to_fit();

        match(QUERIES);
        match(COLON);
        query();
        queryList();
        queryVec = predicateVec;
        predicateVec.clear();
        predicateVec.shrink_to_fit();
        match(END);
        DatalogProgram datalog = DatalogProgram(schemeVec, factVec, queryVec, ruleVec, domainSet);
        this->db = datalog;
        return datalog;
    }

    void schemeList() {
        if (tokenType() == ID) {
            scheme();
            schemeList();
        }
    }

    void factList() {
        if (tokenType() == ID) {
            fact();
            factList();
        }
    }

    void ruleList() {
        if (tokenType() == ID) {
            rule();
            ruleList();
        }
    }


    void queryList() {
        if (tokenType() == ID) {
            query();
            queryList();
        }
    }

    void scheme() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        string currParam = tokens.at(0).getValue();
        Parameter tempParam(tokens.at(0).getValue());
        parameterVec.push_back(tempParam);
        match(ID);
        idList();
        match(RIGHT_PAREN);
        predicateVec.push_back(Predicate(name, parameterVec));
        parameterVec.clear();
        parameterVec.shrink_to_fit();
    }

    void fact() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        string currParam = tokens.at(0).getValue();
        parameterVec.push_back(currParam);
        domainSet.insert(currParam);
        match(STRING);
        stringList();
        match(RIGHT_PAREN);
        match(PERIOD);
        predicateVec.push_back(Predicate(name, parameterVec));
        parameterVec.clear();
        parameterVec.shrink_to_fit();
    }

    void rule() {
        Predicate headPred = headPredicate();
        match(COLON_DASH);
        predicate();
        predicateList();
        match(PERIOD);
        ruleVec.push_back(Rule(headPred, predicateVec));
        parameterVec.clear();
        parameterVec.shrink_to_fit();
        predicateVec.clear();
        predicateVec.shrink_to_fit();
    }

    void query() {
        string name = tokens.at(0).getValue();
        predicate();
        match(Q_MARK);

        parameterVec.clear();
        parameterVec.shrink_to_fit();
    }

    Predicate headPredicate() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        string currentToken = tokens.at(0).getValue();
        match(ID);
        parameterVec.push_back(currentToken);
        idList();
        Predicate hPredicate = Predicate(name, parameterVec);
        match(RIGHT_PAREN);

        parameterVec.clear();
        parameterVec.shrink_to_fit();

        return hPredicate;
    }

    void predicate() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        parameter();
        parameterList();
        match(RIGHT_PAREN);
        predicateVec.push_back(Predicate(name, parameterVec));

        parameterVec.clear();
        parameterVec.shrink_to_fit();
    }

    void predicateList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            predicate();
            predicateList();
        }
    }

    void parameterList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            parameter();
            parameterList();
        }
    }

    void stringList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            string currentToken = tokens.at(0).getValue();
            parameterVec.push_back(currentToken);
            match(STRING);
            if (place == "facts") {
                domainSet.insert(currentToken);
            }
            stringList();
        }
    }

    void idList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            string currentToken = tokens.at(0).getValue();
            parameterVec.push_back(currentToken);
            match(ID);
            idList();
        }
    }

    void parameter() {
        if (tokenType() == STRING) {
            string currToken = tokens.at(0).getValue();
            parameterVec.push_back(currToken);
            match(STRING);
        }
        else if (tokenType() == ID) {
            string currToken = tokens.at(0).getValue();
            parameterVec.push_back(currToken);
            match(ID);
        }
        else {
            throwError();
        }
    }


};