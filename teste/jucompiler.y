/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Definição da Árvore (Node) - Tem de vir PRIMEIRO!
typedef struct _node {
    char *type;
    char *value;
    struct _node *child;
    struct _node *sibling;
} Node;

Node* root = NULL; 

Node* create_node(char *type, char *value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->type = type ? strdup(type) : NULL;
    new_node->value = value ? strdup(value) : NULL;
    new_node->child = NULL;
    new_node->sibling = NULL;
    return new_node;
}

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

// 2. Estruturas da Semântica
typedef struct _symbol {
    char* name;
    char* type;
    char* param_types;
    int is_param;
    struct _symbol* next;
} Symbol;

typedef struct _symtable {
    char* name;
    Symbol* first;
    Symbol* last;
    struct _symtable* next;
} SymTable;

// 3. Variáveis Globais (Flags, Tabelas e Contadores)
SymTable* global_table = NULL;
SymTable* local_tables_head = NULL;
SymTable* local_tables_tail = NULL;

int flag_lex = 0;
int flag_syntax = 0;
int flag_errors = 0;
int syntax_errors_count = 0;
int flag_semantic = 0;
int flag_semantic_errors = 0;
int assign_line = 0, assign_col = 0;
int eq_line = 0, eq_col = 0;

extern int line, col;
extern int token_line, token_col;
extern char* yytext;
extern int yyleng;
extern int yychar;
extern int str_line, str_col;

int yylex(void);
void yyerror(char *s);

// 4. Funções da Semântica
char* get_java_type(char* ast_type) {
    if (!ast_type) return "undef";
    if (strcmp(ast_type, "Int") == 0) return "int";
    if (strcmp(ast_type, "Double") == 0) return "double";
    if (strcmp(ast_type, "Bool") == 0) return "boolean";
    if (strcmp(ast_type, "Void") == 0) return "void";
    if (strcmp(ast_type, "StringArray") == 0) return "String[]";
    return "undef";
}

SymTable* create_table(char* name) {
    SymTable* t = (SymTable*)malloc(sizeof(SymTable));
    t->name = strdup(name); // Alocação segura
    t->first = NULL;
    t->last = NULL;
    t->next = NULL;
    return t;
}

void add_symbol(SymTable* table, char* name, char* type, char* param_types, int is_param) {
    Symbol* s = (Symbol*)malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->type = strdup(type);
    s->param_types = param_types ? strdup(param_types) : strdup("");
    s->is_param = is_param;
    s->next = NULL;

    if (table->first == NULL) {
        table->first = s;
        table->last = s;
    } else {
        table->last->next = s;
        table->last = s;
    }
}

void add_local_table(SymTable* t) {
    if (local_tables_head == NULL) {
        local_tables_head = t;
        local_tables_tail = t;
    } else {
        local_tables_tail->next = t;
        local_tables_tail = t;
    }
}

// O algoritmo completo de travessia (inclui FieldDecl e VarDecl)
void parse_method_declarations(Node* root) {
    if (!root || strcmp(root->type, "Program") != 0) return;

    Node* class_id_node = root->child; 
    char global_name[2048]; // Buffer gigante para evitar overflows de sprintf
    sprintf(global_name, "Class %s", class_id_node->value);
    global_table = create_table(global_name);

    Node* curr = class_id_node->sibling;
    while (curr != NULL) {
        // 1. Capturar Variáveis Globais da Classe (FieldDecl)
        if (strcmp(curr->type, "FieldDecl") == 0) {
            Node* type_node = curr->child;
            Node* id_node = type_node->sibling;
            add_symbol(global_table, id_node->value, get_java_type(type_node->type), NULL, 0);
        }
        
        // 2. Capturar Métodos e Variáveis Locais
        if (strcmp(curr->type, "MethodDecl") == 0) {
            Node* method_header = curr->child;
            Node* return_type_node = method_header->child;
            Node* method_id_node = return_type_node->sibling;
            Node* method_params_node = method_id_node->sibling;

            char* ret_type_java = get_java_type(return_type_node->type);
            char* method_name = method_id_node->value;

            char param_str[2048] = "("; // Buffer gigante
            Node* param = method_params_node->child;
            int first_param = 1;

            while (param != NULL) {
                if (!first_param) strcat(param_str, ", ");
                strcat(param_str, get_java_type(param->child->type));
                first_param = 0;
                param = param->sibling;
            }
            strcat(param_str, ")");

            // Adicionar método à Tabela Global
            add_symbol(global_table, method_name, ret_type_java, param_str, 0);

            // Criar Tabela Local para o Método
            char local_name[2048];
            sprintf(local_name, "Method %s%s", method_name, param_str);
            SymTable* local_table = create_table(local_name);

            // Símbolo especial de retorno exigido
            add_symbol(local_table, "return", ret_type_java, NULL, 0);

            // Adicionar Parâmetros à Tabela Local
            param = method_params_node->child;
            while (param != NULL) {
                Node* param_type_node = param->child;
                Node* param_id_node = param_type_node->sibling;
                add_symbol(local_table, param_id_node->value, get_java_type(param_type_node->type), NULL, 1);
                param = param->sibling;
            }

            // Adicionar Variáveis Locais (VarDecl) dentro do MethodBody
            Node* method_body = method_header->sibling; // Na vossa AST, MethodBody é irmão do MethodHeader
            if (method_body != NULL) {
                Node* stmt = method_body->child;
                while (stmt != NULL) {
                    if (strcmp(stmt->type, "VarDecl") == 0) {
                        Node* vtype = stmt->child;
                        Node* vid = vtype->sibling;
                        add_symbol(local_table, vid->value, get_java_type(vtype->type), NULL, 0);
                    }
                    stmt = stmt->sibling;
                }
            }

            add_local_table(local_table);
        }
        curr = curr->sibling;
    }
}
%}

