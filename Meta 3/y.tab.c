/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 3 "jucompiler.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int flag_lex = 0;
int flag_syntax = 0;
int syntax_errors_count = 0;

extern int line, col;
extern int token_line, token_col;
extern char* yytext;
extern int yyleng;
extern int yychar;
extern int str_line, str_col;

int yylex(void);
void yyerror(char *s);

typedef struct _symbol Symbol;
typedef struct _table SymTable;
typedef struct _node Node;

char* lookup_variable_type(SymTable *table, char *name);
void annotate_ast(Node *node, SymTable *current_env);
int is_expression(char *type);

typedef struct _symbol {
    char *name;
    char *type;
    char *param_types; 
    int is_param;
    int line;
    int col;
    struct _symbol *next;
} Symbol;

typedef struct _table {
    char *name;
    char *type;
    // "Global" or "Method"
    Symbol *symbols;
    struct _table *next;
} SymTable;

typedef struct _node {
    char *type;
    char *value;
    int line;
    int col;
    char *anot_type;
    struct _node *child;
    struct _node *sibling;
} Node;

Node* root = NULL; 
SymTable *global_table = NULL;
SymTable *all_tables = NULL;

Node* create_node(char *type, char *value, int line, int col) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->type = type ? strdup(type) : NULL;
    new_node->value = value ? strdup(value) : NULL;
    new_node->line = line;
    new_node->col = col;
    new_node->anot_type = NULL;
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

SymTable* create_table(char *name, char *type) {
    SymTable *t = (SymTable*)malloc(sizeof(SymTable));
    t->name = strdup(name);
    t->type = strdup(type);
    t->symbols = NULL;
    t->next = NULL;
    if (!all_tables) all_tables = t;
    else {
        SymTable *curr = all_tables;
        while (curr->next) curr = curr->next;
        curr->next = t;
    }
    return t;
}

void clear_anot(Node *node) {
    if (!node) return;
    if (node->anot_type) {
        free(node->anot_type);
        node->anot_type = NULL;
    }
    Node *c = node->child;
    while (c) {
        clear_anot(c);
        c = c->sibling;
    }
}

Symbol* create_symbol(char *name, char *type, char *param_types, int is_param, int line, int col) {
    Symbol *s = (Symbol*)malloc(sizeof(Symbol));
    s->name = name ? strdup(name) : strdup("");
    s->type = type ? strdup(type) : strdup("undef");
    s->param_types = param_types ? strdup(param_types) : NULL;
    s->is_param = is_param;
    s->line = line;
    s->col = col;
    s->next = NULL;
    return s;
}

