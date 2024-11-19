// Dharam Shah
// CS 280 - 10
// 2/26/2024
// PA1: lex.cpp

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "lex.h"

using namespace std;

LexItem getNextToken(istream& in, int& linenum)
{
    enum TokState {ININT, REAL, STRING, START, INID} lexstate = START;
    string lexeme;
    char c;
    while(in.get(c)) 
    {
        switch(lexstate)
        {
        case START:
            if (c == '!')
            {
                string comment;
                getline(in, comment);
                linenum++;
                break;
            } 
            else if(c == ')')
            {
                lexeme += c;
                return LexItem(RPAREN, lexeme, linenum);
            } 
            else if(c == '(')
            {
                lexeme += c;
                return LexItem(LPAREN, lexeme, linenum);
            } 
            else if (c == '>')
            {
                lexeme += c;
                return LexItem(GTHAN, lexeme, linenum);
            } 
            else if(c == '<')
            {
                lexeme += c;
                return LexItem(LTHAN, lexeme, linenum);
            } 
            else if(c == '\r')
            {
                break;
            }
            else if (c == '\n')
            {
                linenum++;
            } 
            else if (c == ' ')
            {
                break;
            } 
            else if (c == '\t')
            {
                break; 
            }
            else if (c == '+')
            {
                lexeme += c;
                return LexItem(PLUS, lexeme, linenum);
            } 
            else if (c == '*')
            {
                lexeme += c;
                if(in.peek() == '*')
                {
                    in.get(c);
                    lexeme += c;
                    return LexItem(POW, lexeme, linenum);
                } 
                else if (in.peek() == ',')
                {
                    return LexItem(DEF, lexeme, linenum);
                }
                return LexItem(MULT, lexeme, linenum);
            } 
            else if (c == '-')
            {
                lexeme += c;
                return LexItem(MINUS, lexeme, linenum);
            } 
            else if (c == '/')
            {
                lexeme += c;
                if(in.peek() == '/'){
                    in.get(c);
                    lexeme += c;
                    return LexItem(CAT, lexeme, linenum);
                }
                return LexItem(DIV, lexeme, linenum);
            } 
            else if(c == '=')
            {
                lexeme += c;
                if (in.peek() == '=')
                {
                    in.get(c);
                    lexeme += c;
                    return LexItem(EQ, lexeme, linenum);
                }
                return LexItem(ASSOP, lexeme, linenum);
            } 
            else if (c == ':')
            {
                lexeme += c;
                if(in.peek() == ':')
                {
                    in.get(c);
                    lexeme += c;
                    return LexItem(DCOLON, lexeme, linenum);
                }
                return LexItem(ERR, lexeme, linenum);
            } 
            else if (c == ',')
            {
                lexeme += c;
                return LexItem(COMMA, lexeme, linenum);
            } 
            else if (c == '.')
            {
                if(isdigit(in.peek()))
                {
                    lexstate = REAL;
                    lexeme += c;
                    break;
                }
                lexeme += c;
                return LexItem(DOT, lexeme, linenum);
            } 
            else if (isalpha(c))
            {
                lexeme += c;
                lexstate = INID;
            } 
            else if (c == '"' || c == '\'')
            {
                lexeme += c;
                lexstate = STRING;
            } 
            else if (isdigit(c))
            {
                lexeme += c;
                lexstate = ININT;
            } 
            else
            {
                lexeme += c;
                return LexItem(ERR, lexeme, linenum);
            }
            break;

        case ININT:
            if(isdigit(c) == false)
            {
                if(c == '.')
                {
                    lexstate = REAL;
                } else 
                {
                    in.putback(c);
                    return LexItem(ICONST, lexeme, linenum);
                }
            }
            lexeme += c;
            break;
        
        case STRING:
            if (c == '"' || c == '\'')
            {
                if(c == lexeme[0])
                {
                    lexeme[0] = '"';
                    lexeme += '"';
                    return LexItem(SCONST, lexeme, linenum);
                } 
                else 
                {
                    lexeme += c;
                    return LexItem(ERR, lexeme, linenum);
                }
            }
            if (c == '\n')
            {
                return LexItem(ERR, lexeme, linenum);
            }
            lexeme += c;
            break;

        case REAL:
            if(isdigit(c) == false)
            {
                if(c == '.')
                {
                    lexeme += c;
                    return LexItem(ERR, lexeme, linenum);
                } 
                else 
                {
                    in.putback(c);
                    return LexItem(RCONST, lexeme, linenum);
                }
            }
            lexeme += c;
            break;

        case INID:
            if ((isalpha(c) == false && isdigit(c) == false) && c != '_') 
            {
                in.putback(c);
                return id_or_kw(lexeme, linenum);
            }
            lexeme += c;
            break;

    }
    }
    return LexItem(DONE, lexeme, linenum);
}

map<Token, string> TokenString = 
{
    {PROGRAM, "PROGRAM"},
    {PRINT, "PRINT"},
    {IF, "IF"},
    {ELSE, "ELSE"},
    {IDENT, "IDENT"},
    {END, "END"},
    {IDENT, "IDENT"},

    {INTEGER, "INTEGER"},
    {REAL, "REAL"},
    {CHARACTER, "CHARACTER"},
    {THEN, "THEN"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {POW, "POW"},
    {ASSOP, "ASSOP"},
    {EQ, "EQ"},
    {GTHAN, "GTHAN"},
    {LTHAN, "LTHAN"},
    {COMMA, "COMMA"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},

    {DOT, "DOT"},
    {DCOLON, "DCOLON"},
    {CAT, "CAT"},
    {DEF, "DEF"},
};

ostream& operator<<(ostream& out, const LexItem& token){
    Token tk = token.GetToken();
    if (tk == IDENT)
    {
        out << "IDENT: '" << token.GetLexeme() << "'" << endl;
    } 
    else if (TokenString.find(tk) != TokenString.end())
    {
        out << TokenString[tk] << endl;
    }
    else if (tk == SCONST)
    {
        out << "SCONST: " << token.GetLexeme() << endl;
    }
    else if (tk == ICONST)
    {
        out << "ICONST: (" << token.GetLexeme() << ")" << endl; 
    }
    else if (tk == RCONST)
    {
        out << "RCONST: (" << token.GetLexeme() << ")" << endl;;
    }
    else if (tk == ERR)
    {
        out << "Error in line " << token.GetLinenum() << ": Unrecognized Lexeme {" << token.GetLexeme() << "}" << endl;
    }
    return out;
}

map<string, Token> KeytoT = 
{
    {"PROGRAM", PROGRAM },
    {"PRINT", PRINT},
    {"IF",  IF},
    {"ELSE" ,  ELSE},	
    { "IDENT", IDENT },

    { "END", END },
    { "INTEGER", INTEGER },
    { "REAL", REAL },
    { "CHARACTER", CHARACTER },
    { "THEN", THEN },
};


LexItem id_or_kw(const string& lexeme, int linenum)
{
    string lexe = "";
    for(int i = 0; i < lexeme.length(); i++)
    {
        lexe += toupper(lexeme[i]);
    }
    if(KeytoT.find(lexe) == KeytoT.end())
    {
        return LexItem(IDENT, lexeme, linenum);
    } 
    else 
    {
        return LexItem(KeytoT[lexe], lexeme, linenum);
    }
}
