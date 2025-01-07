#include <generator/lexer.hpp>
using namespace std;
using namespace boost;
Lexer::Lexer(vector<string> names, vector<string> regexes, string ignore)
{
    if (names.size() != regexes.size())
    {
        throw runtime_error("Lexer: names and regexes must be the same length");
    }
    this->names = names;
    for (int i = 0; i < regexes.size(); i++)
    {
        this->regexes.push_back(regex("^(" + regexes[i] + ")"));
    }
    for (int i = 0; i < ignore.size(); i++)
    {
        this->ignore.insert(ignore[i]);
    }
}

vector<Token> Lexer::lex(const string &contents) const
{
    vector<Token> tokens;
    int pos = 0;
    int lineno = 0;
    int linestart = 0;
    while (pos != contents.size())
    {
        // first, check if this is an ignorable token
        if (ignore.find(contents[pos]) != ignore.end())
        {
            if (contents[pos] == '\n')
            {
                lineno += 1;
                linestart = pos + 1;
            }
            pos += 1;
            continue;
        }

        // take token with largest munch
        // and in cases of ties, use
        // the one that came first
        int bestToken = -1;
        int bestLength = 0;
        string bestVal = "";
        for (int i = 0; i < names.size(); i++)
        {
            smatch m;
            if (regex_search(contents.begin() + pos, contents.end(), m, regexes[i]))
            {
                if (m.length() > bestLength)
                {
                    bestLength = m.length();
                    bestToken = i;
                    bestVal = m.str();
                }
            }
        }
        if (bestToken == -1)
        {
            throw runtime_error("Lexing failed at line " + to_string(lineno) + " col " + to_string(pos - linestart));
        }
        tokens.push_back(Token(names[bestToken], bestVal));

        // update lineno and linestart
        for (int i = 0; i < bestVal.size(); ++i)
        {
            if (bestVal[i] == '\n')
            {
                ++lineno;
                linestart = pos + i + 1;
            }
        }
        pos += bestVal.size();
    }
    return tokens;
}