int insert_symbol(SymTable *table, char *name, char *type, char *param_types, int is_param, int line, int col) {
    if (!table) return 0;
    
    Symbol *curr = table->symbols;
    Symbol *prev = NULL;
    while (curr) {
        if (strcmp(curr->name, name) == 0) { 
            if ((!param_types && !curr->param_types) || (param_types && curr->param_types && strcmp(curr->param_types, param_types) == 0)) {
                if (param_types)
                    printf("Line %d, col %d: Symbol %s(%s) already defined\n", line, col, name, param_types);
                else
                    printf("Line %d, col %d: Symbol %s already defined\n", line, col, name);
                return 0;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    
    Symbol *new_s = create_symbol(name, type, param_types, is_param, line, col);
    if (!table->symbols) table->symbols = new_s;
    else prev->next = new_s;
    return 1;
}

char* lookup_variable_type(SymTable *table, char *name) {
    if (!table || !name) return "undef";
    Symbol *curr = table->symbols;
    while (curr) {
        if (strcmp(curr->name, name) == 0 && !curr->param_types) return curr->type;
        curr = curr->next;
    }
    if (table != global_table) return lookup_variable_type(global_table, name);
    return "undef";
}

char* get_juc_type(char *type) {
    if (!type) return "undef";
    if (strcmp(type, "Bool") == 0) return "boolean";
    if (strcmp(type, "Int") == 0) return "int";
    if (strcmp(type, "Double") == 0) return "double";
    if (strcmp(type, "Void") == 0) return "void";
    if (strcmp(type, "StringArray") == 0) return "String[]";
    return type;
}

int is_expression(char *type) {
    if (!type) return 0;
    char *exprs[] = {"Assign", "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", 
                 "Add", "Sub", "Mul", "Div", "Mod", "Xor", "Lshift", "Rshift",
                 "Not", "Minus", "Plus", "Length", "Call", "ParseArgs", 
                 "Identifier", "Natural", "Decimal", "BoolLit", "StrLit", NULL};
    for (int i = 0; exprs[i] != NULL; i++) {
        if (strcmp(type, exprs[i]) == 0) return 1;
    }
    return 0;
}

char* build_param_types(Node *params_node) {
    if (!params_node || !params_node->child) return strdup("");
    char buf[8192] = ""; // Expanded buffer to prevent segmentation fault
    Node *curr = params_node->child;
    while (curr) {
        if (curr->child) {
            strcat(buf, get_juc_type(curr->child->type));
            if (curr->sibling) strcat(buf, ",");
        }
        curr = curr->sibling;
    }
    return strdup(buf);
}

char* get_op_str(char *type) {
    if (!type) return "";
    if (strcmp(type, "Add") == 0) return "+";
    if (strcmp(type, "Sub") == 0) return "-";
    if (strcmp(type, "Mul") == 0) return "*";
    if (strcmp(type, "Div") == 0) return "/";
    if (strcmp(type, "Mod") == 0) return "%";
    if (strcmp(type, "Eq") == 0) return "==";
    if (strcmp(type, "Ne") == 0) return "!=";
    if (strcmp(type, "Lt") == 0) return "<";
    if (strcmp(type, "Gt") == 0) return ">";
    if (strcmp(type, "Le") == 0) return "<=";
    if (strcmp(type, "Ge") == 0) return ">=";
    if (strcmp(type, "And") == 0) return "&&";
    if (strcmp(type, "Or") == 0) return "||";
    if (strcmp(type, "Xor") == 0) return "^";
    if (strcmp(type, "Lshift") == 0) return "<<";
    if (strcmp(type, "Rshift") == 0) return ">>";
    return "";
}

void annotate_ast(Node *node, SymTable *current_env) {
    if (!node) return;
    
    if (strcmp(node->type, "Identifier") == 0) {
        // 5. ADICIONAR AQUI VERIFICAÇÃO NO USO DO IDENTIFICADOR:
        if (strcmp(node->value, "_") == 0) {
            printf("Line %d, col %d: Symbol _ is reserved\n", node->line, node->col);
            node->anot_type = strdup("undef");
            return;
        }
        char *type = lookup_variable_type(current_env, node->value);
        if (strcmp(type, "undef") == 0) printf("Line %d, col %d: Cannot find symbol %s\n", node->line, node->col, node->value);
        node->anot_type = strdup(type);
        return;
    } else if (strcmp(node->type, "Natural") == 0) {
        node->anot_type = strdup("int");
        char *clean = (char*)malloc(strlen(node->value) + 1);
        int j = 0;
        for (int i = 0; node->value[i]; i++) if (node->value[i] != '_') clean[j++] = node->value[i];
        clean[j] = '\0';
        unsigned long long val = strtoull(clean, NULL, 10);
        if (val > 2147483647ULL) printf("Line %d, col %d: Number %s out of bounds\n", node->line, node->col, node->value);
        free(clean);
        return;
    } else if (strcmp(node->type, "Decimal") == 0) {
        node->anot_type = strdup("double");
        char *clean = (char*)malloc(strlen(node->value) + 1);
        int j = 0;
        for (int i = 0; node->value[i]; i++) if (node->value[i] != '_') clean[j++] = node->value[i];
        clean[j] = '\0';
        errno = 0;
        double val = strtod(clean, NULL);
        if (errno == ERANGE && (val == 0.0 || val >= 1.0e308 || val <= -1.0e308)) {
            printf("Line %d, col %d: Number %s out of bounds\n", node->line, node->col, node->value);
        }
        free(clean);
        return;
    } else if (strcmp(node->type, "BoolLit") == 0) {
        node->anot_type = strdup("boolean");
        return;
    } else if (strcmp(node->type, "StrLit") == 0) {
        node->anot_type = strdup("String");
        return;
    } else if (strcmp(node->type, "VarDecl") == 0 || strcmp(node->type, "FieldDecl") == 0) {
        if (node->child && node->child->sibling) {
            Node *id_node = node->child->sibling;
            // 6. ADICIONAR AQUI PARA VARIÁVEIS LOCAIS:
            if (strcmp(id_node->value, "_") == 0) {
                printf("Line %d, col %d: Symbol _ is reserved\n", id_node->line, id_node->col);
            } else {
                insert_symbol(current_env, id_node->value, get_juc_type(node->child->type), NULL, 0, id_node->line, id_node->col);
            }
        }
        return;
    }

    if (strcmp(node->type, "Program") == 0) {
        if (node->child && strcmp(node->child->value, "_") == 0) {
            printf("Line %d, col %d: Symbol _ is reserved\n", node->child->line, node->child->col);
        }
        global_table = create_table("Class", "Global");
        Node *child = node->child ? node->child->sibling : NULL;
        Node *temp = child;
        
        while (temp) {
            if (strcmp(temp->type, "MethodDecl") == 0) {
                Node *header = temp->child;
                Node *id_node = header->child->sibling;
                char *params_str = build_param_types(id_node->sibling);
                
                int is_duplicate = 0;
                if (strcmp(id_node->value, "_") == 0) {
                    printf("Line %d, col %d: Symbol _ is reserved\n", id_node->line, id_node->col);
                    is_duplicate = 1; 
                } else {
                    Symbol *s_curr = global_table->symbols;
                    while (s_curr) {
                        if (strcmp(s_curr->name, id_node->value) == 0 && s_curr->param_types && strcmp(s_curr->param_types, params_str) == 0) {
                            is_duplicate = 1;
                            break;
                        }
                        s_curr = s_curr->next;
                    }
                }

                SymTable *method_table = NULL;
                if (!is_duplicate) {
                    char table_name[8192];
                    snprintf(table_name, sizeof(table_name), "Method %s(%s)", id_node->value, params_str);
                    method_table = create_table(table_name, "Method");
                    // Inserir na global_table IMEDIATAMENTE (tal como o código do teu amigo faz)
                    insert_symbol(global_table, id_node->value, get_juc_type(header->child->type), params_str, 0, id_node->line, id_node->col);
                } else {
                    method_table = (SymTable*)malloc(sizeof(SymTable));
                    method_table->name = "TMP"; method_table->type = "Tmp"; method_table->symbols = NULL; method_table->next = NULL;
                }
                
                // 3. Inserir return e parâmetros na tabela
                insert_symbol(method_table, "return", get_juc_type(header->child->type), NULL, 0, id_node->line, id_node->col);
                
                if (strcmp(id_node->value, "_") != 0) { // IGNORA PARAMETROS DE METODOS RESERVADOS
                    Node *params = id_node->sibling->child;
                    while (params) {
                        if (strcmp(params->child->sibling->value, "_") == 0) {
                            printf("Line %d, col %d: Symbol _ is reserved\n", params->child->sibling->line, params->child->sibling->col);
                        } else {
                            insert_symbol(method_table, params->child->sibling->value, get_juc_type(params->child->type), NULL, 1, params->child->sibling->line, params->child->sibling->col);
                        }
                        params = params->sibling;
                    }
                }

                // 4. Só NO FIM imprime o erro do método duplicado e liberta a TMP
                if (is_duplicate) {
                    if (strcmp(id_node->value, "_") != 0) { // <-- NÃO IMPRIME ALREADY DEFINED PARA O _
                        printf("Line %d, col %d: Symbol %s(%s) already defined\n", id_node->line, id_node->col, id_node->value, params_str);
                    }
                    Symbol *cs = method_table->symbols;
                    while (cs) {
                        Symbol *next = cs->next;
                        if (cs->name) free(cs->name);
                        if (cs->type) free(cs->type);
                        if (cs->param_types) free(cs->param_types);
                        free(cs);
                        cs = next;
                    }
                    free(method_table);
                }
                free(params_str);

            } else if (strcmp(temp->type, "FieldDecl") == 0) { 
                Node *id_node = temp->child->sibling;
                if (strcmp(id_node->value, "_") == 0) {
                    printf("Line %d, col %d: Symbol _ is reserved\n", id_node->line, id_node->col);
                } else {
                    insert_symbol(global_table, id_node->value, get_juc_type(temp->child->type), NULL, 0, id_node->line, id_node->col);
                }
            }
            temp = temp->sibling;
        }
        
        temp = child;
        while (temp) {
            if (strcmp(temp->type, "MethodDecl") == 0) annotate_ast(temp, global_table);
            temp = temp->sibling;
        }
        return;
    }

    if (strcmp(node->type, "MethodDecl") == 0) {
        Node *header = node->child;
        Node *id_node = header->child->sibling;
        char *params_str = build_param_types(id_node->sibling);
        char table_name[8192];
        snprintf(table_name, sizeof(table_name), "Method %s(%s)", id_node->value, params_str);
        
        int redefined = 0;
        Symbol *s = global_table->symbols;
        while (s) {
            if (strcmp(s->name, id_node->value) == 0 && s->param_types && strcmp(s->param_types, params_str) == 0) {
                // Descobre se este nó é o método duplicado e não o original
                if (s->line < id_node->line || (s->line == id_node->line && s->col < id_node->col)) {
                    redefined = 1;
                    break;
                }
            }
            s = s->next;
        }
        free(params_str);
        
        if (redefined) return; // Salta o interior de métodos duplicados!

        SymTable *method_table = all_tables;
        while (method_table) {
            if (strcmp(method_table->name, table_name) == 0) break;
            method_table = method_table->next;
        }
        
        if (method_table && node->child->sibling) {
            annotate_ast(node->child->sibling, method_table);
        }
        return;
    }

    Node *child = node->child;
    while (child) {
        if (strcmp(node->type, "Call") == 0 && child == node->child) {
            if (strcmp(child->value, "_") == 0) {
                printf("Line %d, col %d: Symbol _ is reserved\n", child->line, child->col);
            }
        } else {
            annotate_ast(child, current_env);
        }
        child = child->sibling;
    }

    if (strcmp(node->type, "Assign") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        int valid = 0;
        if (strcmp(t1, "double") == 0 && (strcmp(t2, "double") == 0 || strcmp(t2, "int") == 0)) valid = 1;
        else if (strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) valid = 1;
        else if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) valid = 1;
        if (!valid) printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", node->line, node->col, t1, t2);
        node->anot_type = strdup(t1);
    } else if (strcmp(node->type, "Add") == 0 || strcmp(node->type, "Sub") == 0 || strcmp(node->type, "Mul") == 0 || strcmp(node->type, "Div") == 0 || strcmp(node->type, "Mod") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        int valid = 0;
        char *res = "undef";
        
        if ((strcmp(t1, "double") == 0 || strcmp(t2, "double") == 0) && (strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) && (strcmp(t2, "int") == 0 || strcmp(t2, "double") == 0)) {
            valid = 1;
            res = "double";
        } else if (strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) {
            valid = 1;
            res = "int";
        }
        
        if (!valid) printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, get_op_str(node->type), t1, t2);
        node->anot_type = strdup(res);
    } else if (strcmp(node->type, "And") == 0 || strcmp(node->type, "Or") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        int valid = 0;
        if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) { 
            valid = 1; 
        }
        if (!valid) {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, get_op_str(node->type), t1, t2);
        }
        node->anot_type = strdup("boolean");
    } else if (strcmp(node->type, "Xor") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        if ((strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0)) {
            node->anot_type = strdup("int");
        } else if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) {
            node->anot_type = strdup("boolean");
        } else {
            printf("Line %d, col %d: Operator ^ cannot be applied to types %s, %s\n", node->line, node->col, t1, t2);
            node->anot_type = strdup("undef");
        }
    } else if (strcmp(node->type, "Lshift") == 0 || strcmp(node->type, "Rshift") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        
        if (strcmp(t1, "int") != 0 || strcmp(t2, "int") != 0) {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, strcmp(node->type, "Lshift") == 0 ? "<<" : ">>", t1, t2);
            node->anot_type = strdup("undef");
        } else {
            node->anot_type = strdup("int");
        }
    } else if (strcmp(node->type, "Eq") == 0 || strcmp(node->type, "Ne") == 0 || strcmp(node->type, "Lt") == 0 || strcmp(node->type, "Gt") == 0 || strcmp(node->type, "Le") == 0 || strcmp(node->type, "Ge") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        int valid = 0;
        if (strcmp(t1, t2) == 0 && (strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0 || strcmp(t1, "boolean") == 0)) {
            if (strcmp(t1, "boolean") == 0 && (strcmp(node->type, "Eq") != 0 && strcmp(node->type, "Ne") != 0)) valid = 0;
            else valid = 1;
        } else if ((strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) && (strcmp(t2, "int") == 0 || strcmp(t2, "double") == 0)) valid = 1;
        if (!valid) {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, get_op_str(node->type), t1, t2);
        }
        node->anot_type = strdup("boolean");
    } else if (strcmp(node->type, "Plus") == 0 || strcmp(node->type, "Minus") == 0) {
        char *t1 = (node->child && node->child->anot_type) ? node->child->anot_type : "none";
        if (strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) node->anot_type = strdup(t1);
        else {
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n", node->line, node->col, strcmp(node->type, "Plus") == 0 ? "+" : "-", t1);
            node->anot_type = strdup("undef");
        }
    } else if (strcmp(node->type, "Not") == 0) {
        char *t1 = (node->child && node->child->anot_type) ? node->child->anot_type : "none";
        
        if (strcmp(t1, "boolean") != 0) {
            printf("Line %d, col %d: Operator ! cannot be applied to type %s\n", node->line, node->col, t1);
        }
        node->anot_type = strdup("boolean");
    } else if (strcmp(node->type, "Length") == 0) {
        char *t1 = (node->child && node->child->anot_type) ? node->child->anot_type : "none";
        
        if (strcmp(t1, "String[]") != 0) {
            printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", node->line, node->col, t1);
        }
        node->anot_type = strdup("int");
    } else if (strcmp(node->type, "Call") == 0) {
        char *method_name = node->child->value;
        Node *arg = node->child->sibling;
        
        char arg_types[512][64]; // Prevenção de Segfault de limit tests
        int num_args = 0;
        char arg_sig[8192] = ""; 
        
        while (arg && num_args < 512) {
            strcpy(arg_types[num_args], arg->anot_type ? arg->anot_type : "none");
            if (num_args > 0) strcat(arg_sig, ",");
            strcat(arg_sig, arg_types[num_args]);
            num_args++;
            arg = arg->sibling;
        }
        
        Symbol *exact_match = NULL;
        Symbol *compat_matches[512];
        int compat_count = 0;
        Symbol *sym = global_table->symbols;
        while (sym) {
            if (strcmp(sym->name, method_name) == 0 && sym->param_types != NULL) {
                char formal_types[512][64];
                int num_formals = 0;
                if (strlen(sym->param_types) > 0) {
                    char *p_copy = strdup(sym->param_types);
                    char *tok = strtok(p_copy, ",");
                    while (tok && num_formals < 512) {
                        strcpy(formal_types[num_formals++], tok);
                        tok = strtok(NULL, ",");
                    }
                    free(p_copy);
                }
                
                if (num_args == num_formals) {
                    int exact = 1;
                    int compat = 1;
                    for (int i = 0; i < num_args; i++) {
                        if (strcmp(arg_types[i], formal_types[i]) != 0) {
                            exact = 0;
                            if (!(strcmp(arg_types[i], "int") == 0 && strcmp(formal_types[i], "double") == 0)) {
                                compat = 0;
                            }
                        }
                    }
                    if (exact) { exact_match = sym; break; }
                    else if (compat) { compat_matches[compat_count++] = sym; }
                }
            }
            sym = sym->next;
        }
        
        int el = node->child ? node->child->line : node->line;
        int ec = node->child ? node->child->col : node->col;

        if (exact_match) {
            node->anot_type = strdup(exact_match->type);
            char sig_form[8192];
            sprintf(sig_form, "(%s)", exact_match->param_types ? exact_match->param_types : "");
            node->child->anot_type = strdup(sig_form);
        } else if (compat_count == 1) {
            node->anot_type = strdup(compat_matches[0]->type);
            char sig_form[8192];
            sprintf(sig_form, "(%s)", compat_matches[0]->param_types ? compat_matches[0]->param_types : "");
            node->child->anot_type = strdup(sig_form);
        } else if (compat_count > 1) {
            printf("Line %d, col %d: Reference to method %s(%s) is ambiguous\n", el, ec, method_name, arg_sig);
            node->anot_type = strdup("undef");
            node->child->anot_type = strdup("undef");
        } else {
            printf("Line %d, col %d: Cannot find symbol %s(%s)\n", el, ec, method_name, arg_sig);
            node->anot_type = strdup("undef");
            node->child->anot_type = strdup("undef");
        }
    } else if (strcmp(node->type, "ParseArgs") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        
        if (strcmp(t1, "String[]") != 0 || strcmp(t2, "int") != 0) {
            printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", node->line, node->col, t1, t2);
        }
        node->anot_type = strdup("int");
    } else if (strcmp(node->type, "Return") == 0) {
        char *expected = lookup_variable_type(current_env, "return");
        char *actual = node->child ? (node->child->anot_type ? node->child->anot_type : "none") : "none";
        int valid = 0;
        if (strcmp(expected, "void") == 0) {
            if (node->child == NULL) valid = 1;
        } else if (strcmp(expected, "double") == 0) {
            if (strcmp(actual, "int") == 0 || strcmp(actual, "double") == 0) valid = 1;
        } else if (strcmp(actual, expected) == 0) {
            valid = 1;
        }
        
        if (!valid) {
            int el = node->child ? node->child->line : node->line;
            int ec = node->child ? node->child->col : node->col;
            if (strcmp(expected, "void") == 0 && node->child != NULL) printf("Line %d, col %d: Incompatible type %s in return statement\n", el, ec, actual);
            else if (strcmp(expected, "void") != 0 && node->child == NULL) printf("Line %d, col %d: Incompatible type void in return statement\n", el, ec);
            else printf("Line %d, col %d: Incompatible type %s in return statement\n", el, ec, actual);
        }
    } else if (strcmp(node->type, "Print") == 0) {
        char *t1 = (node->child && node->child->anot_type) ? node->child->anot_type : "none";
        if (strcmp(t1, "int") != 0 && strcmp(t1, "double") != 0 && strcmp(t1, "boolean") != 0 && strcmp(t1, "String") != 0) {
            int el = node->child ? node->child->line : node->line;
            int ec = node->child ? node->child->col : node->col;
            printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n", el, ec, t1);
        }
    } else if (strcmp(node->type, "If") == 0 || strcmp(node->type, "While") == 0) {
        char *t1 = (node->child && node->child->anot_type) ? node->child->anot_type : "none";
        if (strcmp(t1, "boolean") != 0) {
            int el = node->child ? node->child->line : node->line;
            int ec = node->child ? node->child->col : node->col;
            printf("Line %d, col %d: Incompatible type %s in %s statement\n", el, ec, t1, strcmp(node->type, "If") == 0 ? "if" : "while");
        }
    }
}

