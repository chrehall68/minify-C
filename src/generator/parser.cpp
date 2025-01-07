#include <generator/parser.hpp>
using namespace std;

Parser::Parser(Grammar grammar) : maxPos(0)
{
    for (const auto &rule : grammar.rules)
    {
        ruleMap.insert({rule.name, rule});
    }
}

ParseResult<ASTLeaf> Parser::takeLeaf(const Leaf l, const std::vector<Token> &tokens, int pos)
{
    if (pos >= tokens.size())
    {
        return ParseResult<ASTLeaf>(false);
    }
    if (l.type == "LITERAL")
    {
        // in the case of literals, l.value
        // contains the exact literal to match
        if (tokens[pos].value != l.value)
        {
            return ParseResult<ASTLeaf>(false);
        }
        return ParseResult<ASTLeaf>(make_shared<Token>(tokens[pos]), pos + 1);
    }
    else if (l.type == "TERMINAL")
    {
        // in the case of terminals, l.value
        // contains the name of the terminal to match
        if (tokens[pos].type != l.value)
        {
            return ParseResult<ASTLeaf>(false);
        }
        return ParseResult<ASTLeaf>(make_shared<Token>(tokens[pos]), pos + 1);
    }
    else
    {
        // in the case of nonterminals, l.value
        // contains the name of the nonterminal
        ParseResult<ASTNode> result = checkRule(l.value, tokens, pos);
        if (!result)
        {
            return ParseResult<ASTLeaf>(false);
        }
        return ParseResult<ASTLeaf>(*result.val, result.pos);
    }
}

ParseResult<ASTPart> Parser::takeLeaves(const std::vector<Leaf> &l, const std::vector<Token> &tokens, int pos)
{
    if (pos >= tokens.size())
    {
        return ParseResult<ASTPart>(false);
    }
    vector<shared_ptr<ASTLeaf>> result;
    for (const Leaf &leaf : l)
    {
        ParseResult<ASTLeaf> r = takeLeaf(leaf, tokens, pos);
        if (!r)
        {
            return ParseResult<ASTPart>(false);
        }
        pos = r.pos;
        result.push_back(*r.val);
    }
    return ParseResult<ASTPart>(make_shared<ASTPart>(result), pos);
}

ParseResult<ASTNode> Parser::checkProduction(const std::string rule, const Production &prod, const std::vector<Token> &tokens, int pos)
{
    vector<shared_ptr<ASTPart>> parts;
    for (const ProductionPart &part : prod.parts)
    {
        if (part.optional)
        {
            // try taking, but it's ok if it fails
            ParseResult<ASTPart> r = takeLeaves(part.items, tokens, pos);
            if (r)
            {
                // it actually succeeded, go with this
                parts.push_back(*r.val);
                pos = r.pos;
            }
        }
        else if (part.star)
        {
            // try taking any amount of this
            ParseResult<ASTPart> r = takeLeaves(part.items, tokens, pos);
            while (r)
            {
                parts.push_back(*r.val);
                pos = r.pos;
                r = takeLeaves(part.items, tokens, pos);
            }
        }
        else
        {
            // must take this item
            ParseResult<ASTPart> r = takeLeaves(part.items, tokens, pos);
            if (!r)
            {
                return ParseResult<ASTNode>(false);
            }
            parts.push_back(*r.val);
            pos = r.pos;
        }
    }
    return ParseResult<ASTNode>(make_shared<ASTNode>(rule, parts), pos);
}

ParseResult<ASTNode> Parser::checkRule(const std::string rule, const std::vector<Token> &tokens, int pos)
{
    if (cache.find({rule, pos}) != cache.end())
    {
        // it's already cached
        return cache[{rule, pos}];
    }

    // find the rule
    Rule r = ruleMap[rule];
    ParseResult<ASTNode> result(false);
    for (const Production &prod : r.productions)
    {
        ParseResult<ASTNode> r = checkProduction(rule, prod, tokens, pos);
        if (r && (!result.valid || result.pos < r.pos))
        {
            result = r;
        }
    }
    cache[{rule, pos}] = result;
    return result;
}

ParseResult<ASTNode> Parser::parse(std::string startSymbol, const std::vector<Token> &tokens)
{
    cache.clear();
    return checkRule(startSymbol, tokens, 0);
}