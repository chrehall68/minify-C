#include <iostream>
#include <fstream>
#include <generator/lexer.hpp>
#include <generator/parser.hpp>
#include <generator/context.hpp>
#include <argp.h>
#include <sstream>
#include <cstring>
using namespace std;

const char *argp_program_version =
    "parsegen 1.0";
const char *argp_program_bug_address =
    "chrehall68@gmail.com";

/* Program documentation. */
static char doc[] =
    "Parsegen - Generate a Grammar object from a grammar in BNF form.";

/* A description of the arguments we accept. */
static char args_doc[] = "FILE";

/* The options we understand. */
static struct argp_option options[] = {
    {"output", 'o', "OUTPUTFILE", 0, "Output file name (must end with .hpp) (default is a.hpp)", 0},
    {0}};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *ifname; /* FILE */
    char *ofname;
};
ostream &operator<<(ostream &o, Token &t)
{
    if (t.value == "\n")
    {
        return o << "Token(\"" << t.type << "\", \"\\n\")";
    }
    else
    {
        return o << "Token(\"" << t.type << "\", \"" << t.value << "\")";
    }
}

template <typename T>
ostream &operator<<(ostream &o, vector<T> &v)
{
    o << "{";
    for (int i = 1; i < v.size(); ++i)
    {
        o << v[i - 1] << ", ";
    }
    if (v.size() > 0)
    {
        o << v[v.size() - 1];
    }
    return o << "}";
}
ostream &operator<<(ostream &o, Rule &r)
{
    return o << "Rule(\"" << r.name << "\", " << r.productions << ")";
}
ostream &operator<<(ostream &o, Production &p)
{
    return o << "Production(" << p.parts << ")";
}
ostream &operator<<(ostream &o, ProductionPart &p)
{
    return o << "ProductionPart(" << p.optional << ", " << p.star << ", " << p.items << ")";
}
ostream &operator<<(ostream &o, Leaf &l)
{
    return o << "Leaf(\"" << l.type << "\", \"" << l.value << "\")";
}

ostream &operator<<(ostream &o, Grammar g)
{
    return o << "Grammar(" << g.rules << ")";
}

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
    arguments.ofname = "a.hpp";

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // read contents
    ifstream file(arguments.ifname);
    stringstream buffer;
    buffer << file.rdbuf();
    string contents = buffer.str();

    // make lexer
    vector<string> names = {"TERMINAL", "LITERAL", "NONTERMINAL", "NEWLINE"};
    vector<string> regexes = {"[A-Z_]+", "('[^']+')|([:|\\[\\]\\{\\}])", "[a-z_]+", "\n"};
    Lexer lexer(names, regexes, " \t");

    // lex
    vector<Token> tokens = lexer.lex(contents);

    // parse
    Context ctx(tokens);
    yy::parser parser(ctx);
    parser.parse();

    ofstream output(arguments.ofname);
    output << "#pragma once" << endl;
    output << "#include <generator/types.hpp>" << endl;
    output << "Grammar makeGrammar() { return " << ctx.getGrammar() << "; }" << endl;
    output.close();
    return 0;
}