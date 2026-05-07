/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int flag_lex = 0;
int flag_syntax = 0;
int flag_semantics = 0; 
int flag_errors = 0;
int syntax_errors_count = 0;

extern int line, col;
extern int token_line, token_col;
extern char* yytext;
extern int yyleng;
extern int yychar;
extern int str_line, str_col;

int yylex(void);
void yyerror(char *s);

typedef struct _node {
    char *type;
    char *value;
    char *anot_type;
    int line;
    int col;  
    struct _node *child;
    struct _node *sibling;
} Node;

Node* create_node(char *type, char *value, int line, int col) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->type = type ? strdup(type) : NULL;
    new_node->value = value ? strdup(value) : NULL;
    new_node->anot_type = NULL;
    new_node->line = line;
    new_node->col = col;
    new_node->child = NULL;
    new_node->sibling = NULL;
    return new_node;
}

typedef struct _symbol {
    char *name;
    char *type;
    char *param_types; 
    int is_param;
    struct _symbol *next;
} Symbol;

typedef struct _table {
    char *name;     
    char *type;    
    Symbol *symbols;
    struct _table *next;
} SymTable;

Node* root = NULL;
SymTable *global_table = NULL;
SymTable *tables_list = NULL;
SymTable *current_method_table = NULL;

Node* add_sibling(Node* node, Node* sibling) {
    if (!node) return sibling;
    if (!sibling) return node;
    Node* curr = node;
    while (curr->sibling != NULL) {
        curr = curr->sibling;
    }
    curr->sibling = sibling;
    return node;
}

void add_child(Node *parent, Node *child) {
    if (!parent || !child) return;
    if (!parent->child) {
        parent->child = child;
    } else {
        add_sibling(parent->child, child);
    }
}

typedef struct {
    char *str;
    int line;
    int col;
} TokenInfo;

%}

%union {
    struct _node* node;
    TokenInfo tinfo;
}

%token <tinfo> BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE
%token <tinfo> AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR
%token <tinfo> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type <node> Program ProgramElements MethodDecl FieldDecl Type MethodHeader 
%type <node> OptFormalParams FormalParams FormalParamsRest MethodBody 
%type <node> StatementAndVarDeclList VarDecl IdList StatementList Statement 
%type <node> OptExpr ExprOrStrlit MethodInvocation OptExprList ExprList 
%type <node> Assignment ParseArgs Expr OptDotLength

%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT LE GT GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT UNARY_PLUS UNARY_MINUS

%%

Program : CLASS IDENTIFIER LBRACE ProgramElements RBRACE {
            $$ = create_node("Program", NULL, token_line, token_col);
            add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
            if ($4) add_child($$, $4); 
            root = $$; 
        }

ProgramElements : { $$ = NULL; }
                | ProgramElements MethodDecl { $$ = add_sibling($1, $2); }
                | ProgramElements FieldDecl { $$ = add_sibling($1, $2); }
                | ProgramElements SEMICOLON { $$ = $1; }
                ;

MethodDecl : PUBLIC STATIC MethodHeader MethodBody {
            $$ = create_node("MethodDecl", NULL, token_line, token_col);
            add_child($$, $3);
            add_sibling($3, $4);
        }
           ;

FieldDecl : PUBLIC STATIC Type IDENTIFIER IdList SEMICOLON {
            $$ = create_node("FieldDecl", NULL, token_line, token_col);
            add_child($$, $3);
            add_child($$, create_node("Identifier", $4.str, $4.line, $4.col));
            Node* aux = $5;
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_field = create_node("FieldDecl", NULL, token_line, token_col);
                add_child(new_field, create_node($3->type, NULL, token_line, token_col)); 
                add_child(new_field, aux);
                $$ = add_sibling($$, new_field);
                aux = next;
            }
          }
          | error SEMICOLON { $$ = NULL; }
          ;

VarDecl : Type IDENTIFIER IdList SEMICOLON {
            $$ = create_node("VarDecl", NULL, token_line, token_col);
            add_child($$, $1);
            add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
            Node* aux = $3;
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_var = create_node("VarDecl", NULL, token_line, token_col);
                add_child(new_var, create_node($1->type, NULL, token_line, token_col));
                add_child(new_var, aux);
                $$ = add_sibling($$, new_var);
                aux = next;
            }
        }
        ;

IdList :{ $$ = NULL; }
       | IdList COMMA IDENTIFIER {   $$ = add_sibling($1, create_node("Identifier", $3.str, $3.line, $3.col)); }
       ;

Type : BOOL   { $$ = create_node("Bool", NULL, token_line, token_col); }
     | INT    { $$ = create_node("Int", NULL, token_line, token_col); }
     | DOUBLE { $$ = create_node("Double", NULL, token_line, token_col); }
     ;

