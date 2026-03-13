/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    int line = 1;
    int col = 1;
    int token_line = 1;
    int token_col = 1;
    int comment_line = 1;
    int comment_col = 1;
    int str_line = 1;
    int str_col = 1;
    bool str_invalid_escape = false;
    char str_buf[65536];
    int str_idx = 0;
    #define YY_USER_ACTION token_line = line; token_col = col; col += yyleng;
    bool opt_print_tokens = false;
%}
IDENTIFIER [a-zA-Z_$]
NATURAL [0-9]
NUMERO {NATURAL}([_0-9]*{NATURAL})?
EXPO   [eE][+-]?{NUMERO}


%X COMMENT STRING LINE_COMMENT
%%
"++"|"--"|"null"|"Integer"|"System"|"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"|"_" { if (opt_print_tokens) printf("RESERVED(%s)\n", yytext); }
"boolean"               { if (opt_print_tokens) printf("BOOL\n"); }
"class"                 { if (opt_print_tokens) printf("CLASS\n"); }
".length"               { if (opt_print_tokens) printf("DOTLENGTH\n"); }
"double"                { if (opt_print_tokens) printf("DOUBLE\n"); }
"else"                  { if (opt_print_tokens) printf("ELSE\n"); }
"if"                    { if (opt_print_tokens) printf("IF\n"); }
"int"                   { if (opt_print_tokens) printf("INT\n"); }
"System.out.print"      { if (opt_print_tokens) printf("PRINT\n"); }
"Integer.parseInt"      { if (opt_print_tokens) printf("PARSEINT\n"); }
"public"                { if (opt_print_tokens) printf("PUBLIC\n"); }
"return"                { if (opt_print_tokens) printf("RETURN\n"); }
"static"                { if (opt_print_tokens) printf("STATIC\n"); }
"String"                { if (opt_print_tokens) printf("STRING\n"); }
"void"                  { if (opt_print_tokens) printf("VOID\n"); }
"while"                 { if (opt_print_tokens) printf("WHILE\n"); }
"true"|"false"          { if (opt_print_tokens) printf("BOOLLIT(%s)\n", yytext); }
"&&"                    { if (opt_print_tokens) printf("AND\n"); }
"="                     { if (opt_print_tokens) printf("ASSIGN\n"); }
"*"                     { if (opt_print_tokens) printf("STAR\n"); }
","                     { if (opt_print_tokens) printf("COMMA\n"); }
"/"                     { if (opt_print_tokens) printf("DIV\n"); }
"=="                    { if (opt_print_tokens) printf("EQ\n"); }
">="                    { if (opt_print_tokens) printf("GE\n"); }
">"                     { if (opt_print_tokens) printf("GT\n"); }
"{"                     { if (opt_print_tokens) printf("LBRACE\n"); }
"<="                    { if (opt_print_tokens) printf("LE\n"); }
"("                     { if (opt_print_tokens) printf("LPAR\n"); }
"["                     { if (opt_print_tokens) printf("LSQ\n"); }
"<"                     { if (opt_print_tokens) printf("LT\n"); }
"-"                     { if (opt_print_tokens) printf("MINUS\n"); }
"%"                     { if (opt_print_tokens) printf("MOD\n"); }
"!="                    { if (opt_print_tokens) printf("NE\n"); }
"!"                     { if (opt_print_tokens) printf("NOT\n"); }
"||"                    { if (opt_print_tokens) printf("OR\n"); }
"+"                     { if (opt_print_tokens) printf("PLUS\n"); }
"}"                     { if (opt_print_tokens) printf("RBRACE\n"); }
")"                     { if (opt_print_tokens) printf("RPAR\n"); }
"]"                     { if (opt_print_tokens) printf("RSQ\n"); }
";"                     { if (opt_print_tokens) printf("SEMICOLON\n"); }
"->"                    { if (opt_print_tokens) printf("ARROW\n"); }
"<<"                    { if (opt_print_tokens) printf("LSHIFT\n"); }
">>"                    { if (opt_print_tokens) printf("RSHIFT\n"); }
"^"                     { if (opt_print_tokens) printf("XOR\n"); }

{IDENTIFIER}({IDENTIFIER}|{NATURAL})* { if (opt_print_tokens) printf("IDENTIFIER(%s)\n", yytext);}
0|([1-9]([_0-9]*[0-9])?)                                      { if (opt_print_tokens) printf("NATURAL(%s)\n", yytext); }
({NUMERO}\.{NUMERO}?{EXPO}?)|(\.{NUMERO}{EXPO}?)|({NUMERO}{EXPO})  { if (opt_print_tokens) printf("DECIMAL(%s)\n", yytext); }

"//"                    { BEGIN(LINE_COMMENT); }
<LINE_COMMENT>\n|\r|\r\n { line++; col = 1; BEGIN(INITIAL); }
<LINE_COMMENT>.         { }

"/*"                    { BEGIN(COMMENT);comment_line = token_line; comment_col = token_col; }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>\n|\r|\r\n     { line++; col = 1; }
<COMMENT>.              {  }
<COMMENT><<EOF>>        { printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col); return 0; }

\"                      { BEGIN(STRING); str_idx = 0; str_invalid_escape = false; str_line = token_line; str_col = token_col;}
<STRING>\"              { BEGIN(INITIAL);if (!str_invalid_escape && opt_print_tokens) {str_buf[str_idx] = '\0';printf("STRLIT(\"%s\")\n", str_buf);}}
<STRING>\\[fnrt\\"]     { if(str_idx < 65534) {str_buf[str_idx++] = '\\';str_buf[str_idx++] = yytext[1];}}
<STRING>\\[^fnrt\\"\r\n] { printf("Line %d, col %d: invalid escape sequence (%s)\n", token_line, token_col, yytext);str_invalid_escape = true;}
<STRING>\\              { printf("Line %d, col %d: invalid escape sequence (\\)\n", token_line, token_col);str_invalid_escape = true;}
<STRING>\n|\r|\r\n      {printf("Line %d, col %d: unterminated string literal\n", str_line, str_col);line++; col = 1;BEGIN(INITIAL);}
<STRING><<EOF>>         { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); return 0; }
<STRING>.               { if(str_idx < 65535) {str_buf[str_idx++] = yytext[0];}}
[ \t\f]                 { }
\n|\r|\r\n              { line++; col = 1; }
.                       { printf("Line %d, col %d: illegal character (%s)\n", token_line, token_col, yytext); }



%%
extern int yylex();
int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "-l") == 0) {
        opt_print_tokens = true;
    }
    yylex();
    return 0;
}
int yywrap() {  
    return 1;
}
