#pragma once
#include <iostream>
#include <cctype>
#include "Token.h"

using namespace std;

class Scanner {
private:
    string input;
    int numTokens = 0;
    int lineNumber = 1;
    string fileInput;

public:
    Scanner(const string& input) : fileInput(input) { }

    int getLine() {
        return lineNumber;
    }

    int getNumTokens() {
        return numTokens;
    }

    Token scanToken() {
        while (!fileInput.empty() && isspace(fileInput.at(0))) {
            if (fileInput.at(0) == '\n') {
                lineNumber++;
            }
            fileInput = fileInput.substr(1);
        }
        if (fileInput.empty()) {
            TokenType type = END;
            string value = "";
            numTokens++;
            return Token(type, value, lineNumber);
        }
        else if (fileInput.at(0) == ',') {
            TokenType type = COMMA;
            string value = ",";
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
        else if (fileInput.at(0) == '.') {
            TokenType type = PERIOD;
            string value = ".";
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
        else if (fileInput.at(0) == '?') {
            TokenType type = Q_MARK;
            string value = "?";
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
        else if (fileInput.at(0) == '(') {
            TokenType type = LEFT_PAREN;
            string value = "(";
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
        else if (fileInput.at(0) == ')') {
            TokenType type = RIGHT_PAREN;
            string value = ")";
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
        else if (fileInput.at(0) == ':') {
            if (fileInput.length() > 1 && fileInput.at(1) == '-') {
                TokenType type = COLON_DASH;
                string value = ":-";
                numTokens++;
                fileInput = fileInput.substr(2);
                return Token(type, value, lineNumber);
            }
            else {
                TokenType type = COLON;
                string value = ":";
                numTokens++;
                fileInput = fileInput.substr(1);
                return Token(type, value, lineNumber);
            }
        }
        else if (fileInput.at(0) == '*') {
            TokenType type = MULTIPLY;
            string value = "*";
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
        else if (fileInput.at(0) == '+') {
            TokenType type = ADD;
            string value = "+";
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
        else if (isalpha(fileInput.at(0))) {
            string id;
            while (isalpha(fileInput.at(0)) || isdigit(fileInput.at(0))) {
                id.push_back(fileInput.at(0));
                fileInput = fileInput.substr(1);
            }
            if (id == "Schemes") {
                TokenType type = SCHEMES;
                string value = "Schemes";
                numTokens++;
                return Token(type, value, lineNumber);
            }
            else if (id == "Facts") {
                TokenType type = FACTS;
                string value = "Facts";
                numTokens++;
                return Token(type, value, lineNumber);
            }
            else if (id == "Rules") {
                TokenType type = RULES;
                string value = "Rules";
                numTokens++;
                return Token(type, value, lineNumber);
            }
            else if (id == "Queries") {
                TokenType type = QUERIES;
                string value = "Queries";
                numTokens++;
                return Token(type, value, lineNumber);
            }
            else {
                TokenType type = ID;
                string value = id;
                numTokens++;
                return Token(type, value, lineNumber);
            }
        }
         else if (fileInput.at(0) == '\'') {
             string str;
             int stringLineNumber = lineNumber;
             str = fileInput.at(0);
             if (fileInput.length() < 2) {
                 TokenType type = UNDEFINED;
                 string value = to_string(fileInput.at(0));
                 return Token(type, value, stringLineNumber);
             }
             fileInput = fileInput.substr(1);
             while (fileInput.length() > 0) {
                 if (fileInput.at(0) == '\n') {
                     lineNumber++;
                 }
                 //back to back ''
                 if (fileInput.at(0) == '\'' && fileInput.length() > 1 && fileInput.at(1) == '\'') {
                     str.push_back(fileInput.at(0));
                     str.push_back(fileInput.at(1));
                     fileInput = fileInput.substr(2);
                 }
                 else if (fileInput.at(0) == '\'') {
                     str.push_back(fileInput.at(0));
                     fileInput = fileInput.substr(1);
                     break;
                 }
                 else {
                     str.push_back(fileInput.at(0));
                     fileInput = fileInput.substr(1);
                 }
             }
             if (fileInput.empty() || str.back() != '\'') {
                 TokenType type = UNDEFINED;
                 string value = str;
                 numTokens++;
                 return Token(type, value, stringLineNumber);
             }

             TokenType type = STRING;
             string value = str;
             numTokens++;
             return Token(type, value, stringLineNumber);
         }
         else if (fileInput.at(0) == '#') {
             string str;
             if (fileInput.length() > 1 && fileInput.at(1) == '|') {
                 str = fileInput.at(0);
                 int commentLineNumber = lineNumber;
                 while (fileInput.length() > 0 && fileInput.at(0) != '|' && fileInput.at(1) != '#') {
                     str.push_back(fileInput.at(0));
                     fileInput = fileInput.substr(1);
                     if (fileInput.at(0) == '\n') {
                         lineNumber++;
                     }
                     if (fileInput.at(0) == '|' && fileInput.at(1) == '#') {
                         TokenType type = COMMENT;
                         string value = str;
                         numTokens++;
                         fileInput = fileInput.substr(1);
                         return Token(type, value, commentLineNumber);
                     }
                 }
                 TokenType type = UNDEFINED;
                 string value = str;
                 numTokens++;
                 return Token(type, value, lineNumber);
             }
             else {
                 while (!fileInput.empty() && fileInput.at(0) != '\n') {
                     str.push_back(fileInput.at(0));
                     fileInput = fileInput.substr(1);
                 }
                 TokenType type = COMMENT;
                 string value = str;
                 numTokens++;
                 return Token(type, value, lineNumber);
             }
         }
        else {
            string undefined;
            undefined.push_back(fileInput.at(0));
            TokenType type = UNDEFINED;
            string value = undefined;
            numTokens++;
            fileInput = fileInput.substr(1);
            return Token(type, value, lineNumber);
        }
    }
};