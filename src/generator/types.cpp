#include <generator/types.hpp>

Leaf::Leaf(std::string type, std::string value) : type(type), value(value) {}
ProductionPart::ProductionPart(bool optional, bool star, std::vector<Leaf> items) : optional(optional), star(star), items(items) {}
Production::Production(std::vector<ProductionPart> parts) : parts(parts) {}
Rule::Rule(std::string name, std::vector<Production> productions) : name(name), productions(productions) {}
Grammar::Grammar(std::vector<Rule> rules) : rules(rules) {}