%union {
    char* str;
    struct _node* node;
}

%token BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE
%token AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR
%token <str> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type <node> Program ProgramElements MethodDecl FieldDecl Type MethodHeader 
%type <node> OptFormalParams FormalParams FormalParamsRest MethodBody 
%type <node> StatementAndVarDeclList VarDecl IdList StatementList Statement 
%type <node> OptExpr ExprOrStrlit MethodInvocation OptExprList ExprList 
%type <node> Assignment ParseArgs Expr AExpr OptDotLength

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
            $$ = create_node("Program", NULL);
            add_child($$, create_node("Identifier", $2));
            if ($4) add_child($$, $4); 
            root = $$; 
        }

ProgramElements : { $$ = NULL; }
                | ProgramElements MethodDecl { $$ = add_sibling($1, $2); }
                | ProgramElements FieldDecl { $$ = add_sibling($1, $2); }
                | ProgramElements SEMICOLON { $$ = $1; }
                ;

MethodDecl : PUBLIC STATIC MethodHeader MethodBody {
            $$ = create_node("MethodDecl", NULL);
            add_child($$, $3);
            add_sibling($3, $4);
        }
           ;

FieldDecl : PUBLIC STATIC Type IDENTIFIER IdList SEMICOLON {
            $$ = create_node("FieldDecl", NULL);
            add_child($$, $3);
            add_child($$, create_node("Identifier", $4));
            Node* aux = $5;
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_field = create_node("FieldDecl", NULL);
                add_child(new_field, create_node($3->type, NULL)); 
                add_child(new_field, aux);
                $$ = add_sibling($$, new_field);
                aux = next;
            }
          }
          | error SEMICOLON { $$ = NULL; }
          ;

VarDecl : Type IDENTIFIER IdList SEMICOLON {
            $$ = create_node("VarDecl", NULL);
            add_child($$, $1);
            add_child($$, create_node("Identifier", $2));
            Node* aux = $3;
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_var = create_node("VarDecl", NULL);
                add_child(new_var, create_node($1->type, NULL));
                add_child(new_var, aux);
                $$ = add_sibling($$, new_var);
                aux = next;
            }
        }
        ;

IdList :{ $$ = NULL; }
       | IdList COMMA IDENTIFIER {   $$ = add_sibling($1, create_node("Identifier", $3)); }
       ;

Type : BOOL   { $$ = create_node("Bool", NULL); }
     | INT    { $$ = create_node("Int", NULL); }
     | DOUBLE { $$ = create_node("Double", NULL); }
     ;

MethodHeader : Type IDENTIFIER LPAR OptFormalParams RPAR {
                $$ = create_node("MethodHeader", NULL);
                add_child($$, $1);
                add_sibling($1, create_node("Identifier", $2));
                
                Node* params = create_node("MethodParams", NULL);
                if ($4) add_child(params, $4);
                add_sibling($1, params);
             }
             | VOID IDENTIFIER LPAR OptFormalParams RPAR {
                $$ = create_node("MethodHeader", NULL);
                Node* v = create_node("Void", NULL);
                add_child($$, v);
                add_sibling(v, create_node("Identifier", $2));
                
                Node* params = create_node("MethodParams", NULL);
                if ($4) add_child(params, $4);
                add_sibling(v, params);
             }
             ;