void print_tables() {
    SymTable *t = all_tables;
    while (t) {
        if (strcmp(t->type, "Global") == 0 && root && root->child) printf("===== %s %s Symbol Table =====\n", t->name, root->child->value);
        else printf("===== %s Symbol Table =====\n", t->name);
        
        Symbol *s = t->symbols;
        while (s) {
            if (s->param_types) printf("%s\t(%s)\t%s\n", s->name, s->param_types, s->type);
            else printf("%s\t\t%s%s\n", s->name, s->type, s->is_param ? "\tparam" : ""); // <- RESTAURADOS OS DOIS \t
            s = s->next;
        }
        printf("\n");
        t = t->next;
    }
}


#line 735 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BOOL = 258,                    /* BOOL  */
    CLASS = 259,                   /* CLASS  */
    DOTLENGTH = 260,               /* DOTLENGTH  */
    DOUBLE = 261,                  /* DOUBLE  */
    ELSE = 262,                    /* ELSE  */
    IF = 263,                      /* IF  */
    INT = 264,                     /* INT  */
    PRINT = 265,                   /* PRINT  */
    PARSEINT = 266,                /* PARSEINT  */
    PUBLIC = 267,                  /* PUBLIC  */
    RETURN = 268,                  /* RETURN  */
    STATIC = 269,                  /* STATIC  */
    STRING = 270,                  /* STRING  */
    VOID = 271,                    /* VOID  */
    WHILE = 272,                   /* WHILE  */
    AND = 273,                     /* AND  */
    ASSIGN = 274,                  /* ASSIGN  */
    STAR = 275,                    /* STAR  */
    COMMA = 276,                   /* COMMA  */
    DIV = 277,                     /* DIV  */
    EQ = 278,                      /* EQ  */
    GE = 279,                      /* GE  */
    GT = 280,                      /* GT  */
    LBRACE = 281,                  /* LBRACE  */
    LE = 282,                      /* LE  */
    LPAR = 283,                    /* LPAR  */
    LSQ = 284,                     /* LSQ  */
    LT = 285,                      /* LT  */
    MINUS = 286,                   /* MINUS  */
    MOD = 287,                     /* MOD  */
    NE = 288,                      /* NE  */
    NOT = 289,                     /* NOT  */
    OR = 290,                      /* OR  */
    PLUS = 291,                    /* PLUS  */
    RBRACE = 292,                  /* RBRACE  */
    RPAR = 293,                    /* RPAR  */
    RSQ = 294,                     /* RSQ  */
    SEMICOLON = 295,               /* SEMICOLON  */
    ARROW = 296,                   /* ARROW  */
    LSHIFT = 297,                  /* LSHIFT  */
    RSHIFT = 298,                  /* RSHIFT  */
    XOR = 299,                     /* XOR  */
    IDENTIFIER = 300,              /* IDENTIFIER  */
    NATURAL = 301,                 /* NATURAL  */
    DECIMAL = 302,                 /* DECIMAL  */
    STRLIT = 303,                  /* STRLIT  */
    BOOLLIT = 304,                 /* BOOLLIT  */
    RESERVED = 305,                /* RESERVED  */
    IF_WITHOUT_ELSE = 306,         /* IF_WITHOUT_ELSE  */
    UNARY_PLUS = 307,              /* UNARY_PLUS  */
    UNARY_MINUS = 308              /* UNARY_MINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define BOOL 258
#define CLASS 259
#define DOTLENGTH 260
#define DOUBLE 261
#define ELSE 262
#define IF 263
#define INT 264
#define PRINT 265
#define PARSEINT 266
#define PUBLIC 267
#define RETURN 268
#define STATIC 269
#define STRING 270
#define VOID 271
#define WHILE 272
#define AND 273
#define ASSIGN 274
#define STAR 275
#define COMMA 276
#define DIV 277
#define EQ 278
#define GE 279
#define GT 280
#define LBRACE 281
#define LE 282
#define LPAR 283
#define LSQ 284
#define LT 285
#define MINUS 286
#define MOD 287
#define NE 288
#define NOT 289
#define OR 290
#define PLUS 291
#define RBRACE 292
#define RPAR 293
#define RSQ 294
#define SEMICOLON 295
#define ARROW 296
#define LSHIFT 297
#define RSHIFT 298
#define XOR 299
#define IDENTIFIER 300
#define NATURAL 301
#define DECIMAL 302
#define STRLIT 303
#define BOOLLIT 304
#define RESERVED 305
#define IF_WITHOUT_ELSE 306
#define UNARY_PLUS 307
#define UNARY_MINUS 308

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 667 "jucompiler.y"

    struct {
        char* str;
        int line;
        int col;
    } token;
    struct _node* node;

#line 903 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_BOOL = 3,                       /* BOOL  */
  YYSYMBOL_CLASS = 4,                      /* CLASS  */
  YYSYMBOL_DOTLENGTH = 5,                  /* DOTLENGTH  */
  YYSYMBOL_DOUBLE = 6,                     /* DOUBLE  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_IF = 8,                         /* IF  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_PRINT = 10,                     /* PRINT  */
  YYSYMBOL_PARSEINT = 11,                  /* PARSEINT  */
  YYSYMBOL_PUBLIC = 12,                    /* PUBLIC  */
  YYSYMBOL_RETURN = 13,                    /* RETURN  */
  YYSYMBOL_STATIC = 14,                    /* STATIC  */
  YYSYMBOL_STRING = 15,                    /* STRING  */
  YYSYMBOL_VOID = 16,                      /* VOID  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_AND = 18,                       /* AND  */
  YYSYMBOL_ASSIGN = 19,                    /* ASSIGN  */
  YYSYMBOL_STAR = 20,                      /* STAR  */
  YYSYMBOL_COMMA = 21,                     /* COMMA  */
  YYSYMBOL_DIV = 22,                       /* DIV  */
  YYSYMBOL_EQ = 23,                        /* EQ  */
  YYSYMBOL_GE = 24,                        /* GE  */
  YYSYMBOL_GT = 25,                        /* GT  */
  YYSYMBOL_LBRACE = 26,                    /* LBRACE  */
  YYSYMBOL_LE = 27,                        /* LE  */
  YYSYMBOL_LPAR = 28,                      /* LPAR  */
  YYSYMBOL_LSQ = 29,                       /* LSQ  */
  YYSYMBOL_LT = 30,                        /* LT  */
  YYSYMBOL_MINUS = 31,                     /* MINUS  */
  YYSYMBOL_MOD = 32,                       /* MOD  */
  YYSYMBOL_NE = 33,                        /* NE  */
  YYSYMBOL_NOT = 34,                       /* NOT  */
  YYSYMBOL_OR = 35,                        /* OR  */
  YYSYMBOL_PLUS = 36,                      /* PLUS  */
  YYSYMBOL_RBRACE = 37,                    /* RBRACE  */
  YYSYMBOL_RPAR = 38,                      /* RPAR  */
  YYSYMBOL_RSQ = 39,                       /* RSQ  */
  YYSYMBOL_SEMICOLON = 40,                 /* SEMICOLON  */
  YYSYMBOL_ARROW = 41,                     /* ARROW  */
  YYSYMBOL_LSHIFT = 42,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 43,                    /* RSHIFT  */
  YYSYMBOL_XOR = 44,                       /* XOR  */
  YYSYMBOL_IDENTIFIER = 45,                /* IDENTIFIER  */
  YYSYMBOL_NATURAL = 46,                   /* NATURAL  */
  YYSYMBOL_DECIMAL = 47,                   /* DECIMAL  */
  YYSYMBOL_STRLIT = 48,                    /* STRLIT  */
  YYSYMBOL_BOOLLIT = 49,                   /* BOOLLIT  */
  YYSYMBOL_RESERVED = 50,                  /* RESERVED  */
  YYSYMBOL_IF_WITHOUT_ELSE = 51,           /* IF_WITHOUT_ELSE  */
  YYSYMBOL_UNARY_PLUS = 52,                /* UNARY_PLUS  */
  YYSYMBOL_UNARY_MINUS = 53,               /* UNARY_MINUS  */
  YYSYMBOL_YYACCEPT = 54,                  /* $accept  */
  YYSYMBOL_AnyId = 55,                     /* AnyId  */
  YYSYMBOL_Program = 56,                   /* Program  */
  YYSYMBOL_ProgramElements = 57,           /* ProgramElements  */
  YYSYMBOL_MethodDecl = 58,                /* MethodDecl  */
  YYSYMBOL_FieldDecl = 59,                 /* FieldDecl  */
  YYSYMBOL_VarDecl = 60,                   /* VarDecl  */
  YYSYMBOL_IdList = 61,                    /* IdList  */
  YYSYMBOL_Type = 62,                      /* Type  */
  YYSYMBOL_MethodHeader = 63,              /* MethodHeader  */
  YYSYMBOL_OptFormalParams = 64,           /* OptFormalParams  */
  YYSYMBOL_FormalParams = 65,              /* FormalParams  */
  YYSYMBOL_FormalParamsRest = 66,          /* FormalParamsRest  */
  YYSYMBOL_MethodBody = 67,                /* MethodBody  */
  YYSYMBOL_StatementAndVarDeclList = 68,   /* StatementAndVarDeclList  */
  YYSYMBOL_StatementList = 69,             /* StatementList  */
  YYSYMBOL_Statement = 70,                 /* Statement  */
  YYSYMBOL_OptExpr = 71,                   /* OptExpr  */
  YYSYMBOL_ExprOrStrlit = 72,              /* ExprOrStrlit  */
  YYSYMBOL_MethodInvocation = 73,          /* MethodInvocation  */
  YYSYMBOL_OptExprList = 74,               /* OptExprList  */
  YYSYMBOL_ExprList = 75,                  /* ExprList  */
  YYSYMBOL_ParseArgs = 76,                 /* ParseArgs  */
  YYSYMBOL_Expr = 77                       /* Expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   575

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  82
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   308


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   704,   704,   707,   714,   715,   716,   717,   720,   727,
     742,   745,   762,   763,   766,   767,   768,   771,   780,   791,
     792,   795,   801,   809,   810,   818,   824,   825,   829,   832,
     833,   839,   849,   855,   861,   866,   870,   871,   872,   881,
     885,   886,   889,   890,   893,   894,   897,   903,   906,   907,
     910,   911,   914,   920,   923,   932,   933,   934,   935,   936,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     947,   948,   949,   950,   951,   952,   953,   954,   955,   956,
     957,   958,   959
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "BOOL", "CLASS",
  "DOTLENGTH", "DOUBLE", "ELSE", "IF", "INT", "PRINT", "PARSEINT",
  "PUBLIC", "RETURN", "STATIC", "STRING", "VOID", "WHILE", "AND", "ASSIGN",
  "STAR", "COMMA", "DIV", "EQ", "GE", "GT", "LBRACE", "LE", "LPAR", "LSQ",
  "LT", "MINUS", "MOD", "NE", "NOT", "OR", "PLUS", "RBRACE", "RPAR", "RSQ",
  "SEMICOLON", "ARROW", "LSHIFT", "RSHIFT", "XOR", "IDENTIFIER", "NATURAL",
  "DECIMAL", "STRLIT", "BOOLLIT", "RESERVED", "IF_WITHOUT_ELSE",
  "UNARY_PLUS", "UNARY_MINUS", "$accept", "AnyId", "Program",
  "ProgramElements", "MethodDecl", "FieldDecl", "VarDecl", "IdList",
  "Type", "MethodHeader", "OptFormalParams", "FormalParams",
  "FormalParamsRest", "MethodBody", "StatementAndVarDeclList",
  "StatementList", "Statement", "OptExpr", "ExprOrStrlit",
  "MethodInvocation", "OptExprList", "ExprList", "ParseArgs", "Expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-49)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-49)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       2,   -38,    12,   -49,    -6,   -49,   -49,    28,   -16,    13,
     -49,   -49,   -49,   -49,   -49,   106,   -49,   -49,   -49,   -38,
     -38,    15,     0,    11,   -49,   -49,     7,     7,   -17,   133,
      22,   -38,     5,   -49,     6,   -38,   -49,    16,    25,    27,
      34,   237,    38,   -49,   208,   237,   237,   237,   -49,   -49,
     -49,   -49,   -49,     3,   -49,   -38,   -49,    29,    30,   404,
      32,   -49,   -49,   -49,   -49,   -49,   237,   230,     4,    33,
     -49,   -49,   431,   237,   155,    36,   269,   -49,   -49,   -49,
     -49,    14,   -49,   -49,   -49,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   -38,    54,   296,   -49,    57,   431,    61,    75,
     -49,   237,   323,   -49,   -49,   -49,   -49,    69,    70,    99,
     431,    -7,   483,   350,   -49,   -49,   532,    74,    74,    74,
      74,   -11,   -49,   532,   458,   -11,   193,   193,   508,   -49,
      94,   186,    81,   -49,   237,   431,   186,   -49,   -49,   237,
     -49,   -49,   -38,   116,   -49,   377,   -49,   431,   -49,   186,
      86,   -49,   -49
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     0,     1,     4,     0,     0,     0,
       3,     7,     5,     6,    10,     0,    14,    16,    15,     0,
       0,     0,     0,    12,    26,     8,    19,    19,     0,     0,
       0,     0,     0,    20,     0,     0,     9,     0,     0,     0,
       0,    42,     0,    29,     0,     0,     0,     0,    25,    41,
      77,    78,    79,    76,    28,     0,    27,    74,    75,     0,
       0,    23,    18,    17,    13,    40,     0,     0,     0,     0,
      74,    75,    43,     0,     0,     0,     0,    72,    73,    71,
      80,     0,    12,    36,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,    45,     0,    44,     0,     0,
      35,     0,     0,    31,    30,    82,    81,     0,     0,    49,
      50,     0,    60,     0,    57,    58,    65,    70,    69,    68,
      67,    56,    59,    66,    61,    55,    63,    64,    62,    22,
       0,     0,     0,    53,     0,    54,     0,    47,    46,     0,
      11,    38,     0,    32,    39,     0,    34,    51,    24,     0,
       0,    33,    52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -49,    -1,   -49,   -49,   -49,   -49,   -49,    43,   -12,   -49,
     100,   -49,   -49,   -49,   -49,   -49,   -48,   -49,   -49,   -28,
     -49,   -49,   -27,    -9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    53,     2,     7,    12,    13,    54,    28,    31,    21,
      32,    33,   103,    25,    29,    74,    56,    69,   106,    70,
     118,   119,    71,    59
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       4,    57,    58,    20,    35,   108,     1,     3,    80,    87,
      16,    88,     5,    17,    35,   117,    18,    55,    22,    23,
       6,    95,    30,    36,    14,    40,   114,    15,    26,     8,
      61,    81,    72,   150,    64,    76,    77,    78,    79,    27,
       9,    24,    44,    62,    63,    45,    57,    58,    46,     3,
      47,    60,   -48,    66,    82,    67,    65,   104,   107,     3,
      50,    51,    68,    52,   112,    10,    73,   109,    11,    83,
      84,   102,   120,   110,   115,   140,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   153,    87,   142,    88,    16,   156,   143,
      17,   139,   145,    18,   144,    94,    95,   147,   148,    16,
      98,   161,    17,    57,    58,    18,    99,   100,    57,    58,
     149,   154,    19,   159,   162,   121,     0,    34,   152,     0,
       0,    57,    58,     0,    37,   155,    16,     0,     0,    17,
     157,    38,    18,    39,    40,     0,    41,     0,     0,     0,
      42,   158,     0,     0,     0,     0,    37,     0,     0,    43,
       0,    44,     0,    38,    45,    39,    40,    46,    41,    47,
      48,     0,    42,    49,     0,     0,     0,     0,     3,    50,
      51,    43,    52,    44,     0,     0,    45,    37,     0,    46,
       0,    47,   113,     0,    38,    49,    39,    40,     0,    41,
       3,    50,    51,    42,    52,     0,     0,     0,     0,    75,
       0,     0,    43,    87,    44,    88,     0,    45,     0,    40,
      46,     0,    47,     0,    94,    95,    49,     0,     0,    98,
       0,     3,    50,    51,     0,    52,    44,     0,     0,    45,
       0,    40,    46,     0,    47,     0,     0,     0,    40,     0,
       0,     0,     0,     3,    50,    51,     0,    52,    44,     0,
       0,    45,     0,     0,    46,    44,    47,     0,    45,     0,
       0,    46,     0,    47,     0,     3,    50,    51,   105,    52,
       0,     0,     3,    50,    51,     0,    52,    85,   111,    87,
       0,    88,    89,    90,    91,     0,    92,     0,     0,    93,
      94,    95,    96,     0,    97,    98,     0,   116,     0,     0,
       0,    99,   100,   101,    85,   111,    87,     0,    88,    89,
      90,    91,     0,    92,     0,     0,    93,    94,    95,    96,
       0,    97,    98,     0,   141,     0,     0,     0,    99,   100,
     101,    85,   111,    87,     0,    88,    89,    90,    91,     0,
      92,     0,     0,    93,    94,    95,    96,     0,    97,    98,
       0,   146,     0,     0,     0,    99,   100,   101,    85,   111,
      87,     0,    88,    89,    90,    91,     0,    92,     0,     0,
      93,    94,    95,    96,     0,    97,    98,     0,     0,     0,
     151,     0,    99,   100,   101,    85,   111,    87,     0,    88,
      89,    90,    91,     0,    92,     0,     0,    93,    94,    95,
      96,     0,    97,    98,     0,     0,   160,     0,     0,    99,
     100,   101,    85,    86,    87,     0,    88,    89,    90,    91,
       0,    92,     0,     0,    93,    94,    95,    96,     0,    97,
      98,     0,     0,     0,     0,     0,    99,   100,   101,    85,
     111,    87,     0,    88,    89,    90,    91,     0,    92,     0,
       0,    93,    94,    95,    96,     0,    97,    98,     0,     0,
       0,     0,     0,    99,   100,   101,    85,     0,    87,     0,
      88,    89,    90,    91,     0,    92,     0,     0,    93,    94,
      95,    96,     0,     0,    98,     0,     0,     0,     0,     0,
      99,   100,   101,    87,     0,    88,    89,    90,    91,     0,
      92,     0,     0,    93,    94,    95,    96,     0,     0,    98,
       0,     0,     0,     0,     0,    99,   100,   101,    87,     0,
      88,    89,    90,    91,     0,    92,     0,     0,    93,    94,
      95,    96,     0,     0,    98,     0,     0,     0,     0,     0,
      99,   100,    87,     0,    88,     0,    90,    91,     0,    92,
       0,     0,    93,    94,    95,     0,     0,     0,    98,     0,
       0,     0,     0,     0,    99,   100
};

static const yytype_int16 yycheck[] =
{
       1,    29,    29,    15,    21,     1,     4,    45,     5,    20,
       3,    22,     0,     6,    21,     1,     9,    29,    19,    20,
      26,    32,    15,    40,    40,    11,    74,    14,    28,     1,
      31,    28,    41,    40,    35,    44,    45,    46,    47,    28,
      12,    26,    28,    38,    38,    31,    74,    74,    34,    45,
      36,    29,    38,    28,    55,    28,    40,    66,    67,    45,
      46,    47,    28,    49,    73,    37,    28,    68,    40,    40,
      40,    39,    81,    40,    38,    21,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   141,    20,    38,    22,     3,   146,    38,
       6,   102,   111,     9,    29,    31,    32,    38,    38,     3,
      36,   159,     6,   141,   141,     9,    42,    43,   146,   146,
      21,    40,    16,     7,    38,    82,    -1,    27,   140,    -1,
      -1,   159,   159,    -1,     1,   144,     3,    -1,    -1,     6,
     149,     8,     9,    10,    11,    -1,    13,    -1,    -1,    -1,
      17,   152,    -1,    -1,    -1,    -1,     1,    -1,    -1,    26,
      -1,    28,    -1,     8,    31,    10,    11,    34,    13,    36,
      37,    -1,    17,    40,    -1,    -1,    -1,    -1,    45,    46,
      47,    26,    49,    28,    -1,    -1,    31,     1,    -1,    34,
      -1,    36,    37,    -1,     8,    40,    10,    11,    -1,    13,
      45,    46,    47,    17,    49,    -1,    -1,    -1,    -1,     1,
      -1,    -1,    26,    20,    28,    22,    -1,    31,    -1,    11,
      34,    -1,    36,    -1,    31,    32,    40,    -1,    -1,    36,
      -1,    45,    46,    47,    -1,    49,    28,    -1,    -1,    31,
      -1,    11,    34,    -1,    36,    -1,    -1,    -1,    11,    -1,
      -1,    -1,    -1,    45,    46,    47,    -1,    49,    28,    -1,
      -1,    31,    -1,    -1,    34,    28,    36,    -1,    31,    -1,
      -1,    34,    -1,    36,    -1,    45,    46,    47,    48,    49,
      -1,    -1,    45,    46,    47,    -1,    49,    18,    19,    20,
      -1,    22,    23,    24,    25,    -1,    27,    -1,    -1,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    -1,    -1,
      -1,    42,    43,    44,    18,    19,    20,    -1,    22,    23,
      24,    25,    -1,    27,    -1,    -1,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    -1,    -1,    -1,    42,    43,
      44,    18,    19,    20,    -1,    22,    23,    24,    25,    -1,
      27,    -1,    -1,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    -1,    -1,    -1,    42,    43,    44,    18,    19,
      20,    -1,    22,    23,    24,    25,    -1,    27,    -1,    -1,
      30,    31,    32,    33,    -1,    35,    36,    -1,    -1,    -1,
      40,    -1,    42,    43,    44,    18,    19,    20,    -1,    22,
      23,    24,    25,    -1,    27,    -1,    -1,    30,    31,    32,
      33,    -1,    35,    36,    -1,    -1,    39,    -1,    -1,    42,
      43,    44,    18,    19,    20,    -1,    22,    23,    24,    25,
      -1,    27,    -1,    -1,    30,    31,    32,    33,    -1,    35,
      36,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    18,
      19,    20,    -1,    22,    23,    24,    25,    -1,    27,    -1,
      -1,    30,    31,    32,    33,    -1,    35,    36,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    18,    -1,    20,    -1,
      22,    23,    24,    25,    -1,    27,    -1,    -1,    30,    31,
      32,    33,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    20,    -1,    22,    23,    24,    25,    -1,
      27,    -1,    -1,    30,    31,    32,    33,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    20,    -1,
      22,    23,    24,    25,    -1,    27,    -1,    -1,    30,    31,
      32,    33,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      42,    43,    20,    -1,    22,    -1,    24,    25,    -1,    27,
      -1,    -1,    30,    31,    32,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    42,    43
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    56,    45,    55,     0,    26,    57,     1,    12,
      37,    40,    58,    59,    40,    14,     3,     6,     9,    16,
      62,    63,    55,    55,    26,    67,    28,    28,    61,    68,
      15,    62,    64,    65,    64,    21,    40,     1,     8,    10,
      11,    13,    17,    26,    28,    31,    34,    36,    37,    40,
      46,    47,    49,    55,    60,    62,    70,    73,    76,    77,
      29,    55,    38,    38,    55,    40,    28,    28,    28,    71,
      73,    76,    77,    28,    69,     1,    77,    77,    77,    77,
       5,    28,    55,    40,    40,    18,    19,    20,    22,    23,
      24,    25,    27,    30,    31,    32,    33,    35,    36,    42,
      43,    44,    39,    66,    77,    48,    72,    77,     1,    55,
      40,    19,    77,    37,    70,    38,    38,     1,    74,    75,
      77,    61,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    55,
      21,    38,    38,    38,    29,    77,    38,    38,    38,    21,
      40,    40,    62,    70,    40,    77,    70,    77,    55,     7,
      39,    70,    38
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    56,    57,    57,    57,    57,    58,    59,
      59,    60,    61,    61,    62,    62,    62,    63,    63,    64,
      64,    65,    65,    66,    66,    67,    68,    68,    68,    69,
      69,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    71,    71,    72,    72,    73,    73,    74,    74,
      75,    75,    76,    76,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     5,     0,     2,     2,     2,     4,     6,
       2,     4,     0,     3,     1,     1,     1,     5,     5,     0,
       1,     3,     4,     0,     4,     3,     0,     2,     2,     0,
       2,     3,     5,     7,     5,     3,     2,     2,     4,     5,
       2,     1,     0,     1,     1,     1,     4,     4,     0,     1,
       1,     3,     7,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* AnyId: IDENTIFIER  */
