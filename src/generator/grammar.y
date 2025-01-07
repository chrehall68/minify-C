%require "3.2"
%language "c++"
%{
#include <iostream>
#include <generator/types.hpp>
#include <vector>
#include <generator/context.hpp>

%}
%code requires {
class Context;
  #include <generator/types.hpp>

}
%param { Context& ctx }
%define parse.trace
%define parse.error detailed
%define parse.lac full

%define api.value.type variant
%define api.token.constructor

%start grammar
/* declare types */
%nterm <Grammar> grammar
%nterm <Rule> rule
%nterm <Production> production
%nterm <ProductionPart> production_part
%nterm <std::vector<Leaf>> production_item_list
%nterm <Leaf> production_leaf

/* declare tokens */
%token <std::string> LITERAL
%token <std::string> TERMINAL
%token <std::string> NONTERMINAL
%token <std::string> NEWLINE
%token <std::string> COLON
%token <std::string> BAR
%token <std::string> LCURL
%token <std::string> RCURL
%token <std::string> LBRACKET
%token <std::string> RBRACKET

%%
grammar : rule         { $$ = Grammar(std::vector<Rule>({$1})); ctx.setGrammar($$); }  /* contains one rule */
  | NEWLINE            { $$ = Grammar(); ctx.setGrammar($$); } /* newline is empty grammar */
  | /* empty */        { }
  | rule grammar       { $$ = $2; $$.rules.push_back($1); ctx.setGrammar($$); }
  | NEWLINE grammar    { $$ = $2; ctx.setGrammar($$); }
  ;
rule : NONTERMINAL COLON production NEWLINE { $$ = Rule($1, std::vector<Production>({$3})); }
  | rule BAR production NEWLINE           { $$ = $1; $$.productions.push_back($3); }
  ;
production : production_part             { $$ = Production(std::vector<ProductionPart>({$1})); }
  | production production_part           { $$ = $1; $$.parts.push_back($2); }
  ;
production_part : LBRACKET production_item_list RBRACKET  { $$ = ProductionPart(true, false, $2); }
  | LCURL production_item_list RCURL                { $$ = ProductionPart(false, true, $2); }
  | production_item_list                        { $$ = ProductionPart(false, false, $1); }
  ;
production_item_list : production_leaf          { $$ = std::vector<Leaf>({$1}); }
  | production_item_list production_leaf        { $$ = $1; $$.push_back($2); }
  ;
production_leaf : TERMINAL  { $$ = Leaf("TERMINAL", $1); }
  | NONTERMINAL             { $$ = Leaf("NONTERMINAL", $1); }
  | LITERAL                 { $$ = Leaf("LITERAL", $1.substr(1, $1.size()-2)); }
  ;
%%

namespace yy
{
  // Report an error to the user.
  auto parser::error (const std::string& msg) -> void
  {
    std::cerr << msg << '\n';
  }
}