MethodHeader : Type IDENTIFIER LPAR OptFormalParams RPAR {
                $$ = create_node("MethodHeader", NULL, token_line, token_col);
                add_child($$, $1);
                add_sibling($1, create_node("Identifier", $2.str, $2.line, $2.col));
                
                Node* params = create_node("MethodParams", NULL, token_line, token_col);
                if ($4) add_child(params, $4);
                add_sibling($1, params);
             }
             | VOID IDENTIFIER LPAR OptFormalParams RPAR {
                $$ = create_node("MethodHeader", NULL, token_line, token_col);
                Node* v = create_node("Void", NULL, token_line, token_col);
                add_child($$, v);
                add_sibling(v, create_node("Identifier", $2.str, $2.line, $2.col));
                
                Node* params = create_node("MethodParams", NULL, token_line, token_col);
                if ($4) add_child(params, $4);
                add_sibling(v, params);
             }
             ;

OptFormalParams : { $$ = NULL; }
                | FormalParams { $$ = $1; }
                ;

FormalParams : Type IDENTIFIER FormalParamsRest {
                $$ = create_node("ParamDecl", NULL, token_line, token_col);
                add_child($$, $1);
                add_sibling($1, create_node("Identifier", $2.str, $2.line, $2.col));
                if ($3) add_sibling($$, $3);
             }
             | STRING LSQ RSQ IDENTIFIER {
                $$ = create_node("ParamDecl", NULL, token_line, token_col);
                Node* strArray = create_node("StringArray", NULL, token_line, token_col);
                add_child($$, strArray);
                add_sibling(strArray, create_node("Identifier", $4.str, $4.line, $4.col));
             }
             ;

FormalParamsRest : { $$ = NULL; }
                 | FormalParamsRest COMMA Type IDENTIFIER {
                    Node* param = create_node("ParamDecl", NULL, token_line, token_col);
                    add_child(param, $3);
                    add_sibling($3, create_node("Identifier", $4.str, $4.line, $4.col));
                    $$ = add_sibling($1, param);
                 }
                 ;

MethodBody : LBRACE StatementAndVarDeclList RBRACE {
                $$ = create_node("MethodBody", NULL, token_line, token_col);
                if ($2) add_child($$, $2);
           }
           ;

StatementAndVarDeclList : { $$ = NULL; }
                        | StatementAndVarDeclList Statement {
                            if ($2 != NULL) { 
                                $$ = add_sibling($1, $2);
                            } else {
                                $$ = $1;
                            }
                        }
                        | StatementAndVarDeclList VarDecl { $$ = add_sibling($1, $2); }
                        ;


StatementList : { $$ = NULL; }
              | StatementList Statement {
                  if ($2 != NULL) {
                      $$ = add_sibling($1, $2);
                  } else {
                      $$ = $1;
                  }
              }
              ;

Statement : LBRACE StatementList RBRACE {
              if ($2 != NULL && $2->sibling != NULL) {
                  $$ = create_node("Block", NULL, token_line, token_col);
                  add_child($$, $2);
              } else if ($2 != NULL) {
                  $$ = $2; 
              } else {
                  $$ = NULL; 
              }
          }
          | IF LPAR Expr RPAR Statement %prec IF_WITHOUT_ELSE {
              $$ = create_node("If", NULL, $1.line, $1.col);
              add_child($$, $3); 
              add_child($$, $5 ? $5 : create_node("Block", NULL, token_line, token_col)); 
              add_child($$, create_node("Block", NULL, token_line, token_col)); 
          }
          | IF LPAR Expr RPAR Statement ELSE Statement {
              $$ = create_node("If", NULL, $1.line, $1.col);
              add_child($$, $3);
              add_child($$, $5 ? $5 : create_node("Block", NULL, token_line, token_col));
              add_child($$, $7 ? $7 : create_node("Block", NULL, token_line, token_col));
          }
          | WHILE LPAR Expr RPAR Statement {
              $$ = create_node("While", NULL, $1.line, $1.col);
              add_child($$, $3);
              add_child($$, $5 ? $5 : create_node("Block", NULL, token_line, token_col));
          }
          | RETURN OptExpr SEMICOLON {
              $$ = create_node("Return", NULL, $1.line, $1.col);
              if ($2) add_child($$, $2);
          }
          | MethodInvocation SEMICOLON { $$ = $1; }
          | Assignment SEMICOLON { $$ = $1; }
          | ParseArgs SEMICOLON { $$ = $1; }
          | PRINT LPAR ExprOrStrlit RPAR SEMICOLON {
              $$ = create_node("Print", NULL, $1.line, $1.col);
              add_child($$, $3);
          }
          | error SEMICOLON { $$ = NULL; }
          | SEMICOLON { $$ = NULL; }
          ;

