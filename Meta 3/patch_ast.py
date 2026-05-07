import re

with open("jucompiler.l", "r") as f:
    lex = f.read()

lex = lex.replace('yylval.str', 'yylval.tinfo.str')
lex = lex.replace('#define YY_USER_ACTION token_line = line; token_col = col; col += yyleng;',
                  '#define YY_USER_ACTION token_line = line; token_col = col; col += yyleng; yylval.tinfo.line = token_line; yylval.tinfo.col = token_col;')

with open("jucompiler.l", "w") as f:
    f.write(lex)


with open("jucompiler.y", "r") as f:
    yacc = f.read()

# Add TokenInfo to union
yacc = yacc.replace('%union {\n    char* str;\n    struct _node* node;\n}',
'''typedef struct {
    char *str;
    int line;
    int col;
} TokenInfo;

%union {
    struct _node* node;
    TokenInfo tinfo;
}''')

# Change tokens to tinfo
yacc = yacc.replace('%token BOOL', '%token <tinfo> BOOL')
yacc = yacc.replace('%token AND', '%token <tinfo> AND')
yacc = yacc.replace('%token <str>', '%token <tinfo>')

# Fix Identifier, Natural, etc
yacc = re.sub(r'create_node\("Identifier",\s*\$([0-9]+),\s*token_line,\s*token_col\)', r'create_node("Identifier", $\1.str, $\1.line, $\1.col)', yacc)
yacc = re.sub(r'create_node\("Natural",\s*\$([0-9]+),\s*token_line,\s*token_col\)', r'create_node("Natural", $\1.str, $\1.line, $\1.col)', yacc)
yacc = re.sub(r'create_node\("Decimal",\s*\$([0-9]+),\s*token_line,\s*token_col\)', r'create_node("Decimal", $\1.str, $\1.line, $\1.col)', yacc)
yacc = re.sub(r'create_node\("StrLit",\s*\$([0-9]+),\s*token_line,\s*token_col\)', r'create_node("StrLit", $\1.str, $\1.line, $\1.col)', yacc)
yacc = re.sub(r'create_node\("BoolLit",\s*\$([0-9]+),\s*token_line,\s*token_col\)', r'create_node("BoolLit", $\1.str, $\1.line, $\1.col)', yacc)

# Fix Operators
yacc = re.sub(r'create_node\("Add",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Add", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Sub",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Sub", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Mul",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Mul", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Div",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Div", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Mod",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Mod", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("And",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("And", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Or",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Or", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Xor",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Xor", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Lshift",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Lshift", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Rshift",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Rshift", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Eq",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Eq", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Ge",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Ge", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Gt",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Gt", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Le",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Le", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Lt",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Lt", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("Ne",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Ne", NULL, $2.line, $2.col)', yacc)

yacc = re.sub(r'create_node\("Plus",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Plus", NULL, $1.line, $1.col)', yacc)
yacc = re.sub(r'create_node\("Minus",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Minus", NULL, $1.line, $1.col)', yacc)
yacc = re.sub(r'create_node\("Not",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Not", NULL, $1.line, $1.col)', yacc)
yacc = re.sub(r'create_node\("Length",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Length", NULL, $1.line, $1.col)', yacc)

yacc = re.sub(r'create_node\("Assign",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Assign", NULL, $2.line, $2.col)', yacc)
yacc = re.sub(r'create_node\("ParseArgs",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("ParseArgs", NULL, $1.line, $1.col)', yacc)
yacc = re.sub(r'create_node\("Call",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Call", NULL, $1.line, $1.col)', yacc)

yacc = re.sub(r'create_node\("If",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("If", NULL, $1.line, $1.col)', yacc)
yacc = re.sub(r'create_node\("While",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("While", NULL, $1.line, $1.col)', yacc)
yacc = re.sub(r'create_node\("Return",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Return", NULL, $1.line, $1.col)', yacc)
yacc = re.sub(r'create_node\("Print",\s*NULL,\s*token_line,\s*token_col\)', r'create_node("Print", NULL, $1.line, $1.col)', yacc)

# Change usage of yylval.str in yyerror logic if any? yyerror uses str_line/str_col and token_line/token_col, which is fine!
# But what about yylval.str in yyerror?
yacc = yacc.replace('yylval.str', 'yylval.tinfo.str')

with open("jucompiler.y", "w") as f:
    f.write(yacc)

print("Patch applied successfully.")
