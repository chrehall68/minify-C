#pragma once
#include "pg.hpp"
#include <generator/lexer.hpp>
#include <vector>
#include <generator/types.hpp>

class Context
{
private:
    std::vector<Token> tokens;
    int pos;
    Grammar grammar;

public:
    Context(std::vector<Token> tokens);
    Token current() const;
    void next();
    bool done() const;
    Grammar getGrammar() const;
    void setGrammar(Grammar grammar);
};

yy::parser::symbol_type yylex(Context &ctx);