#line 704 "jucompiler.y"
                  { (yyval.token) = (yyvsp[0].token); }
#line 2151 "y.tab.c"
    break;

  case 3: /* Program: CLASS AnyId LBRACE ProgramElements RBRACE  */
#line 707 "jucompiler.y"
                                                    {
            (yyval.node) = create_node("Program", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col);
            add_child((yyval.node), create_node("Identifier", (yyvsp[-3].token).str, (yyvsp[-3].token).line, (yyvsp[-3].token).col));
            if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node)); 
            root = (yyval.node); 
        }
#line 2162 "y.tab.c"
    break;

  case 4: /* ProgramElements: %empty  */
#line 714 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 2168 "y.tab.c"
    break;

  case 5: /* ProgramElements: ProgramElements MethodDecl  */
#line 715 "jucompiler.y"
                                             { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2174 "y.tab.c"
    break;

  case 6: /* ProgramElements: ProgramElements FieldDecl  */
#line 716 "jucompiler.y"
                                            { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2180 "y.tab.c"
    break;

  case 7: /* ProgramElements: ProgramElements SEMICOLON  */
#line 717 "jucompiler.y"
                                            { (yyval.node) = (yyvsp[-1].node); }
#line 2186 "y.tab.c"
    break;

  case 8: /* MethodDecl: PUBLIC STATIC MethodHeader MethodBody  */
#line 720 "jucompiler.y"
                                                   {
            (yyval.node) = create_node("MethodDecl", NULL, (yyvsp[-3].token).line, (yyvsp[-3].token).col);
            add_child((yyval.node), (yyvsp[-1].node));
            add_sibling((yyvsp[-1].node), (yyvsp[0].node));
        }
#line 2196 "y.tab.c"
    break;

  case 9: /* FieldDecl: PUBLIC STATIC Type AnyId IdList SEMICOLON  */
#line 727 "jucompiler.y"
                                                      {
            (yyval.node) = create_node("FieldDecl", NULL, (yyvsp[-5].token).line, (yyvsp[-5].token).col);
            add_child((yyval.node), (yyvsp[-3].node));
            add_child((yyval.node), create_node("Identifier", (yyvsp[-2].token).str, (yyvsp[-2].token).line, (yyvsp[-2].token).col));
            Node* aux = (yyvsp[-1].node);
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_field = create_node("FieldDecl", NULL, (yyvsp[-5].token).line, (yyvsp[-5].token).col);
                add_child(new_field, create_node((yyvsp[-3].node)->type, NULL, (yyvsp[-3].node)->line, (yyvsp[-3].node)->col)); 
                add_child(new_field, aux);
                (yyval.node) = add_sibling((yyval.node), new_field);
                aux = next;
            }
          }
#line 2216 "y.tab.c"
    break;

  case 10: /* FieldDecl: error SEMICOLON  */
#line 742 "jucompiler.y"
                            { (yyval.node) = NULL; }
#line 2222 "y.tab.c"
    break;

  case 11: /* VarDecl: Type AnyId IdList SEMICOLON  */
#line 745 "jucompiler.y"
                                      {
            (yyval.node) = create_node("VarDecl", NULL, (yyvsp[-3].node)->line, (yyvsp[-3].node)->col);
            add_child((yyval.node), (yyvsp[-3].node));
            add_child((yyval.node), create_node("Identifier", (yyvsp[-2].token).str, (yyvsp[-2].token).line, (yyvsp[-2].token).col));
            Node* aux = (yyvsp[-1].node);
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_var = create_node("VarDecl", NULL, (yyvsp[-3].node)->line, (yyvsp[-3].node)->col);
                add_child(new_var, create_node((yyvsp[-3].node)->type, NULL, (yyvsp[-3].node)->line, (yyvsp[-3].node)->col));
                add_child(new_var, aux);
                (yyval.node) = add_sibling((yyval.node), new_var);
                aux = next;
            }
        }