OptExpr : { $$ = NULL; }
        | Expr { $$ = $1; }
        ;

ExprOrStrlit : Expr { $$ = $1; }
             | STRLIT { $$ = create_node("StrLit", $1.str, $1.line, $1.col); }
             ;

MethodInvocation : IDENTIFIER LPAR OptExprList RPAR {
                     $$ = create_node("Call", NULL, $1.line, $1.col);
                     Node* id = create_node("Identifier", $1.str, $1.line, $1.col);
                     add_child($$, id);
                     if ($3) add_sibling(id, $3);
                 }
                 | IDENTIFIER LPAR error RPAR { $$ = NULL; }
                 ;

OptExprList : { $$ = NULL; }
            | ExprList { $$ = $1; }
            ;

ExprList : Expr { $$ = $1; }
         | ExprList COMMA Expr { $$ = add_sibling($1, $3); }
         ;

Assignment : IDENTIFIER ASSIGN Expr {
               $$ = create_node("Assign", NULL, $2.line, $2.col);
               Node* id = create_node("Identifier", $1.str, $1.line, $1.col);
               add_child($$, id);
               add_sibling(id, $3);
           }
           ;

ParseArgs : PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR {
              $$ = create_node("ParseArgs", NULL, $1.line, $1.col);
              Node* id = create_node("Identifier", $3.str, $3.line, $3.col);
              add_child($$, id);
              add_sibling(id, $5);
          }
          | PARSEINT LPAR error RPAR { $$ = NULL; }
          ;