OptFormalParams : { $$ = NULL; }
                | FormalParams { $$ = $1; }
                ;

FormalParams : Type IDENTIFIER FormalParamsRest {
                $$ = create_node("ParamDecl", NULL);
                add_child($$, $1);
                add_sibling($1, create_node("Identifier", $2));
                if ($3) add_sibling($$, $3);
             }
             | STRING LSQ RSQ IDENTIFIER {
                $$ = create_node("ParamDecl", NULL);
                Node* strArray = create_node("StringArray", NULL);
                add_child($$, strArray);
                add_sibling(strArray, create_node("Identifier", $4));
             }
             ;

FormalParamsRest : { $$ = NULL; }
                 | FormalParamsRest COMMA Type IDENTIFIER {
                    Node* param = create_node("ParamDecl", NULL);
                    add_child(param, $3);
                    add_sibling($3, create_node("Identifier", $4));
                    $$ = add_sibling($1, param);
                 }
                 ;

MethodBody : LBRACE StatementAndVarDeclList RBRACE {
                $$ = create_node("MethodBody", NULL);
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
                  $$ = create_node("Block", NULL);
                  add_child($$, $2);
              } else if ($2 != NULL) {
                  $$ = $2; 
              } else {
                  $$ = NULL; 
              }
          }
          | IF LPAR AExpr RPAR Statement %prec IF_WITHOUT_ELSE {
              $$ = create_node("If", NULL);
              add_child($$, $3); 
              add_child($$, $5 ? $5 : create_node("Block", NULL)); 
              add_child($$, create_node("Block", NULL)); 
          }
          | IF LPAR AExpr RPAR Statement ELSE Statement {
              $$ = create_node("If", NULL);
              add_child($$, $3);
              add_child($$, $5 ? $5 : create_node("Block", NULL));
              add_child($$, $7 ? $7 : create_node("Block", NULL));
          }
          | WHILE LPAR AExpr RPAR Statement {
              $$ = create_node("While", NULL);
              add_child($$, $3);
              add_child($$, $5 ? $5 : create_node("Block", NULL));
          }
          | RETURN OptExpr SEMICOLON {
              $$ = create_node("Return", NULL);
              if ($2) add_child($$, $2);
          }
          | MethodInvocation SEMICOLON { $$ = $1; }
          | Assignment SEMICOLON { $$ = $1; }
          | ParseArgs SEMICOLON { $$ = $1; }
          | PRINT LPAR ExprOrStrlit RPAR SEMICOLON {
              $$ = create_node("Print", NULL);
              add_child($$, $3);
          }
          | error SEMICOLON { $$ = NULL; }
          | SEMICOLON { $$ = NULL; }
          ;

OptExpr : { $$ = NULL; }
        | AExpr { $$ = $1; }
        ;

ExprOrStrlit : AExpr { $$ = $1; }
             | STRLIT { $$ = create_node("StrLit", $1); }
             ;

MethodInvocation : IDENTIFIER LPAR OptExprList RPAR {
                     $$ = create_node("Call", NULL);
                     Node* id = create_node("Identifier", $1);
                     add_child($$, id);
                     if ($3) add_sibling(id, $3);
                 }
                 | IDENTIFIER LPAR error RPAR { $$ = NULL; }
                 ;

OptExprList : { $$ = NULL; }
            | ExprList { $$ = $1; }
            ;

ExprList : AExpr { $$ = $1; }
         | ExprList COMMA AExpr { $$ = add_sibling($1, $3); }
         ;

Assignment : IDENTIFIER ASSIGN AExpr {
               $$ = create_node("Assign", NULL);
               Node* id = create_node("Identifier", $1);
               add_child($$, id);
               add_sibling(id, $3);
           }
           ;

AExpr : IDENTIFIER ASSIGN AExpr {
            $$ = create_node("Assign", NULL);
            Node* id = create_node("Identifier", $1);
            add_child($$, id);
            add_sibling(id, $3);
        }
      | Expr { $$ = $1; }
      ;

ParseArgs : PARSEINT LPAR IDENTIFIER LSQ AExpr RSQ RPAR {
              $$ = create_node("ParseArgs", NULL);
              Node* id = create_node("Identifier", $3);
              add_child($$, id);
              add_sibling(id, $5);
          }
          | PARSEINT LPAR error RPAR { $$ = NULL; }
          ;