#line 2242 "y.tab.c"
    break;

  case 12: /* IdList: %empty  */
#line 762 "jucompiler.y"
        { (yyval.node) = NULL; }
#line 2248 "y.tab.c"
    break;

  case 13: /* IdList: IdList COMMA AnyId  */
#line 763 "jucompiler.y"
                            {   (yyval.node) = add_sibling((yyvsp[-2].node), create_node("Identifier", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col)); }
#line 2254 "y.tab.c"
    break;

  case 14: /* Type: BOOL  */
#line 766 "jucompiler.y"
              { (yyval.node) = create_node("Bool", NULL, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2260 "y.tab.c"
    break;

  case 15: /* Type: INT  */
#line 767 "jucompiler.y"
              { (yyval.node) = create_node("Int", NULL, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2266 "y.tab.c"
    break;

  case 16: /* Type: DOUBLE  */
#line 768 "jucompiler.y"
              { (yyval.node) = create_node("Double", NULL, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2272 "y.tab.c"
    break;

  case 17: /* MethodHeader: Type AnyId LPAR OptFormalParams RPAR  */
#line 771 "jucompiler.y"
                                                    {
                (yyval.node) = create_node("MethodHeader", NULL, (yyvsp[-4].node)->line, (yyvsp[-4].node)->col);
                add_child((yyval.node), (yyvsp[-4].node));
                add_child((yyval.node), create_node("Identifier", (yyvsp[-3].token).str, (yyvsp[-3].token).line, (yyvsp[-3].token).col));
                
                Node* params = create_node("MethodParams", NULL, (yyvsp[-2].token).line, (yyvsp[-2].token).col);
                if ((yyvsp[-1].node)) add_child(params, (yyvsp[-1].node));
                add_child((yyval.node), params);
             }
#line 2286 "y.tab.c"
    break;

  case 18: /* MethodHeader: VOID AnyId LPAR OptFormalParams RPAR  */
#line 780 "jucompiler.y"
                                                    {
                (yyval.node) = create_node("MethodHeader", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col);
                Node* v = create_node("Void", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col);
                add_child((yyval.node), v);
                add_child((yyval.node), create_node("Identifier", (yyvsp[-3].token).str, (yyvsp[-3].token).line, (yyvsp[-3].token).col));
                Node* params = create_node("MethodParams", NULL, (yyvsp[-2].token).line, (yyvsp[-2].token).col);
                if ((yyvsp[-1].node)) add_child(params, (yyvsp[-1].node));
                add_child((yyval.node), params);
             }
#line 2300 "y.tab.c"
    break;

  case 19: /* OptFormalParams: %empty  */
#line 791 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 2306 "y.tab.c"
    break;

  case 20: /* OptFormalParams: FormalParams  */
#line 792 "jucompiler.y"
                               { (yyval.node) = (yyvsp[0].node); }
#line 2312 "y.tab.c"
    break;

  case 21: /* FormalParams: Type AnyId FormalParamsRest  */
#line 795 "jucompiler.y"
                                           {
                (yyval.node) = create_node("ParamDecl", NULL, (yyvsp[-2].node)->line, (yyvsp[-2].node)->col);
                add_child((yyval.node), (yyvsp[-2].node));
                add_child((yyval.node), create_node("Identifier", (yyvsp[-1].token).str, (yyvsp[-1].token).line, (yyvsp[-1].token).col));
                if ((yyvsp[0].node)) add_sibling((yyval.node), (yyvsp[0].node));
             }
#line 2323 "y.tab.c"
    break;

  case 22: /* FormalParams: STRING LSQ RSQ AnyId  */
#line 801 "jucompiler.y"
                                    {
                (yyval.node) = create_node("ParamDecl", NULL, (yyvsp[-3].token).line, (yyvsp[-3].token).col);
                Node* strArray = create_node("StringArray", NULL, (yyvsp[-3].token).line, (yyvsp[-3].token).col);
                add_child((yyval.node), strArray);
                add_child((yyval.node), create_node("Identifier", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col));
             }
#line 2334 "y.tab.c"
    break;

  case 23: /* FormalParamsRest: %empty  */
#line 809 "jucompiler.y"
                   { (yyval.node) = NULL; }
#line 2340 "y.tab.c"
    break;

  case 24: /* FormalParamsRest: FormalParamsRest COMMA Type AnyId  */
#line 810 "jucompiler.y"
                                                     {
                    Node* param = create_node("ParamDecl", NULL, (yyvsp[-1].node)->line, (yyvsp[-1].node)->col);
                    add_child(param, (yyvsp[-1].node));
                    add_child(param, create_node("Identifier", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col));
                    (yyval.node) = add_sibling((yyvsp[-3].node), param);
                 }
#line 2351 "y.tab.c"
    break;

  case 25: /* MethodBody: LBRACE StatementAndVarDeclList RBRACE  */
#line 818 "jucompiler.y"
                                                   {
                (yyval.node) = create_node("MethodBody", NULL, (yyvsp[-2].token).line, (yyvsp[-2].token).col);
                if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
           }
#line 2360 "y.tab.c"
    break;

  case 26: /* StatementAndVarDeclList: %empty  */
#line 824 "jucompiler.y"
                          { (yyval.node) = NULL; }
#line 2366 "y.tab.c"
    break;

  case 27: /* StatementAndVarDeclList: StatementAndVarDeclList Statement  */
#line 825 "jucompiler.y"
                                                            {
                            if ((yyvsp[0].node) != NULL) (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node));
                            else (yyval.node) = (yyvsp[-1].node);
                        }
#line 2375 "y.tab.c"
    break;

  case 28: /* StatementAndVarDeclList: StatementAndVarDeclList VarDecl  */
#line 829 "jucompiler.y"
                                                          { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2381 "y.tab.c"
    break;

  case 29: /* StatementList: %empty  */
#line 832 "jucompiler.y"
                { (yyval.node) = NULL; }
#line 2387 "y.tab.c"
    break;

  case 30: /* StatementList: StatementList Statement  */
#line 833 "jucompiler.y"
                                        {
                  if ((yyvsp[0].node) != NULL) (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node));
                  else (yyval.node) = (yyvsp[-1].node);
              }
#line 2396 "y.tab.c"
    break;

  case 31: /* Statement: LBRACE StatementList RBRACE  */
#line 839 "jucompiler.y"
                                        {
              if ((yyvsp[-1].node) != NULL && (yyvsp[-1].node)->sibling != NULL) {
                  (yyval.node) = create_node("Block", NULL, (yyvsp[-2].token).line, (yyvsp[-2].token).col);
                  add_child((yyval.node), (yyvsp[-1].node));
              } else if ((yyvsp[-1].node) != NULL) {
                  (yyval.node) = (yyvsp[-1].node);
              } else {
                  (yyval.node) = NULL;
              }
          }
#line 2411 "y.tab.c"
    break;

  case 32: /* Statement: IF LPAR Expr RPAR Statement  */
#line 849 "jucompiler.y"
                                                              {
              (yyval.node) = create_node("If", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col);
              add_child((yyval.node), (yyvsp[-2].node)); 
              add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col)); 
              add_child((yyval.node), create_node("Block", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col));
          }
#line 2422 "y.tab.c"
    break;

  case 33: /* Statement: IF LPAR Expr RPAR Statement ELSE Statement  */
#line 855 "jucompiler.y"
                                                       {
              (yyval.node) = create_node("If", NULL, (yyvsp[-6].token).line, (yyvsp[-6].token).col);
              add_child((yyval.node), (yyvsp[-4].node));
              add_child((yyval.node), (yyvsp[-2].node) ? (yyvsp[-2].node) : create_node("Block", NULL, (yyvsp[-6].token).line, (yyvsp[-6].token).col));
              add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL, (yyvsp[-6].token).line, (yyvsp[-6].token).col));
          }
#line 2433 "y.tab.c"
    break;

  case 34: /* Statement: WHILE LPAR Expr RPAR Statement  */
#line 861 "jucompiler.y"
                                           {
              (yyval.node) = create_node("While", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col);
              add_child((yyval.node), (yyvsp[-2].node));
              add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col));
          }
