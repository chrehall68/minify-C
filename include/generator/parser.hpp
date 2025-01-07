#pragma once
#include <generator/ast.hpp>
#include <generator/types.hpp>
#include <map>
#include <string>
#include <memory>
#include <optional>
template <typename T>
struct ParseResult
{
    std::optional<std::shared_ptr<T>> val;
    int pos;
    bool valid;

    ParseResult() : val(std::nullopt), pos(-1), valid(false) {}
    ParseResult(std::shared_ptr<T> val, int pos) : val(val), pos(pos), valid(true) {}
    ParseResult(bool valid) : val(std::nullopt), pos(-1), valid(valid) {}

    operator bool() { return valid; }
};
class Parser
{
private:
    int maxPos;
    std::map<std::pair<std::string, int>, ParseResult<ASTNode>> cache;
    std::map<std::string, Rule> ruleMap;

    ParseResult<ASTLeaf> takeLeaf(const Leaf l, const std::vector<Token> &tokens, int pos);
    ParseResult<ASTPart> takeLeaves(const std::vector<Leaf> &l, const std::vector<Token> &tokens, int pos);
    ParseResult<ASTNode> checkProduction(const std::string rule, const Production &prod, const std::vector<Token> &tokens, int pos);
    ParseResult<ASTNode> checkRule(const std::string rule, const std::vector<Token> &tokens, int pos);

public:
    Parser(Grammar grammar);
    ParseResult<ASTNode> parse(std::string startSymbol, const std::vector<Token> &tokens);
};
