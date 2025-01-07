#include <generator/context.hpp>
#include <iostream>
using namespace std;
yy::parser::symbol_type yylex(Context &ctx)
{
    map<string, yy::parser::symbol_type> literalMap = {
        {"{", yy::parser::make_LCURL("{")},
        {"}", yy::parser::make_RCURL("}")},
        {"|", yy::parser::make_BAR("|")},
        {"[", yy::parser::make_LBRACKET("[")},
        {"]", yy::parser::make_RBRACKET("]")},
        {":", yy::parser::make_COLON(":")},
    };
    if (ctx.done())
    {
        return yy::parser::make_YYEOF();
    }

    if (ctx.current().type == "LITERAL")
    {

        if (literalMap.find(ctx.current().value) == literalMap.end())
        {
            auto ret = yy::parser::make_LITERAL(ctx.current().value);
            ctx.next();
            return ret;
        }
        // reserved literal
        auto ret = literalMap[ctx.current().value];
        ctx.next();
        return ret;
    }
    else if (ctx.current().type == "TERMINAL")
    {
        auto ret = yy::parser::make_TERMINAL(ctx.current().value);
        ctx.next();
        return ret;
    }
    else if (ctx.current().type == "NONTERMINAL")
    {
        auto ret = yy::parser::make_NONTERMINAL(ctx.current().value);
        ctx.next();
        return ret;
    }
    else
    {
        // newline
        auto ret = yy::parser::make_NEWLINE("\n");
        ctx.next();
        return ret;
    }
}
Context::Context(std::vector<Token> tokens) : tokens(tokens), pos(0) {}
Token Context::current() const { return tokens[pos]; }
void Context::next() { ++pos; }
bool Context::done() const { return pos >= tokens.size(); }
Grammar Context::getGrammar() const { return grammar; }
void Context::setGrammar(Grammar grammar) { this->grammar = grammar; }