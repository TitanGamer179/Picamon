/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int flag_lex = 0;
int flag_syntax = 0;
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
          | IF LPAR Expr RPAR Statement %prec IF_WITHOUT_ELSE {
              $$ = create_node("If", NULL);
              add_child($$, $3); 
              add_child($$, $5 ? $5 : create_node("Block", NULL)); 
              add_child($$, create_node("Block", NULL)); 
          }
          | IF LPAR Expr RPAR Statement ELSE Statement {
              $$ = create_node("If", NULL);
              add_child($$, $3);
              add_child($$, $5 ? $5 : create_node("Block", NULL));
              add_child($$, $7 ? $7 : create_node("Block", NULL));
          }
          | WHILE LPAR Expr RPAR Statement {
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
        | Expr { $$ = $1; }
        ;

ExprOrStrlit : Expr { $$ = $1; }
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

ExprList : Expr { $$ = $1; }
         | ExprList COMMA Expr { $$ = add_sibling($1, $3); }
         ;

Assignment : IDENTIFIER ASSIGN Expr {
               $$ = create_node("Assign", NULL);
               Node* id = create_node("Identifier", $1);
               add_child($$, id);
               add_sibling(id, $3);
           }
           ;

ParseArgs : PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR {
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
     | LPAR Expr RPAR   { $$ = $2; }
     | LPAR error RPAR  { $$ = NULL; }
     | MethodInvocation { $$ = $1; }
     | Assignment       { $$ = $1; }
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
int get_error_col() { return col - yyleng; }

void yyerror(char *s) {
    syntax_errors_count++;
    if (yychar == 0) {
        printf("Line %d, col %d: %s: \"end of file\"\n", line, get_error_col(), s);
    } 
    else {
        printf("Line %d, col %d: %s: %s\n", line, get_error_col(), s, yytext);
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
        if (strcmp(argv[1], "-1") == 0) {
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
        }
    }
    yyparse();
    if (flag_syntax && syntax_errors_count == 0) {
        print_tree(root, 0);
    }
    return 0;
}