#pragma once
#include <string>
#include <set>
#include <vector>
#include <generator/ast.hpp>
#include <boost/regex.hpp>

class Lexer
{
private:
    std::set<char> ignore;
    std::vector<std::string> names;
    std::vector<boost::regex> regexes;

public:
    Lexer(std::vector<std::string> names, std::vector<std::string> regexes, std::string ignore = "");
    std::vector<Token> lex(const std::string &contents) const;
};