#line 2443 "y.tab.c"
    break;

  case 35: /* Statement: RETURN OptExpr SEMICOLON  */
#line 866 "jucompiler.y"
                                     {
              (yyval.node) = create_node("Return", NULL, (yyvsp[-2].token).line, (yyvsp[-2].token).col);
              if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
          }
#line 2452 "y.tab.c"
    break;

  case 36: /* Statement: MethodInvocation SEMICOLON  */
#line 870 "jucompiler.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2458 "y.tab.c"
    break;

  case 37: /* Statement: ParseArgs SEMICOLON  */
#line 871 "jucompiler.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2464 "y.tab.c"
    break;

  case 38: /* Statement: Expr ASSIGN Expr SEMICOLON  */
#line 872 "jucompiler.y"
                                       { 
              if ((yyvsp[-3].node) && strcmp((yyvsp[-3].node)->type, "Identifier") != 0) {
                  printf("Line %d, col %d: syntax error: =\n", (yyvsp[-2].token).line, (yyvsp[-2].token).col);
                  syntax_errors_count++;
              }
              (yyval.node) = create_node("Assign", NULL, (yyvsp[-2].token).line, (yyvsp[-2].token).col);
              add_child((yyval.node), (yyvsp[-3].node));
              add_child((yyval.node), (yyvsp[-1].node));
          }
