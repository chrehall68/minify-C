#include <generator/ast.hpp>
#include <generator/parser.hpp>
#include <iostream>
#include <argp.h>
#include <lang/clexer.hpp>
#include <fstream>
#include "cgrammar.hpp"
using namespace std;

string toSymbol(int i)
{
    vector<char> result;
    while (i >= 0)
    {
        int tmp = i % 52; // use every lowercase and uppercase letter
        if (tmp < 26)
        {
            result.push_back('a' + tmp);
        }
        else
        {
            result.push_back('A' + tmp - 26);
        }
        i /= 52;
        i -= 1;
    }
    reverse(result.begin(), result.end());
    return string(result.begin(), result.end());
}

/**
 * @brief Replace variables with a dfs. Variables will be replaced by assuming
 * that variables should be visible to siblings and descendants, but not
 * ancestors of the current Node
 *
 * @param cur the current node
 * @param curMaxSymbol the current max symbol, exclusive
 * @param symbolMap the symbol map
 * @return pair<int, int>  a tuple containing (curMaxSymbol, maxSymbol)
 */
pair<int, int> dfs(ASTNode *cur, int curMaxSymbol, map<string, string> &symbolMap)
{
    // if compound statement, then the compound statement's siblings shouldn't be able to access
    // anything from inside the compound statement
    // for loops also have a special case since the variable declared as part 1 of the for loop
    // should only be accessible inside the for loop and not outside

    // everything else follows the below rule:
    // any variable declared inside it will be accessible to its siblings and descendants
    // and passed up to ancestors
    int maxSymbol = curMaxSymbol;
    if (cur->rule == "direct_declarator")
    {
        // adds variables to the symbol map
        for (shared_ptr<ASTPart> part : cur->parts)
        {
            for (shared_ptr<ASTLeaf> leaf : part->leaves)
            {
                Token *token = dynamic_cast<Token *>(leaf.get());
                if (token)
                {
                    if (token->type == "IDENTIFIER")
                    {
                        if (symbolMap.find(token->value) == symbolMap.end())
                        {
                            if (token->value == "main")
                            {
                                // special case for main
                                symbolMap[token->value] = "main";
                            }
                            else
                            {
                                symbolMap[token->value] = toSymbol(curMaxSymbol);
                                token->value = toSymbol(curMaxSymbol);
                                curMaxSymbol += 1;
                                maxSymbol = max(maxSymbol, curMaxSymbol);
                            }
                        }
                        else
                        {
                            // shouldn't happen?
                            throw runtime_error("Variable " + token->value + " already defined");
                        }
                    }
                }
                else if (dynamic_cast<ASTNode *>(leaf.get()))
                {
                    // recurse further
                    pair<int, int> result = dfs(dynamic_cast<ASTNode *>(leaf.get()), curMaxSymbol, symbolMap);
                    curMaxSymbol = result.first;
                    maxSymbol = max(maxSymbol, result.second);
                }
                else
                {
                    // shouldn't happen?
                    cout << "Unknown leaf type" << endl;
                    throw runtime_error("Unknown leaf type");
                }
            }
        }
        return {curMaxSymbol, maxSymbol};
    }
    // else, replace variables
    bool special = cur->rule == "compound_statement" || cur->rule == "iteration_statement";
    int originalCurMaxSymbol = curMaxSymbol;
    map<string, string> originalSymbolMap;
    if (special)
    {
        // copy operation...
        originalSymbolMap = symbolMap;
    }

    // main dfs part
    for (shared_ptr<ASTPart> part : cur->parts)
    {
        for (shared_ptr<ASTLeaf> leaf : part->leaves)
        {
            Token *token = dynamic_cast<Token *>(leaf.get());
            if (token)
            {
                if (token->type == "IDENTIFIER")
                {
                    if (symbolMap.find(token->value) == symbolMap.end())
                    {
                        // shouldn't happen?
                        cerr << "Variable " << token->value << " not defined" << endl;
                    }
                    else
                    {
                        token->value = symbolMap[token->value];
                    }
                }
            }
            else
            {
                // recurse further
                pair<int, int> result = dfs(dynamic_cast<ASTNode *>(leaf.get()), curMaxSymbol, symbolMap);
                curMaxSymbol = result.first;
                maxSymbol = result.second;
            }
        }
    }
    if (special)
    {
        // copy operation...
        symbolMap = originalSymbolMap;
        curMaxSymbol = originalCurMaxSymbol;
    }
    return {curMaxSymbol, maxSymbol};
}

