#pragma once
#include <string>
#include <vector>
struct Leaf
{
    std::string type;
    std::string value;
    Leaf(std::string type, std::string value);
    Leaf() = default;
};
struct ProductionPart
{
    bool optional;
    bool star;
    std::vector<Leaf> items;
    ProductionPart(bool optional, bool star, std::vector<Leaf> items);
    ProductionPart() = default;
};
struct Production
{
    std::vector<ProductionPart> parts;
    Production(std::vector<ProductionPart> parts);
    Production() = default;
};
struct Rule
{
    std::string name;
    std::vector<Production> productions;
    Rule(std::string name, std::vector<Production> productions);
    Rule() = default;
    Rule(const Rule &other) = default;
};
struct Grammar
{
    std::vector<Rule> rules;
    Grammar(std::vector<Rule> rules);
    Grammar() = default;
};