#line 2478 "y.tab.c"
    break;

  case 39: /* Statement: PRINT LPAR ExprOrStrlit RPAR SEMICOLON  */
#line 881 "jucompiler.y"
                                                   {
              (yyval.node) = create_node("Print", NULL, (yyvsp[-4].token).line, (yyvsp[-4].token).col);
              add_child((yyval.node), (yyvsp[-2].node));
          }
#line 2487 "y.tab.c"
    break;

  case 40: /* Statement: error SEMICOLON  */
#line 885 "jucompiler.y"
                            { (yyval.node) = NULL; }
#line 2493 "y.tab.c"
    break;

  case 41: /* Statement: SEMICOLON  */
#line 886 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 2499 "y.tab.c"
    break;

  case 42: /* OptExpr: %empty  */
#line 889 "jucompiler.y"
          { (yyval.node) = NULL; }
#line 2505 "y.tab.c"
    break;

  case 43: /* OptExpr: Expr  */
#line 890 "jucompiler.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2511 "y.tab.c"
    break;

  case 44: /* ExprOrStrlit: Expr  */
#line 893 "jucompiler.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2517 "y.tab.c"
    break;

  case 45: /* ExprOrStrlit: STRLIT  */
#line 894 "jucompiler.y"
                      { (yyval.node) = create_node("StrLit", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2523 "y.tab.c"
    break;

  case 46: /* MethodInvocation: AnyId LPAR OptExprList RPAR  */
#line 897 "jucompiler.y"
                                               {
                     (yyval.node) = create_node("Call", NULL, (yyvsp[-3].token).line, (yyvsp[-3].token).col);
                     Node* id = create_node("Identifier", (yyvsp[-3].token).str, (yyvsp[-3].token).line, (yyvsp[-3].token).col);
                     add_child((yyval.node), id);
                     if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
                 }
#line 2534 "y.tab.c"
    break;

  case 47: /* MethodInvocation: AnyId LPAR error RPAR  */
#line 903 "jucompiler.y"
                                         { (yyval.node) = NULL; }
#line 2540 "y.tab.c"
    break;

  case 48: /* OptExprList: %empty  */
#line 906 "jucompiler.y"
              { (yyval.node) = NULL; }
#line 2546 "y.tab.c"
    break;

  case 49: /* OptExprList: ExprList  */
#line 907 "jucompiler.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2552 "y.tab.c"
    break;

  case 50: /* ExprList: Expr  */
#line 910 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2558 "y.tab.c"
    break;

  case 51: /* ExprList: ExprList COMMA Expr  */
#line 911 "jucompiler.y"
                               { (yyval.node) = add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2564 "y.tab.c"
    break;

  case 52: /* ParseArgs: PARSEINT LPAR AnyId LSQ Expr RSQ RPAR  */
#line 914 "jucompiler.y"
                                                  {
              (yyval.node) = create_node("ParseArgs", NULL, (yyvsp[-6].token).line, (yyvsp[-6].token).col);
              Node* id = create_node("Identifier", (yyvsp[-4].token).str, (yyvsp[-4].token).line, (yyvsp[-4].token).col);
              add_child((yyval.node), id);
              add_child((yyval.node), (yyvsp[-2].node));
          }
#line 2575 "y.tab.c"
    break;

  case 53: /* ParseArgs: PARSEINT LPAR error RPAR  */
#line 920 "jucompiler.y"
                                     { (yyval.node) = NULL; }
#line 2581 "y.tab.c"
    break;

  case 54: /* Expr: Expr ASSIGN Expr  */
#line 923 "jucompiler.y"
                        { 
          if ((yyvsp[-2].node) && strcmp((yyvsp[-2].node)->type, "Identifier") != 0) {
              printf("Line %d, col %d: syntax error: =\n", (yyvsp[-1].token).line, (yyvsp[-1].token).col);
              syntax_errors_count++;
          }
          (yyval.node) = create_node("Assign", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col);
          add_child((yyval.node), (yyvsp[-2].node));
          add_child((yyval.node), (yyvsp[0].node));
     }
#line 2595 "y.tab.c"
    break;

  case 55: /* Expr: Expr PLUS Expr  */
#line 932 "jucompiler.y"
                        { (yyval.node) = create_node("Add", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2601 "y.tab.c"
    break;

  case 56: /* Expr: Expr MINUS Expr  */
#line 933 "jucompiler.y"
                        { (yyval.node) = create_node("Sub", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2607 "y.tab.c"
    break;

  case 57: /* Expr: Expr STAR Expr  */
#line 934 "jucompiler.y"
                        { (yyval.node) = create_node("Mul", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2613 "y.tab.c"
    break;

  case 58: /* Expr: Expr DIV Expr  */
#line 935 "jucompiler.y"
                        { (yyval.node) = create_node("Div", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2619 "y.tab.c"
    break;

  case 59: /* Expr: Expr MOD Expr  */
#line 936 "jucompiler.y"
                        { (yyval.node) = create_node("Mod", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2625 "y.tab.c"
    break;

  case 60: /* Expr: Expr AND Expr  */
#line 937 "jucompiler.y"
                        { (yyval.node) = create_node("And", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2631 "y.tab.c"
    break;

  case 61: /* Expr: Expr OR Expr  */
#line 938 "jucompiler.y"
                        { (yyval.node) = create_node("Or", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2637 "y.tab.c"
    break;

  case 62: /* Expr: Expr XOR Expr  */
#line 939 "jucompiler.y"
                        { (yyval.node) = create_node("Xor", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2643 "y.tab.c"
    break;

  case 63: /* Expr: Expr LSHIFT Expr  */
#line 940 "jucompiler.y"
                        { (yyval.node) = create_node("Lshift", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2649 "y.tab.c"
    break;

  case 64: /* Expr: Expr RSHIFT Expr  */
#line 941 "jucompiler.y"
                        { (yyval.node) = create_node("Rshift", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2655 "y.tab.c"
    break;

  case 65: /* Expr: Expr EQ Expr  */
#line 942 "jucompiler.y"
                        { (yyval.node) = create_node("Eq", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2661 "y.tab.c"
    break;

  case 66: /* Expr: Expr NE Expr  */
#line 943 "jucompiler.y"
                        { (yyval.node) = create_node("Ne", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2667 "y.tab.c"
    break;

  case 67: /* Expr: Expr LT Expr  */
#line 944 "jucompiler.y"
                        { (yyval.node) = create_node("Lt", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2673 "y.tab.c"
    break;

  case 68: /* Expr: Expr LE Expr  */
#line 945 "jucompiler.y"
                        { (yyval.node) = create_node("Le", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2679 "y.tab.c"
    break;

  case 69: /* Expr: Expr GT Expr  */
#line 946 "jucompiler.y"
                        { (yyval.node) = create_node("Gt", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2685 "y.tab.c"
    break;

  case 70: /* Expr: Expr GE Expr  */
#line 947 "jucompiler.y"
                        { (yyval.node) = create_node("Ge", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2691 "y.tab.c"
    break;

  case 71: /* Expr: PLUS Expr  */
#line 948 "jucompiler.y"
                                    { (yyval.node) = create_node("Plus", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[0].node)); }
#line 2697 "y.tab.c"
    break;

  case 72: /* Expr: MINUS Expr  */
#line 949 "jucompiler.y"
                                    { (yyval.node) = create_node("Minus", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[0].node)); }
#line 2703 "y.tab.c"
    break;

  case 73: /* Expr: NOT Expr  */
#line 950 "jucompiler.y"
                        { (yyval.node) = create_node("Not", NULL, (yyvsp[-1].token).line, (yyvsp[-1].token).col); add_child((yyval.node), (yyvsp[0].node)); }
#line 2709 "y.tab.c"
    break;

  case 74: /* Expr: MethodInvocation  */
#line 951 "jucompiler.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2715 "y.tab.c"
    break;

  case 75: /* Expr: ParseArgs  */
#line 952 "jucompiler.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2721 "y.tab.c"
    break;

  case 76: /* Expr: AnyId  */
#line 953 "jucompiler.y"
                        { (yyval.node) = create_node("Identifier", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2727 "y.tab.c"
    break;

  case 77: /* Expr: NATURAL  */
#line 954 "jucompiler.y"
                        { (yyval.node) = create_node("Natural", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2733 "y.tab.c"
    break;

  case 78: /* Expr: DECIMAL  */
#line 955 "jucompiler.y"
                        { (yyval.node) = create_node("Decimal", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2739 "y.tab.c"
    break;

  case 79: /* Expr: BOOLLIT  */
#line 956 "jucompiler.y"
                        { (yyval.node) = create_node("BoolLit", (yyvsp[0].token).str, (yyvsp[0].token).line, (yyvsp[0].token).col); }
#line 2745 "y.tab.c"
    break;

  case 80: /* Expr: AnyId DOTLENGTH  */
#line 957 "jucompiler.y"
                        { (yyval.node) = create_node("Length", NULL, (yyvsp[0].token).line, (yyvsp[0].token).col); add_child((yyval.node), create_node("Identifier", (yyvsp[-1].token).str, (yyvsp[-1].token).line, (yyvsp[-1].token).col)); }
#line 2751 "y.tab.c"
    break;

  case 81: /* Expr: LPAR Expr RPAR  */
#line 958 "jucompiler.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 2757 "y.tab.c"
    break;

  case 82: /* Expr: LPAR error RPAR  */
#line 959 "jucompiler.y"
                        { (yyval.node) = NULL; }
#line 2763 "y.tab.c"
    break;


#line 2767 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 961 "jucompiler.y"

void yyerror(char *s) {
    syntax_errors_count++;
    if (yychar == 0) {
        printf("Line %d, col %d: %s: \n", line, col, s);
    } 
    else if (yychar == STRLIT) {
        printf("Line %d, col %d: %s: %s\n", str_line, str_col, s, yylval.token.str);
    } 
    else {
        printf("Line %d, col %d: %s: %s\n", token_line, token_col, s, yytext);
    }
}
const char* display_type(const char *type) {
    if (!type) return "";
    return type;
}

void print_tree(Node* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("..");
    
    const char *disp = display_type(node->type);
    if (node->value) printf("%s(%s)", disp, node->value);
    else printf("%s", disp);
    if (node->anot_type && is_expression(node->type)) printf(" - %s", node->anot_type);
    printf("\n");
    
    Node* child = node->child;
    while (child) {
        print_tree(child, depth + 1);
        child = child->sibling;
    }
}

int main(int argc, char *argv[]) {
    int flag_s = 0;
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
            // syntax only
        } else if (strcmp(argv[1], "-s") == 0) {
            flag_s = 1;
        } else if (strcmp(argv[1], "-e3") == 0) {
            /* -e3: run semantics, errors only - no tables or AST */
        }
    }
    
    yyparse();
    if (syntax_errors_count == 0) {
        if (flag_syntax) {
            /* -t: syntax AST only */
            print_tree(root, 0);
        } else if (flag_s) {
            /* -s: errors + tables + annotated AST */
            annotate_ast(root, NULL);
            print_tables();
            print_tree(root, 0);
        } else {
            /* -e3 or no flag: errors only */
            annotate_ast(root, NULL);
        }
    }
    return 0;
}