Expr : Expr PLUS Expr   { $$ = create_node("Add", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr MINUS Expr  { $$ = create_node("Sub", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr STAR Expr   { $$ = create_node("Mul", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr DIV Expr    { $$ = create_node("Div", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr MOD Expr    { $$ = create_node("Mod", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr AND Expr    { $$ = create_node("And", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr OR Expr     { $$ = create_node("Or", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr XOR Expr    { $$ = create_node("Xor", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr LSHIFT Expr { $$ = create_node("Lshift", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr RSHIFT Expr { $$ = create_node("Rshift", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr EQ Expr     { $$ = create_node("Eq", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr GE Expr     { $$ = create_node("Ge", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr GT Expr     { $$ = create_node("Gt", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr LE Expr     { $$ = create_node("Le", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr LT Expr     { $$ = create_node("Lt", NULL); add_child($$, $1); add_child($$, $3); }
     | Expr NE Expr     { $$ = create_node("Ne", NULL); add_child($$, $1); add_child($$, $3); }
     | MINUS Expr %prec UNARY_MINUS { $$ = create_node("Minus", NULL); add_child($$, $2); }
     | PLUS Expr %prec UNARY_PLUS   { $$ = create_node("Plus", NULL); add_child($$, $2); }
     | NOT Expr         { $$ = create_node("Not", NULL); add_child($$, $2); }
     | LPAR AExpr RPAR  { $$ = $2; }
     | LPAR error RPAR  { $$ = NULL; }
     | MethodInvocation { $$ = $1; }
     | ParseArgs        { $$ = $1; }
     | IDENTIFIER OptDotLength {
         if ($2) {
             $$ = create_node("Length", NULL);
             add_child($$, create_node("Identifier", $1));
         } else {
             $$ = create_node("Identifier", $1);
         }
     }
     | NATURAL          { $$ = create_node("Natural", $1); }
     | DECIMAL          { $$ = create_node("Decimal", $1); }
     | BOOLLIT          { $$ = create_node("BoolLit", $1); }
     ;

OptDotLength : { $$ = NULL; }
             | DOTLENGTH { $$ = create_node("DotLength", NULL); }
             ;

%%
void print_tables() {
    // Imprimir Tabela Global
    if (global_table) {
        printf("===== %s Symbol Table =====\n", global_table->name);
        Symbol* s = global_table->first;
        while (s) {
            if (strlen(s->param_types) > 0) { // Se for método tem params
                printf("%s\t%s\t%s\n", s->name, s->param_types, s->type);
            } else { // Se for field
                printf("%s\t\t%s\n", s->name, s->type);
            }
            s = s->next;
        }
        printf("\n");
    }

    // Imprimir Tabelas Locais
    SymTable* t = local_tables_head;
    while (t) {
        printf("===== %s Symbol Table =====\n", t->name);
        Symbol* s = t->first;
        while (s) {
            if (s->is_param) {
                printf("%s\t\t%s\tparam\n", s->name, s->type);
            } else {
                printf("%s\t\t%s\n", s->name, s->type);
            }
            s = s->next;
        }
        printf("\n"); // Linha em branco entre tabelas
        t = t->next;
    }
}

int get_error_col() { return col - yyleng; }

void yyerror(char *s) {
    syntax_errors_count++;
    if (yychar == 0) {
        printf("Line %d, col %d: %s: \n", line, col, s);
    } 
    else if (yychar == STRLIT) {
        printf("Line %d, col %d: %s: %s\n", str_line, str_col, s, yylval.str);
    } 
    else {
        printf("Line %d, col %d: %s: %s\n", token_line, token_col, s, yytext);
    }
}
void print_tree(Node* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("..");
    if (node->value) printf("%s(%s)\n", node->type, node->value);
    else printf("%s\n", node->type);
    
    Node* child = node->child;
    while (child) {
        print_tree(child, depth + 1);
        child = child->sibling;
    }
}

int main(int argc, char *argv[]) {
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
        } else if (strcmp(argv[1], "-s") == 0) {      // <-- ADD THIS
            flag_semantic = 1;                        // <-- ADD THIS
        } else if (strcmp(argv[1], "-e3") == 0) {     // <-- ADD THIS
            flag_semantic_errors = 1;                 // <-- ADD THIS
        }
    }
    yyparse();
    
    // Check syntax and print tree
    if (flag_syntax && syntax_errors_count == 0) {
        print_tree(root, 0);
    }
    
    // Check semantics, generate and print tables
    if (flag_semantic && syntax_errors_count == 0) { 
        parse_method_declarations(root);
        print_tables();
        // Depois irás imprimir a AST anotada se for caso disso
    }
    
    return 0;
}