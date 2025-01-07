#include <lang/clexer.hpp>
using namespace std;
Lexer makeCLexer()
{
    // keywords
    vector<string> keywords = {
        "auto",
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "goto",
        "if",
        "inline",
        "int",
        "long",
        "register",
        "restrict",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "volatile",
        "while",
        "_Bool",
        "_Complex",
        "_Imaginary",
    };
    // sort in order of length, but we want longest first
    sort(keywords.begin(), keywords.end(), [](const string &a, const string &b)
         { return a.size() > b.size(); });
    stringstream buffer;
    for (int i = 0; i < keywords.size(); i++)
    {
        buffer << "(" << keywords[i] << ")";
        if (i < keywords.size() - 1)
        {
            buffer << "|";
        }
    }
    string keywordsRegex = buffer.str();

    // identifiers
    string identifierRegex = "[a-zA-Z_][a-zA-Z0-9_]*";

    // constants...
    string integerConstant = "((0[xX][0-9a-fA-F]+)|([1-9][0-9]*)|([0-7]+))(u|l|U|L|(ll)|(LL))?";
    string decFloatingConstant = "(([0-9]+(\\.[0-9]*)?)|(\\.[0-9]+))([eE][\\+\\-]?[0-9]+)?";
    string hexFloatingConstant = "(0[xX](([0-9a-fA-F]+(\\.[0-9a-fA-F]*)?))|([0-9a-fA-F]*\\.[0-9a-fA-F]+))[pP][\\+\\-]?[0-9]+";
    string floatingConstant = "((" + hexFloatingConstant + ")|(" + decFloatingConstant + "))[flFL]?";

    string simpleEscapeSequence = "\\\\['\"\\?\\\\abfnrtv]";
    string octalEscapeSequence = "\\\\[0-7]{1,3}";
    string hexEscapeSequence = "\\\\x[0-9a-fA-F]+";
    string charset = "\\!#$%&\\(\\)\\*\\+,\\-\\.\\/0-9:;<=\\?\\@A-Z\\[\\]\\^_`a-z\\{\\}\\|\\~ ";
    string ccharset = "[" + charset + "\">]";
    string scharset = "[" + charset + "'>]";
    string hcharset = "[" + charset + "'\"]";
    string qcharset = "[" + charset + "'>]";
    string cchar = "(" + simpleEscapeSequence + ")|(" + octalEscapeSequence + ")|(" + hexEscapeSequence + ")|(" + ccharset + ")";
    string schar = "(" + simpleEscapeSequence + ")|(" + octalEscapeSequence + ")|(" + hexEscapeSequence + ")|(" + scharset + ")";
    string hchar = "(" + simpleEscapeSequence + ")|(" + octalEscapeSequence + ")|(" + hexEscapeSequence + ")|(" + hcharset + ")";
    string qchar = "(" + simpleEscapeSequence + ")|(" + octalEscapeSequence + ")|(" + hexEscapeSequence + ")|(" + qcharset + ")";
    string charConstant = "[L]?'(" + cchar + "+)'";
    string constantRegex = "(" + floatingConstant + ")|(" + integerConstant + ")|(" + charConstant + ")";
    string stringLiteralRegex = "[L]?\"(" + schar + ")*\"";
    string headerNameRegex = "(<(" + hchar + ")+>)|(\"(" + qchar + ")+\")";

        vector<string> punctuators = {
        "\\[",
        "\\]",
        "\\(",
        "\\)",
        "\\{",
        "\\}",
        "\\.",
        "\\->",
        "\\+\\+",
        "\\-\\-",
        "&",
        "\\*",
        "\\+",
        "\\-",
        "\\~",
        "\\!",
        "/",
        "%",
        "<<",
        ">>",
        "<",
        ">",
        "<=",
        ">=",
        "==",
        "\\!=",
        "\\^",
        "\\|",
        "&&",
        "\\|\\|",
        "\\?",
        ":",
        ";",
        "\\.\\.\\.",
        "=",
        "\\*=",
        "/=",
        "%=",
        "\\+=",
        "\\-=",
        "<<=",
        ">>=",
        "&=",
        "\\^=",
        "\\|=",
        ",",
        "\\#",
        "\\#\\#",
        "<:",
        ":>",
        "<%",
        "%>",
        "%:",
        "%:%:",
    };
    sort(punctuators.begin(), punctuators.end(), [](const string &a, const string &b)
         { return a.size() > b.size(); });
    buffer.clear();
    for (int i = 0; i < punctuators.size(); i++)
    {
        buffer << "(" << punctuators[i] << ")";
        if (i < punctuators.size() - 1)
        {
            buffer << "|";
        }
    }
    string punctuatorRegex = buffer.str();

    // comments and preprocessors
    string commentRegex = "(/\\*(.*?\n?)*?\\*/)|(//.*)";
    string preprocessorRegex = "#[^\\n]*";

    vector<string> names = {"COMMENT", "KEYWORD", "IDENTIFIER", "CONSTANT", "STRINGLITERAL", "PUNCTUATOR", "PREPROCESSOR"};
    vector<string> regexes = {commentRegex, keywordsRegex, identifierRegex, constantRegex, stringLiteralRegex, punctuatorRegex, preprocessorRegex};
    return Lexer(names, regexes, " \t\n");
}