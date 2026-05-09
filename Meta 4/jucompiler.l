/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{
    #include "y.tab.h"
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
    extern int flag_lex; 
    #define YY_USER_ACTION token_line = line; token_col = col; col += yyleng;
%}
IDENTIFIER [a-zA-Z_$]
NATURAL [0-9]
NUMERO {NATURAL}([_0-9]*{NATURAL})?
EXPO   [eE][+-]?{NUMERO}
%option nounput noinput
%X COMMENT STR LINE_COMMENT
%%
"++"|"--"|"null"|"Integer"|"System"|"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"|"_" { if (flag_lex) printf("RESERVED(%s)\n", yytext); return RESERVED; }
"boolean"               { if (flag_lex) printf("BOOL\n"); return BOOL; }
"class"                 { if (flag_lex) printf("CLASS\n"); return CLASS; }
".length"               { if (flag_lex) printf("DOTLENGTH\n"); return DOTLENGTH; }
"double"                { if (flag_lex) printf("DOUBLE\n"); return DOUBLE; }
"else"                  { if (flag_lex) printf("ELSE\n"); return ELSE; }
"if"                    { if (flag_lex) printf("IF\n"); return IF; }
"int"                   { if (flag_lex) printf("INT\n"); return INT; }
"System.out.print"      { if (flag_lex) printf("PRINT\n"); return PRINT; }
"Integer.parseInt"      { if (flag_lex) printf("PARSEINT\n"); return PARSEINT; }
"public"                { if (flag_lex) printf("PUBLIC\n"); return PUBLIC; }
"return"                { if (flag_lex) printf("RETURN\n"); return RETURN; }
"static"                { if (flag_lex) printf("STATIC\n"); return STATIC; }
"String"                { if (flag_lex) printf("STRING\n"); return STRING; }
"void"                  { if (flag_lex) printf("VOID\n"); return VOID; }
"while"                 { if (flag_lex) printf("WHILE\n"); return WHILE; }
"true"|"false"          { if (flag_lex) printf("BOOLLIT(%s)\n", yytext); yylval.token.str = strdup(yytext); yylval.token.line = token_line; yylval.token.col = token_col; return BOOLLIT; }
"&&"                    { if (flag_lex) printf("AND\n"); return AND; }
"="                     { if (flag_lex) printf("ASSIGN\n"); return ASSIGN; }
"*"                     { if (flag_lex) printf("STAR\n"); return STAR; }
","                     { if (flag_lex) printf("COMMA\n"); return COMMA; }
"/"                     { if (flag_lex) printf("DIV\n"); return DIV; }
"=="                    { if (flag_lex) printf("EQ\n"); return EQ; }
">="                    { if (flag_lex) printf("GE\n"); return GE; }
">"                     { if (flag_lex) printf("GT\n"); return GT; }
"{"                     { if (flag_lex) printf("LBRACE\n"); return LBRACE; }
"<="                    { if (flag_lex) printf("LE\n"); return LE; }
"("                     { if (flag_lex) printf("LPAR\n"); return LPAR; }
"["                     { if (flag_lex) printf("LSQ\n"); return LSQ; }
"<"                     { if (flag_lex) printf("LT\n"); return LT; }
"-"                     { if (flag_lex) printf("MINUS\n"); return MINUS; }
"%"                     { if (flag_lex) printf("MOD\n"); return MOD; }
"!="                    { if (flag_lex) printf("NE\n"); return NE; }
"!"                     { if (flag_lex) printf("NOT\n"); return NOT; }
"||"                    { if (flag_lex) printf("OR\n"); return OR; }
"+"                     { if (flag_lex) printf("PLUS\n"); return PLUS; }
"}"                     { if (flag_lex) printf("RBRACE\n"); return RBRACE; }
")"                     { if (flag_lex) printf("RPAR\n"); return RPAR; }
"]"                     { if (flag_lex) printf("RSQ\n"); return RSQ; }
";"                     { if (flag_lex) printf("SEMICOLON\n"); return SEMICOLON; }
"->"                    { if (flag_lex) printf("ARROW\n"); return ARROW; }
"<<"                    { if (flag_lex) printf("LSHIFT\n"); return LSHIFT; }
">>"                    { if (flag_lex) printf("RSHIFT\n"); return RSHIFT; }
"^"                     { if (flag_lex) printf("XOR\n"); return XOR; }
{IDENTIFIER}({IDENTIFIER}|{NATURAL})* { 
    if (flag_lex) printf("IDENTIFIER(%s)\n", yytext);
    yylval.token.str = strdup(yytext); yylval.token.line = token_line; yylval.token.col = token_col; 
    return IDENTIFIER;
}
0|([1-9]([_0-9]*[0-9])?) { 
    if (flag_lex) printf("NATURAL(%s)\n", yytext); 
    yylval.token.str = strdup(yytext); yylval.token.line = token_line; yylval.token.col = token_col; 
    return NATURAL; 
}
({NUMERO}\.{NUMERO}?{EXPO}?)|(\.{NUMERO}{EXPO}?)|({NUMERO}{EXPO}) { 
    if (flag_lex) printf("DECIMAL(%s)\n", yytext); 
    yylval.token.str = strdup(yytext); yylval.token.line = token_line; yylval.token.col = token_col; 
    return DECIMAL; 
}
"//"                    { BEGIN(LINE_COMMENT); }
<LINE_COMMENT>\n|\r|\r\n { line++; col = 1; BEGIN(INITIAL); }
<LINE_COMMENT>.         { }
"/*"                    { BEGIN(COMMENT); comment_line = token_line; comment_col = token_col; }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>\n|\r|\r\n     { line++; col = 1; }
<COMMENT>.              { }
<COMMENT><<EOF>>        { printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col); return 0; }
\"                      { BEGIN(STR); str_idx = 0; str_invalid_escape = false; str_line = token_line; str_col = token_col;}
<STR>\" { 
    BEGIN(INITIAL);
    if (!str_invalid_escape) {
        str_buf[str_idx] = '\0';
        if (flag_lex) printf("STRLIT(\"%s\")\n", str_buf);
        char full_str[65540]; 
        sprintf(full_str, "\"%s\"", str_buf);
        yylval.token.str = strdup(full_str); yylval.token.line = token_line; yylval.token.col = token_col;
        return STRLIT;
    }
}
<STR>\\[fnrt\\"]     { if(str_idx < 65534) {str_buf[str_idx++] = '\\';str_buf[str_idx++] = yytext[1];}}
<STR>\\[^fnrt\\"\r\n] { printf("Line %d, col %d: invalid escape sequence (%s)\n", token_line, token_col, yytext);str_invalid_escape = true;}
<STR>\\              { printf("Line %d, col %d: invalid escape sequence (\\)\n", token_line, token_col);str_invalid_escape = true;}
<STR>\n|\r|\r\n      { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); line++; col = 1; BEGIN(INITIAL); }
<STR><<EOF>>         { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); return 0; }
<STR>.               { if(str_idx < 65535) {str_buf[str_idx++] = yytext[0];}}
[ \t\f]                 { }
\n|\r|\r\n              { line++; col = 1; }
.                       { printf("Line %d, col %d: illegal character (%s)\n", token_line, token_col, yytext); }
%%
int yywrap() {  
    return 1;
}