void minify(ASTNode *node)
{
    map<string, string> symbolMap;
    dfs(node, 0, symbolMap);
}
std::vector<Token> extractTokens(ASTNode *node)
{
    std::vector<Token> tokens;
    for (shared_ptr<ASTPart> part : node->parts)
    {
        for (shared_ptr<ASTLeaf> leaf : part->leaves)
        {
            Token *token = dynamic_cast<Token *>(leaf.get());
            if (token)
            {
                tokens.push_back(*token);
            }
            else
            {
                for (Token &token : extractTokens(dynamic_cast<ASTNode *>(leaf.get())))
                {
                    tokens.push_back(token);
                }
            }
        }
    }
    return tokens;
}
std::string minifyTokens(std::vector<Token> &tokens)
{
    vector<string> result;
    for (int i = 0; i < tokens.size(); i++)
    {
        // preprocessors are a special case
        if (tokens[i].type == "PREPROCESSOR")
        {
            result.push_back(tokens[i].value);
            result.push_back("\n");
        }
        // otherwise, put spaces between everything unless it's a punctuator
        else if (tokens[i].type == "PUNCTUATOR")
        {
            if (result.size() > 0 && result.back() == " ")
            {
                result.pop_back();
            }
            result.push_back(tokens[i].value);
        }
        else
        {
            result.push_back(tokens[i].value);
            result.push_back(" ");
        }
    }
    // trim and output
    if (!result.empty() && (result.back() == " " || result.back() == "\n"))
    {
        result.pop_back();
    }
    stringstream buffer;
    for (string &s : result)
    {
        buffer << s;
    }
    return buffer.str();
}

const char *argp_program_version =
    "minify 1.0";
const char *argp_program_bug_address =
    "chrehall68@gmail.com";

/* Program documentation. */
static char doc[] =
    "CMinifier - Minify a C file.";

/* A description of the arguments we accept. */
static char args_doc[] = "FILE";

/* The options we understand. */
static struct argp_option options[] = {
    {"output", 'o', "OUTPUTFILE", 0, "Output file name (default is out.c)", 0},
    {0}};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *ifname; /* FILE */
    char *ofname;
};

/* Parse a single option. */
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = reinterpret_cast<struct arguments *>(state->input);

    switch (key)
    {
    case 'o':
        arguments->ofname = arg;
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            /* Too many arguments. */
            argp_usage(state);

        arguments->ifname = arg;

        break;

    case ARGP_KEY_END:
        if (state->arg_num < 1)
            /* Not enough arguments. */
            argp_usage(state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = {.options = options, .parser = parse_opt, .args_doc = args_doc, .doc = doc};

int main(int argc, char **argv)
{
    struct arguments arguments;

    /* Default values. */
    arguments.ifname = NULL;
    arguments.ofname = "out.c";

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    Lexer lexer = makeCLexer();

    // read contents
    ifstream file(arguments.ifname);
    stringstream buffer;
    buffer << file.rdbuf();
    string contents = buffer.str();

    // lex
    vector<Token> tokens = lexer.lex(contents);

    // parse
    Parser p(makeGrammar());
    ParseResult<ASTNode> result = p.parse("translation_unit", tokens);
    if (!result)
    {
        return 1;
    }
    cout << "Parsed successfully" << endl;

    // minify
    shared_ptr<ASTNode> node = *result.val;
    cout << node->print() << endl;
    minify(node.get());
    vector<Token> minifiedTokens = extractTokens(node.get());
    string minifiedString = minifyTokens(minifiedTokens);

    // write
    ofstream out(arguments.ofname);
    out << minifiedString;
    out.close();

    return 0;
}