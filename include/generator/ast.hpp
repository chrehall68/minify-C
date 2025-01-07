#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
struct ASTLeaf
{
    virtual ~ASTLeaf() {}
    virtual std::string print() = 0;
};

struct Token : public ASTLeaf
{
    std::string type;
    std::string value;
    Token(std::string type, std::string value);
    Token() = default;
    virtual std::string print();
};
struct ASTPart
{
    std::vector<std::shared_ptr<ASTLeaf>> leaves;
    ASTPart(std::vector<std::shared_ptr<ASTLeaf>> leaves);
    ASTPart() = default;
    std::string print();
};

struct ASTNode : ASTLeaf
{
    std::vector<std::shared_ptr<ASTPart>> parts;
    std::string rule;
    ASTNode(std::string rule, std::vector<std::shared_ptr<ASTPart>> parts);
    ASTNode() = default;
    virtual std::string print();
};
