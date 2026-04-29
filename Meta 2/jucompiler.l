/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{
#include "y.tab.h"
#include <string.h>
int line = 1, col = 1;
extern int flag_lex;

void print_token(const char* name, char* val) {
    if (flag_lex) {
        if (val) printf("%s(%s)\n", name, val);
        else printf("%s\n", name);
    }
}
%}

%option nounput noinput

ID [a-zA-Z_$][a-zA-Z0-9_$]*
NATURAL 0|[1-9](_?[0-9])*
DECIMAL ([0-9](_?[0-9])*\.[0-9](_?[0-9])*([eE][+-]?[0-9](_?[0-9])*)?)|(\.[0-9](_?[0-9])*([eE][+-]?[0-9](_?[0-9])*)?)|([0-9](_?[0-9])*([eE][+-]?[0-9](_?[0-9])*)?)
STRLIT \"([^\"\r\n\\]|\\[fnrt\"\\])*\"

%%

"boolean"           { print_token("BOOL", NULL); col += yyleng; return BOOL; }
"class"             { print_token("CLASS", NULL); col += yyleng; return CLASS; }
".length"           { print_token("DOTLENGTH", NULL); col += yyleng; return DOTLENGTH; }
"double"            { print_token("DOUBLE", NULL); col += yyleng; return DOUBLE; }
"else"              { print_token("ELSE", NULL); col += yyleng; return ELSE; }
"if"                { print_token("IF", NULL); col += yyleng; return IF; }
"int"               { print_token("INT", NULL); col += yyleng; return INT; }
"System.out.print"  { print_token("PRINT", NULL); col += yyleng; return PRINT; }
"Integer.parseInt"  { print_token("PARSEINT", NULL); col += yyleng; return PARSEINT; }
"public"            { print_token("PUBLIC", NULL); col += yyleng; return PUBLIC; }
"return"            { print_token("RETURN", NULL); col += yyleng; return RETURN; }
"static"            { print_token("STATIC", NULL); col += yyleng; return STATIC; }
"String"            { print_token("STRING", NULL); col += yyleng; return STRING; }
"void"              { print_token("VOID", NULL); col += yyleng; return VOID; }
"while"             { print_token("WHILE", NULL); col += yyleng; return WHILE; }
"true"|"false"      { print_token("BOOLLIT", yytext); col += yyleng; yylval.str = strdup(yytext); return BOOLLIT; }
"&&"                { print_token("AND", NULL); col += yyleng; return AND; }
"="                 { print_token("ASSIGN", NULL); col += yyleng; return ASSIGN; }
"*"                 { print_token("STAR", NULL); col += yyleng; return STAR; }
","                 { print_token("COMMA", NULL); col += yyleng; return COMMA; }
"/"                 { print_token("DIV", NULL); col += yyleng; return DIV; }
"=="                { print_token("EQ", NULL); col += yyleng; return EQ; }
">="                { print_token("GE", NULL); col += yyleng; return GE; }
">"                 { print_token("GT", NULL); col += yyleng; return GT; }
"{"                 { print_token("LBRACE", NULL); col += yyleng; return LBRACE; }
"<="                { print_token("LE", NULL); col += yyleng; return LE; }
"("                 { print_token("LPAR", NULL); col += yyleng; return LPAR; }
"["                 { print_token("LSQ", NULL); col += yyleng; return LSQ; }
"<"                 { print_token("LT", NULL); col += yyleng; return LT; }
"-"                 { print_token("MINUS", NULL); col += yyleng; return MINUS; }
"%"                 { print_token("MOD", NULL); col += yyleng; return MOD; }
"!="                { print_token("NE", NULL); col += yyleng; return NE; }
"!"                 { print_token("NOT", NULL); col += yyleng; return NOT; }
"||"                { print_token("OR", NULL); col += yyleng; return OR; }
"+"                 { print_token("PLUS", NULL); col += yyleng; return PLUS; }
"}"                 { print_token("RBRACE", NULL); col += yyleng; return RBRACE; }
")"                 { print_token("RPAR", NULL); col += yyleng; return RPAR; }
"]"                 { print_token("RSQ", NULL); col += yyleng; return RSQ; }
";"                 { print_token("SEMICOLON", NULL); col += yyleng; return SEMICOLON; }
"->"                { print_token("ARROW", NULL); col += yyleng; return ARROW; }
"<<"                { print_token("LSHIFT", NULL); col += yyleng; return LSHIFT; }
">>"                { print_token("RSHIFT", NULL); col += yyleng; return RSHIFT; }
"^"                 { print_token("XOR", NULL); col += yyleng; return XOR; }
{ID}                { print_token("IDENTIFIER", yytext); col += yyleng; yylval.str = strdup(yytext); return IDENTIFIER; }
{NATURAL}           { print_token("NATURAL", yytext); col += yyleng; yylval.str = strdup(yytext); return NATURAL; }
{DECIMAL}           { print_token("DECIMAL", yytext); col += yyleng; yylval.str = strdup(yytext); return DECIMAL; }
{STRLIT}            { print_token("STRLIT", yytext); col += yyleng; yylval.str = strdup(yytext); return STRLIT; }
"/*"([^*]|\*+[^*/])*\*+"/" { /* Comentários multi-linha: contar \n para linha e atualizar col */ }
"//".* { /* Comentário linha: não faz nada */ }
[ \t\r]+            { col += yyleng; }
\n                  { line++; col = 1; }

.                   { printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext); col += yyleng; }

%%
int yywrap() { return 1; }