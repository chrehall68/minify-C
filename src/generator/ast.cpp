#include <generator/ast.hpp>
#include <sstream>
using namespace std;
Token::Token(string type, string value) : type(type), value(value) {}
ASTPart::ASTPart(vector<shared_ptr<ASTLeaf>> leaves) : leaves(leaves) {}
ASTNode::ASTNode(string rule, vector<shared_ptr<ASTPart>> parts) : rule(rule), parts(parts) {}

ostream &operator<<(ostream &o, vector<shared_ptr<ASTLeaf>> &v)
{
    o << "{";
    for (int i = 1; i < v.size(); ++i)
    {
        o << v[i - 1]->print() << ", ";
    }
    if (v.size() > 0)
    {
        o << v[v.size() - 1]->print();
    }
    return o << "}";
}
ostream &operator<<(ostream &o, vector<shared_ptr<ASTPart>> &v)
{
    o << "{";
    for (int i = 1; i < v.size(); ++i)
    {
        o << v[i - 1]->print() << ", ";
    }
    if (v.size() > 0)
    {
        o << v[v.size() - 1]->print();
    }
    return o << "}";
}
string Token::print()
{
    stringstream s;
    s << "Token(\"" << type << "\", \"" << value << "\")";
    return s.str();
}

string ASTPart::print()
{
    stringstream s;
    s << "ASTPart(" << leaves << ")";
    return s.str();
}

string ASTNode::print()
{
    stringstream s;
    s << "ASTNode(\"" << rule << "\", " << parts << ")";
    return s.str();
}