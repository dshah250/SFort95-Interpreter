// Dharam Shah
// CS 280 - 10
// 2/26/2024
// PA1: test.cpp

#include "lex.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <cctype>
#include <string>

using namespace std;

int main(int argc, char *argv[ ]) 
{    
    vector<int> integers;
    vector<double> realList;
    vector<string> strings;
    vector<string> all;
    vector<string> titleFile;

    map<string, int> identifiers;
    map<string, int> keyW;
    
    int lineNumber = 1;

    map<string, bool> cmdFlag =
    {
        {"-kw" , false}, {"-id", false}, {"-str", false}, {"-real", false}, {"-int", false}, {"-all", false},
    };

    for(int counter = 1; counter < argc; counter++)
    {
        string argu = argv[counter];
        if (argu[0] == '-') 
        {
            if (cmdFlag.count(argu) == 0) 
            {
                cout << "UNRECOGNIZED FLAG {" << argu << "}" << endl;
                exit(1);
            } 
            else 
            {
                cmdFlag[argu] = true;
            }
        } 
        else 
        {
            if (titleFile.empty() == false) 
            {
                cout << "ONLY ONE FILE NAME IS ALLOWED." << endl;
                exit(1);
            }
            titleFile.insert(titleFile.end(), argu);
        }
    }

    
    if (argc <= 1)
    {
        cout << "NO SPECIFIED INPUT FILE." << endl;
        exit(1);
    }

    ifstream testFile;
    string line;
    string fname = titleFile[0];
    testFile.open(fname.c_str());

    if (titleFile.empty() == true)
    {
        cout << "NO SPECIFIED INPUT FILE." << endl;
        exit(1);
    }   
   
    if (!testFile)
    {
        cerr << "CANNOT OPEN THE FILE " << fname << endl;
        exit (1);
    } 

    if(testFile.peek() == EOF)
        {
            cout << "Empty File." << endl;
            exit(1);
        }  

    LexItem token;
    while((token = getNextToken(testFile, lineNumber)) != DONE && token != ERR)
    {
        string lexeme = token.GetLexeme();
        all.insert(all.end(), token.GetLexeme());
        if(cmdFlag["-all"])
        {
            cout << token;
        } 
        if(token.GetToken() == SCONST)
        {
            if(find(strings.begin(), strings.end(), lexeme) == strings.end())
            {
                strings.insert(strings.end(),lexeme);
            }
        }
        if(token.GetToken() == RCONST)
        {
            if(find(realList.begin(), realList.end(), stod(lexeme)) == realList.end())
            {
                realList.insert(realList.end(), stod(token.GetLexeme()));
            }
        }
        if(token.GetToken() == ICONST)
        {
            if(find(integers.begin(), integers.end(), stoi(lexeme)) == integers.end())
            {
                integers.insert(integers.end(), stoi(lexeme));
            }
        }
        if(token.GetToken() == IDENT)
        {
            identifiers[lexeme]++;
        }
        if((token.GetToken() == PROGRAM || token.GetToken() == PRINT || token.GetToken() == IF || token.GetToken() == ELSE || token.GetToken() == IDENT || token.GetToken() == END || token.GetToken() == INTEGER || token.GetToken() == REAL || token.GetToken() == CHARACTER || token.GetToken() == THEN) && token.GetToken() != IDENT)
        {
            keyW[lexeme]++;
        }
    }
    
    if(token.GetToken() == ERR)
    {
        cout << token;
        exit(1);
    }
    
    cout << endl;
    cout << "Lines: " << (lineNumber - 1) << endl;
    cout << "Total Tokens: " << distance(all.begin(), all.end()) << endl;
    cout << "Identifiers: " << distance(identifiers.begin(), identifiers.end()) << endl;
    cout << "Integers: " << distance(integers.begin(), integers.end()) << endl;
    cout << "Reals: " << distance(realList.begin(), realList.end()) << endl;
    cout << "Strings: " << distance(strings.begin(), strings.end()) << endl;

    if(cmdFlag["-id"] == true && (identifiers.empty() == false))
    {
        cout << "IDENTIFIERS:" << endl;
        for (auto iter = identifiers.begin(); iter != identifiers.end(); ++iter) 
        {
            cout << iter->first << " (" << iter->second << ")";
            if (next(iter) != identifiers.end()) 
            {
                cout << ", ";
            }
        }
        cout << endl;
    }

    if (cmdFlag["-kw"] == true && (keyW.empty() == false))
    {
        cout << "KEYWORDS:" << endl;
        for (auto iter = keyW.begin(); iter != keyW.end(); ++iter) 
        {
            cout << iter->first << " (" << iter->second << ")";
            if (next(iter) != keyW.end()) 
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
            
    sort(integers.begin(), integers.end());
    if (cmdFlag["-int"] == true && integers.empty() == false)
    {
        cout << "INTEGERS:" << endl;
        for (const auto& integer : integers) 
        {
            cout << integer;
            if (&integer != &integers.back()) 
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
    
    sort(realList.begin(), realList.end());
    if (cmdFlag["-real"] == true && (realList.empty() == false))
    {
        cout << "REALS:" << endl;
        for (const auto& real : realList) 
        {
            cout << real;
            if (&real != &realList.back()) 
            {
                cout << ", ";
            }
        }
        cout << endl;
    }

    if(cmdFlag["-str"] == true && strings.empty() == false)
    {
        cout << "STRINGS:" << endl;
        for (const auto& str : strings) 
        {
            cout << str;
            if (&str != &strings.back()) 
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
}