Expr : Expr PLUS Expr   { $$ = create_node("Add", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr MINUS Expr  { $$ = create_node("Sub", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr STAR Expr   { $$ = create_node("Mul", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr DIV Expr    { $$ = create_node("Div", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr MOD Expr    { $$ = create_node("Mod", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr AND Expr    { $$ = create_node("And", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr OR Expr     { $$ = create_node("Or", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr XOR Expr    { $$ = create_node("Xor", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr LSHIFT Expr { $$ = create_node("Lshift", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr RSHIFT Expr { $$ = create_node("Rshift", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr EQ Expr     { $$ = create_node("Eq", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr GE Expr     { $$ = create_node("Ge", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr GT Expr     { $$ = create_node("Gt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr LE Expr     { $$ = create_node("Le", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr LT Expr     { $$ = create_node("Lt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr NE Expr     { $$ = create_node("Ne", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | MINUS Expr %prec UNARY_MINUS { $$ = create_node("Minus", NULL, $1.line, $1.col); add_child($$, $2); }
     | PLUS Expr %prec UNARY_PLUS   { $$ = create_node("Plus", NULL, $1.line, $1.col); add_child($$, $2); }
     | NOT Expr         { $$ = create_node("Not", NULL, $1.line, $1.col); add_child($$, $2); }
     | LPAR Expr RPAR   { $$ = $2; }
     | LPAR error RPAR  { $$ = NULL; }
     | MethodInvocation { $$ = $1; }
     | Assignment       { $$ = $1; }
     | ParseArgs        { $$ = $1; }
     | IDENTIFIER OptDotLength {
         if ($2) {
             $$ = create_node("Length", NULL, $2->line, $2->col);
             add_child($$, create_node("Identifier", $1.str, $1.line, $1.col));
         } else {
             $$ = create_node("Identifier", $1.str, $1.line, $1.col);
         }
     }
     | NATURAL          { $$ = create_node("Natural", $1.str, $1.line, $1.col); }
     | DECIMAL          { $$ = create_node("Decimal", $1.str, $1.line, $1.col); }
     | BOOLLIT          { $$ = create_node("BoolLit", $1.str, $1.line, $1.col); }
     ;

OptDotLength : { $$ = NULL; }
             | DOTLENGTH { $$ = create_node("DotLength", NULL, token_line, token_col); }
             ;

%%

char* get_juc_type(char* ast_type) {
    if (!ast_type) return "undef";
    if (strcmp(ast_type, "Int") == 0) return "int";
    if (strcmp(ast_type, "Double") == 0) return "double";
    if (strcmp(ast_type, "Bool") == 0) return "boolean";
    if (strcmp(ast_type, "Void") == 0) return "void";
    if (strcmp(ast_type, "StringArray") == 0) return "String[]";
    return "undef";
}

Symbol* create_symbol(char *name, char *type, char *param_types, int is_param) {
    Symbol *s = (Symbol*)malloc(sizeof(Symbol));
    s->name = name ? strdup(name) : strdup("");
    s->type = type ? strdup(type) : strdup("undef");
    s->param_types = param_types ? strdup(param_types) : NULL;
    s->is_param = is_param;
    s->next = NULL;
    return s;
}

int insert_symbol(SymTable *table, char *name, char *type, char *param_types, int is_param, int line, int col) {
    if (!table || !name) return 0;
    if (strcmp(name, "_") == 0) {
        printf("Line %d, col %d: Symbol _ is reserved\n", line, col);
        return 0;
    }
    Symbol *curr = table->symbols;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            if (curr->param_types == NULL && param_types == NULL) {
                printf("Line %d, col %d: Symbol %s already defined\n", line, col, name);
                return 0;
            }
            if (curr->param_types != NULL && param_types != NULL) {
                if (strcmp(curr->param_types, param_types) == 0) {
                    printf("Line %d, col %d: Symbol %s already defined\n", line, col, name);
                    return 0;
                }
            }
        }
        curr = curr->next;
    }
    Symbol *s = create_symbol(name, type, param_types, is_param);
    if (!table->symbols) {
        table->symbols = s;
    } else {
        curr = table->symbols;
        while (curr->next != NULL) curr = curr->next;
        curr->next = s;
    }
    return 1;
}

SymTable* create_table(char *name, char *type) {
    SymTable *t = (SymTable*)malloc(sizeof(SymTable));
    t->name = name ? strdup(name) : strdup("");
    t->type = type ? strdup(type) : strdup("");
    t->symbols = NULL;
    t->next = NULL;
    
    if (!tables_list) {
        tables_list = t;
    } else {
        SymTable *curr = tables_list;
        while (curr->next != NULL) curr = curr->next;
        curr->next = t;
    }
    return t;
}

char* build_param_types(Node *methodParams) {
    if (!methodParams) return strdup("");
    int len = 1;
    Node *param = methodParams->child;
    while (param) {
        if (param->child && param->child->type) {
            len += strlen(get_juc_type(param->child->type)) + 2;
        }
        param = param->sibling;
    }
    char *buf = (char*)malloc(len);
    buf[0] = '\0';
    param = methodParams->child;
    while (param) {
        if (param->child && param->child->type) {
            strcat(buf, get_juc_type(param->child->type));
        }
        param = param->sibling;
        if (param) strcat(buf, ",");
    }
    return buf;
}

void build_symbol_tables(Node *node) {
    if (!node || !node->type) return;
    if (strcmp(node->type, "Program") == 0) {
        if (node->child && node->child->value) {
            char *class_name = (char*)malloc(strlen(node->child->value) + 10);
            sprintf(class_name, "Class %s", node->child->value);
            global_table = create_table(class_name, "Class");
            free(class_name);
        }
        Node *child = node->child ? node->child->sibling : NULL;
        while (child) {
            build_symbol_tables(child);
            child = child->sibling;
        }
    } 
    else if (strcmp(node->type, "FieldDecl") == 0) {
        if (node->child && node->child->sibling) {
            char *type = get_juc_type(node->child->type);
            Node *id_node = node->child->sibling;
            char *name = id_node->value ? id_node->value : "";
            insert_symbol(global_table, name, type, NULL, 0, id_node->line, id_node->col);
        }
    } 
    else if (strcmp(node->type, "MethodDecl") == 0) {
        Node *header = node->child;
        if (header && header->child && header->child->sibling) {
            Node *ret_type_node = header->child;
            Node *id_node = ret_type_node->sibling;
            Node *params_node = id_node->sibling;
            
            char *ret_type = get_juc_type(ret_type_node->type);
            char *method_name = id_node->value ? id_node->value : "";
            char *params_str = build_param_types(params_node);
            
            insert_symbol(global_table, method_name, ret_type, params_str, 0, id_node->line, id_node->col);
            
            char *table_name = (char*)malloc(strlen(method_name) + strlen(params_str) + 20);
            sprintf(table_name, "Method %s(%s)", method_name, params_str);
            current_method_table = create_table(table_name, "Method");
            free(table_name);
            
            insert_symbol(current_method_table, "return", ret_type, NULL, 0, id_node->line, id_node->col);
            
            Node *param = params_node ? params_node->child : NULL;
            while (param) {
                if (param->child && param->child->sibling) {
                    Node *p_id = param->child->sibling;
                    char *p_type = get_juc_type(param->child->type);
                    char *p_name = p_id->value ? p_id->value : "";
                    insert_symbol(current_method_table, p_name, p_type, NULL, 1, p_id->line, p_id->col);
                }
                param = param->sibling;
            }
            
            Node *body = header->sibling;
            if (body) {
                Node *stmt = body->child;
                while (stmt) {
                    if (stmt->type && strcmp(stmt->type, "VarDecl") == 0 && stmt->child && stmt->child->sibling) {
                        Node *v_id = stmt->child->sibling;
                        char *v_type = get_juc_type(stmt->child->type);
                        char *v_name = v_id->value ? v_id->value : "";
                        insert_symbol(current_method_table, v_name, v_type, NULL, 0, v_id->line, v_id->col);
                    }
                    stmt = stmt->sibling;
                }
            }
            free(params_str);
        }
    }
}

void print_tables() {
    SymTable *t = tables_list;
    while (t) {
        printf("===== %s Symbol Table =====\n", t->name);
        Symbol *s = t->symbols;
        while (s) {
            if (s->param_types) {
                printf("%s\t(%s)\t%s\n", s->name, s->param_types, s->type);
            } else {
                if (s->is_param) {
                    printf("%s\t\t%s\tparam\n", s->name, s->type);
                } else {
                    printf("%s\t\t%s\n", s->name, s->type);
                }
            }
            s = s->next;
        }
        if (t->next) printf("\n");
        t = t->next;
    }
}

int get_error_col() { return col - yyleng; }

void yyerror(char *s) {
    syntax_errors_count++;
    if (yychar == 0) {
        printf("Line %d, col %d: %s: \"end of file\"\n", line, col, s);
    } 
    else if (yychar == STRLIT) {
        printf("Line %d, col %d: %s: %s\n", str_line, str_col, s, yylval.tinfo.str);
    } 
    else {
        printf("Line %d, col %d: %s: %s\n", token_line, token_col, s, yytext);
    }
}

void print_tree(Node* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("..");
    if (node->value) {
        if (strcmp(node->type, "Natural") == 0 || strcmp(node->type, "Decimal") == 0) {
            char *clean_val = strdup(node->value);
            char *src = clean_val, *dst = clean_val;
            while (*src) {
                if (*src != '_') *dst++ = *src;
                src++;
            }
            *dst = '\0';
            printf("%s(%s)", node->type, clean_val);
            free(clean_val);
        } else {
            printf("%s(%s)", node->type, node->value);
        }
    }
    else printf("%s", node->type);
    if (node->anot_type != NULL) {
        printf(" - %s", node->anot_type);
    }
    printf("\n");
    Node* child = node->child;
    while (child) {
        print_tree(child, depth + 1);
        child = child->sibling;
    }
}

SymTable* find_table(char *table_name) {
    SymTable *curr = tables_list;
    while (curr) {
        if (strcmp(curr->name, table_name) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

char* lookup_variable_type(SymTable *local_table, char *var_name) {
    if (local_table) {
        Symbol *s = local_table->symbols;
        while (s) {
            if (strcmp(s->name, var_name) == 0) return s->type;
            s = s->next;
        }
    }
    if (global_table) {
        Symbol *s = global_table->symbols;
        while (s) {
            if (strcmp(s->name, var_name) == 0 && s->param_types == NULL) return s->type;
            s = s->next;
        }
    }
    return "undef";
}

int is_expression(char *type) {
    if (!type) return 0;
    char *exprs[] = {"Assign", "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", 
                     "Add", "Sub", "Mul", "Div", "Mod", "Lshift", "Rshift", "Xor", 
                     "Not", "Minus", "Plus", "Length", "Call", "ParseArgs", 
                     "Identifier", "Natural", "Decimal", "BoolLit", "StrLit", NULL};
    for (int i = 0; exprs[i] != NULL; i++) {
        if (strcmp(type, exprs[i]) == 0) return 1;
    }
    return 0;
}

void annotate_ast(Node *node, SymTable *current_env) {
    if (!node || !node->type) return;
    SymTable *next_env = current_env;
    if (strcmp(node->type, "MethodDecl") == 0) {
        Node *header = node->child;
        if (header && header->child && header->child->sibling) {
            char *method_name = header->child->sibling->value ? header->child->sibling->value : "";
            char *params_str = build_param_types(header->child->sibling->sibling);
            char *table_name = (char*)malloc(strlen(method_name) + strlen(params_str) + 20);
            sprintf(table_name, "Method %s(%s)", method_name, params_str);
            next_env = find_table(table_name);
            free(table_name);
            free(params_str);
        }
        /* Only recurse into MethodBody (header->sibling), NOT into MethodHeader */
        Node *body = node->child ? node->child->sibling : NULL;
        if (body) annotate_ast(body, next_env);
        return;
    }
    /* Do not recurse into declaration nodes - they are not expressions */
    if (strcmp(node->type, "MethodHeader") == 0 ||
        strcmp(node->type, "FieldDecl") == 0 ||
        strcmp(node->type, "VarDecl") == 0 ||
        strcmp(node->type, "ParamDecl") == 0 ||
        strcmp(node->type, "MethodParams") == 0) {
        return;
    }
    /* For Program, skip the first child (class name Identifier) */
    if (strcmp(node->type, "Program") == 0) {
        Node *child = node->child ? node->child->sibling : NULL;
        while (child) {
            annotate_ast(child, next_env);
            child = child->sibling;
        }
        return;
    }
    if (strcmp(node->type, "Call") == 0) {
        Node *arg = node->child ? node->child->sibling : NULL;
        while (arg) {
            annotate_ast(arg, next_env);
            arg = arg->sibling;
        }
    } else {
        Node *child = node->child;
        while (child) {
            annotate_ast(child, next_env);
            child = child->sibling;
        }
    }
    
    if (is_expression(node->type)) {
        if (strcmp(node->type, "Natural") == 0) {
            if (node->value) {
                char *clean_val = (char*)malloc(strlen(node->value) + 1);
                int j = 0;
                for(int i = 0; node->value[i] != '\0'; i++) {
                    if(node->value[i] != '_') clean_val[j++] = node->value[i];
                }
                clean_val[j] = '\0';
                double val = atof(clean_val); 
                free(clean_val);
                if (val > 2147483647.0) {
                    printf("Line %d, col %d: Number %s out of bounds\n", node->line, node->col, node->value);
                }
            }
            node->anot_type = strdup("int");
        } 
        else if (strcmp(node->type, "Decimal") == 0) {
            node->anot_type = strdup("double");
        } 
        else if (strcmp(node->type, "BoolLit") == 0) {
            node->anot_type = strdup("boolean");
        } 
        else if (strcmp(node->type, "StrLit") == 0) {
            node->anot_type = strdup("String");
        }
        else if (strcmp(node->type, "Identifier") == 0) {
            char *t = lookup_variable_type(current_env, node->value ? node->value : "");
            if (strcmp(t, "undef") == 0) {
                printf("Line %d, col %d: Cannot find symbol %s\n", node->line, node->col, node->value ? node->value : "");
            }
            node->anot_type = strdup(t);
        }
        else if (strcmp(node->type, "Assign") == 0) {
            Node *c1 = node->child;
            Node *c2 = c1 ? c1->sibling : NULL;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "undef";
            
            int valid = 0;
            if (strcmp(t1, "double") == 0 && (strcmp(t2, "double") == 0 || strcmp(t2, "int") == 0)) valid = 1;
            else if (strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) valid = 1;
            else if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) valid = 1;
            
            if (!valid) {
                printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", node->line, node->col, t1, t2);
                node->anot_type = strdup("undef");
            } else {
                node->anot_type = strdup(t1);
            }
        }
        else if (strcmp(node->type, "Add") == 0 || strcmp(node->type, "Sub") == 0 || 
                 strcmp(node->type, "Mul") == 0 || strcmp(node->type, "Div") == 0 || strcmp(node->type, "Mod") == 0) {
            Node *c1 = node->child;
            Node *c2 = c1 ? c1->sibling : NULL;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "undef";
            char op = strcmp(node->type, "Add")==0 ? '+' : (strcmp(node->type, "Sub")==0 ? '-' : (strcmp(node->type, "Mul")==0 ? '*' : (strcmp(node->type, "Div")==0 ? '/' : '%')));
            
            if ((strcmp(t1, "int") != 0 && strcmp(t1, "double") != 0) || 
                (strcmp(t2, "int") != 0 && strcmp(t2, "double") != 0)) {
                printf("Line %d, col %d: Operator %c cannot be applied to types %s, %s\n", node->line, node->col, op, t1, t2);
                node->anot_type = strdup("undef");
            } else if (strcmp(t1, "double") == 0 || strcmp(t2, "double") == 0) {
                node->anot_type = strdup("double");
            } else {
                node->anot_type = strdup("int");
            }
        }
        else if (strcmp(node->type, "Eq") == 0 || strcmp(node->type, "Ne") == 0 || 
                 strcmp(node->type, "Lt") == 0 || strcmp(node->type, "Gt") == 0 || 
                 strcmp(node->type, "Le") == 0 || strcmp(node->type, "Ge") == 0 ||
                 strcmp(node->type, "And") == 0 || strcmp(node->type, "Or") == 0 || strcmp(node->type, "Xor") == 0) {
            
            Node *c1 = node->child;
            Node *c2 = c1 ? c1->sibling : NULL;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "undef";
            
            char *op_str = "";
            if(strcmp(node->type,"Eq")==0) op_str = "==";
            else if(strcmp(node->type,"Ne")==0) op_str = "!=";
            else if(strcmp(node->type,"Lt")==0) op_str = "<";
            else if(strcmp(node->type,"Gt")==0) op_str = ">";
            else if(strcmp(node->type,"Le")==0) op_str = "<=";
            else if(strcmp(node->type,"Ge")==0) op_str = ">=";
            else if(strcmp(node->type,"And")==0) op_str = "&&";
            else if(strcmp(node->type,"Or")==0) op_str = "||";
            else if(strcmp(node->type,"Xor")==0) op_str = "^";

            int valid = 0;
            char *res_type = "boolean";

            if (strcmp(node->type, "Eq") == 0 || strcmp(node->type, "Ne") == 0) {
                if ((strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) && 
                    (strcmp(t2, "int") == 0 || strcmp(t2, "double") == 0)) valid = 1;
                else if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) valid = 1;
            } 
            else if (strcmp(node->type, "Lt") == 0 || strcmp(node->type, "Gt") == 0 || 
                     strcmp(node->type, "Le") == 0 || strcmp(node->type, "Ge") == 0) {
                if ((strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) && 
                    (strcmp(t2, "int") == 0 || strcmp(t2, "double") == 0)) valid = 1;
            }
            else if (strcmp(node->type, "And") == 0 || strcmp(node->type, "Or") == 0) {
                if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) valid = 1;
            }
            else if (strcmp(node->type, "Xor") == 0) {
                if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) valid = 1;
                else if (strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) {
                    valid = 1;
                    res_type = "int";
                }
            }

            if (!valid) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, op_str, t1, t2);
                node->anot_type = strdup("undef");
            } else {
                node->anot_type = strdup(res_type);
            }
        }
        else if (strcmp(node->type, "Lshift") == 0 || strcmp(node->type, "Rshift") == 0) {
            Node *c1 = node->child;
            Node *c2 = c1 ? c1->sibling : NULL;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "undef";
            char *op_str = strcmp(node->type, "Lshift")==0 ? "<<" : ">>";
            if (strcmp(t1, "int") != 0 || strcmp(t2, "int") != 0) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, op_str, t1, t2);
                node->anot_type = strdup("undef");
            } else {
                node->anot_type = strdup("int");
            }
        }
        else if (strcmp(node->type, "Plus") == 0 || strcmp(node->type, "Minus") == 0) {
            Node *c1 = node->child;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            if (strcmp(t1, "int") != 0 && strcmp(t1, "double") != 0) {
                char op = strcmp(node->type, "Plus") == 0 ? '+' : '-';
                printf("Line %d, col %d: Operator %c cannot be applied to type %s\n", node->line, node->col, op, t1);
                node->anot_type = strdup("undef");
            } else {
                node->anot_type = strdup(t1);
            }
        }
        else if (strcmp(node->type, "Not") == 0) {
            Node *c1 = node->child;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            if (strcmp(t1, "boolean") != 0) {
                printf("Line %d, col %d: Operator ! cannot be applied to type %s\n", node->line, node->col, t1);
                node->anot_type = strdup("undef");
            } else {
                node->anot_type = strdup("boolean");
            }
        }
        else if (strcmp(node->type, "Length") == 0) {
            Node *c1 = node->child;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            if (strcmp(t1, "String[]") != 0) {
                printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", node->line, node->col, t1);
                node->anot_type = strdup("undef");
            } else {
                node->anot_type = strdup("int");
            }
        }
        else if (strcmp(node->type, "ParseArgs") == 0) {
            Node *c1 = node->child;
            Node *c2 = c1 ? c1->sibling : NULL;
            char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "undef";
            char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "undef";
            if (strcmp(t1, "String[]") != 0 || strcmp(t2, "int") != 0) {
                printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", node->line, node->col, t1, t2);
                node->anot_type = strdup("undef");
            } else {
                node->anot_type = strdup("int");
            }
        }
        else if (strcmp(node->type, "Call") == 0) {
            char *method_name = (node->child && node->child->value) ? node->child->value : "";
            
            char arg_types[100][30];
            int num_args = 0;
            Node *arg = node->child->sibling;
            while(arg) {
                char *atype = arg->anot_type ? arg->anot_type : "undef";
                strcpy(arg_types[num_args++], atype);
                arg = arg->sibling;
            }
            Symbol *s = global_table ? global_table->symbols : NULL;
            Symbol *exact_match = NULL;
            Symbol *compat_matches[100];
            int compat_count = 0;
            while (s) {
                if (strcmp(s->name, method_name) == 0 && s->param_types != NULL) {
                    char formal_types[100][30];
                    int num_formals = 0;
                    if (strlen(s->param_types) > 0) {
                        char *params_copy = strdup(s->param_types);
                        char *tok = strtok(params_copy, ",");
                        while(tok) {
                            strcpy(formal_types[num_formals++], tok);
                            tok = strtok(NULL, ",");
                        }
                        free(params_copy);
                    }
                    if (num_args == num_formals) {
                        int is_exact = 1;
                        int is_compat = 1;
                        for (int i = 0; i < num_args; i++) {
                            if (strcmp(arg_types[i], formal_types[i]) != 0) {
                                is_exact = 0;
                                if (!(strcmp(arg_types[i], "int") == 0 && strcmp(formal_types[i], "double") == 0)) {
                                    is_compat = 0;
                                }
                            }
                        }
                        if (is_exact) {
                            exact_match = s;
                            break;
                        } else if (is_compat) {
                            compat_matches[compat_count++] = s;
                        }
                    }
                }
                s = s->next;
            }
            if (exact_match) {
                node->anot_type = strdup(exact_match->type);
                char sig[200];
                if (exact_match->param_types) sprintf(sig, "(%s)%s", exact_match->param_types, exact_match->type);
                else sprintf(sig, "()%s", exact_match->type);
                if (node->child) node->child->anot_type = strdup(sig);
            } else if (compat_count == 1) {
                node->anot_type = strdup(compat_matches[0]->type);
                char sig[200];
                if (compat_matches[0]->param_types) sprintf(sig, "(%s)%s", compat_matches[0]->param_types, compat_matches[0]->type);
                else sprintf(sig, "()%s", compat_matches[0]->type);
                if (node->child) node->child->anot_type = strdup(sig);
            } else if (compat_count > 1) {
                printf("Line %d, col %d: Reference to method %s is ambiguous\n", node->child->line, node->child->col, method_name);
                node->anot_type = strdup("undef");
                if (node->child) node->child->anot_type = strdup("undef");
            } else {
                printf("Line %d, col %d: Cannot find symbol %s\n", node->child->line, node->child->col, method_name);
                node->anot_type = strdup("undef");
                if (node->child) node->child->anot_type = strdup("undef");
            }
        }
    }
    if (strcmp(node->type, "If") == 0 || strcmp(node->type, "While") == 0) {
        Node *condition = node->child;
        char *c_type = (condition && condition->anot_type) ? condition->anot_type : "undef";
        if (strcmp(c_type, "boolean") != 0) {
            char *stmt = strcmp(node->type, "If") == 0 ? "if" : "while";
            printf("Line %d, col %d: Incompatible type %s in %s statement\n", node->line, node->col, c_type, stmt);
        }
    }
    else if (strcmp(node->type, "Print") == 0) {
        Node *c = node->child;
        char *c_type = (c && c->anot_type) ? c->anot_type : "undef";
        if (strcmp(c_type, "String[]") == 0 || strcmp(c_type, "void") == 0 || strcmp(c_type, "undef") == 0) {
            printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n", node->line, node->col, c_type);
        }
    }
    else if (strcmp(node->type, "Return") == 0) {
        char *expected_return = lookup_variable_type(next_env, "return");
        char *actual_return = (node->child && node->child->anot_type) ? node->child->anot_type : "void"; 
        
        int valid = 0;
        if (strcmp(expected_return, "double") == 0 && (strcmp(actual_return, "double") == 0 || strcmp(actual_return, "int") == 0)) valid = 1;
        else if (strcmp(expected_return, actual_return) == 0) valid = 1;
        
        if (!valid) {
            if (strcmp(expected_return, "void") != 0 && node->child == NULL) {
                printf("Line %d, col %d: Incompatible type void in return statement\n", node->line, node->col);
            } else {
                printf("Line %d, col %d: Incompatible type %s in return statement\n", node->line, node->col, actual_return);
            }
        }
    }
}

void clean_declaration_annotations(Node *node) {
    if (!node || !node->type) return;
    if (strcmp(node->type, "FieldDecl") == 0 || strcmp(node->type, "VarDecl") == 0 ||
        strcmp(node->type, "MethodHeader") == 0 || strcmp(node->type, "ParamDecl") == 0) {
        Node *id = node->child;
        while (id) {
            if (id->type && strcmp(id->type, "Identifier") == 0) {
                id->anot_type = NULL;
            }
            id = id->sibling;
        }
    }
    Node *child = node->child;
    while (child) {
        clean_declaration_annotations(child);
        child = child->sibling;
    }
}

int main(int argc, char *argv[]) {
    int flag_errors_sem = 0;
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            flag_lex = 1;
            while (yylex());
            return 0;
        } else if (strcmp(argv[1], "-e1") == 0) {
            while (yylex());
            return 0;
        } else if (strcmp(argv[1], "-t") == 0) {
            flag_syntax = 1;
        } else if (strcmp(argv[1], "-e2") == 0) {
            flag_errors = 1;
        } else if (strcmp(argv[1], "-s") == 0) {
            flag_semantics = 1;
        } else if (strcmp(argv[1], "-e3") == 0) {
            flag_errors_sem = 1;
        }
    } 
    yyparse();
    if (syntax_errors_count == 0) {
        if (flag_syntax) {
            print_tree(root, 0);
        } else if (flag_semantics || flag_errors_sem) {
            build_symbol_tables(root);
            annotate_ast(root, NULL);
            clean_declaration_annotations(root);
            
            if (flag_semantics) {
                print_tables();
                printf("\n");
                print_tree(root, 0);
            }
        }
    }
    return 0;
}