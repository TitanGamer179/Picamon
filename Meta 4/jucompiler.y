/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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
            else printf("%s\t\t%s%s\n", s->name, s->type, s->is_param ? "\tparam" : "");
            s = s->next;
        }
        printf("\n");
        t = t->next;
    }
}

/* ============================================================
 * LLVM IR CODE GENERATION - Meta 4
 * ============================================================ */

static int reg_counter = 0;
static int label_counter = 0;
static int str_const_counter = 0;

/* String constants buffer */
#define MAX_STR_CONSTS 4096
static char *str_consts[MAX_STR_CONSTS];
static int str_const_len[MAX_STR_CONSTS];
static int str_const_ids[MAX_STR_CONSTS];
static int num_str_consts = 0;

/* Current method return type for codegen */
static char *current_method_return_type = NULL;
/* Current method symbol table */
static SymTable *current_method_table = NULL;
/* Name of the class */
static char *class_name = NULL;

static int new_reg() { return ++reg_counter; }
static int new_label() { return ++label_counter; }

static const char* llvm_type(const char *juc_type) {
    if (!juc_type) return "i32";
    if (strcmp(juc_type, "int") == 0) return "i32";
    if (strcmp(juc_type, "double") == 0) return "double";
    if (strcmp(juc_type, "boolean") == 0) return "i1";
    if (strcmp(juc_type, "void") == 0) return "void";
    if (strcmp(juc_type, "String[]") == 0) return "i8**";
    return "i32";
}

static const char* default_value(const char *juc_type) {
    if (!juc_type) return "0";
    if (strcmp(juc_type, "double") == 0) return "0.0";
    if (strcmp(juc_type, "boolean") == 0) return "0";
    return "0";
}

/* Remove underscores from a numeric literal */
static char* clean_num(const char *s) {
    char *r = (char*)malloc(strlen(s) + 1);
    int j = 0;
    for (int i = 0; s[i]; i++) if (s[i] != '_') r[j++] = s[i];
    r[j] = 0;
    return r;
}

/* Escape a string literal (stored as "...") to get the LLVM bytes */
/* Returns alloc'd string with the actual content (no surrounding quotes, escape sequences processed) */
static char* process_strlit(const char *raw) {
    /* raw has surrounding quotes like "hello\nworld" */
    int len = strlen(raw);
    char *out = (char*)malloc(len + 1);
    int j = 0;
    int i = 1; /* skip opening quote */
    while (i < len - 1) { /* stop before closing quote */
        if (raw[i] == '\\' && i + 1 < len - 1) {
            char c = raw[i+1];
            switch(c) {
                case 'n': out[j++] = '\n'; i += 2; break;
                case 't': out[j++] = '\t'; i += 2; break;
                case 'r': out[j++] = '\r'; i += 2; break;
                case 'f': out[j++] = '\f'; i += 2; break;
                case '\\': out[j++] = '\\'; i += 2; break;
                case '"': out[j++] = '"'; i += 2; break;
                default: out[j++] = raw[i]; i++; break;
            }
        } else {
            out[j++] = raw[i++];
        }
    }
    out[j] = 0;
    return out;
}

/* Convert processed string content to LLVM string constant notation */
static char* to_llvm_str(const char *s, int *outlen) {
    int len = strlen(s);
    char *buf = (char*)malloc(len * 4 + 4);
    int j = 0;
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == '\\' || c == '"' || c < 32 || c > 126) {
            sprintf(buf + j, "\\%02X", c);
            j += 3;
        } else {
            buf[j++] = c;
        }
    }
    buf[j++] = '\\';
    buf[j++] = '0';
    buf[j++] = '0';
    buf[j] = 0;
    *outlen = len + 1; /* +1 for null terminator */
    return buf;
}

/* Register a string literal, return its id. Deduplicates by raw value. */
static int register_strlit(const char *raw_strlit) {
    /* Check if already registered */
    for (int i = 0; i < num_str_consts; i++) {
        /* We store the raw_strlit as a parallel array for lookup */
        /* Instead: look up by llvm_s content match - simpler: store raw too */
    }
    char *content = process_strlit(raw_strlit);
    int slen = 0;
    char *llvm_s = to_llvm_str(content, &slen);
    free(content);
    /* Check for duplicate by llvm string content */
    for (int i = 0; i < num_str_consts; i++) {
        if (strcmp(str_consts[i], llvm_s) == 0) {
            free(llvm_s);
            return str_const_ids[i];
        }
    }
    int id = str_const_counter++;
    str_consts[num_str_consts] = llvm_s;
    str_const_len[num_str_consts] = slen;
    str_const_ids[num_str_consts] = id;
    num_str_consts++;
    return id;
}

/* Look up string literal id (must have been registered already) */
static int lookup_strlit(const char *raw_strlit) {
    return register_strlit(raw_strlit); /* now idempotent */
}

/* Forward declarations for codegen */
static int codegen_expr(Node *node);
static void codegen_stmt(Node *node);
static void codegen_method(Node *node);

/* Emit integer conversion: if expr is int but we need double, do sitofp */
static int emit_to_double(int reg, const char *t) {
    if (strcmp(t, "int") == 0) {
        int r2 = new_reg();
        printf("  %%r%d = sitofp i32 %%r%d to double\n", r2, reg);
        return r2;
    }
    return reg;
}

/* codegen_expr: generates code for an expression node, returns register holding result */
static int codegen_expr(Node *node) {
    if (!node) return 0;

    if (strcmp(node->type, "Natural") == 0) {
        char *clean = clean_num(node->value);
        int r = new_reg();
        printf("  %%r%d = add i32 0, %s\n", r, clean);
        free(clean);
        return r;
    }

    if (strcmp(node->type, "Decimal") == 0) {
        char *clean = clean_num(node->value);
        int r = new_reg();
        /* Use hex representation for precision */
        double val = strtod(clean, NULL);
        free(clean);
        /* print as hex double */
        union { double d; unsigned long long u; } conv;
        conv.d = val;
        printf("  %%r%d = fadd double 0.0, 0x%016llX\n", r, (unsigned long long)conv.u);
        return r;
    }

    if (strcmp(node->type, "BoolLit") == 0) {
        int r = new_reg();
        int val = (strcmp(node->value, "true") == 0) ? 1 : 0;
        printf("  %%r%d = add i1 0, %d\n", r, val);
        return r;
    }

    if (strcmp(node->type, "StrLit") == 0) {
        int id = register_strlit(node->value);
        int r = new_reg();
        /* get pointer to the string constant */
        char *content = process_strlit(node->value);
        int slen = strlen(content) + 1;
        free(content);
        printf("  %%r%d = getelementptr inbounds [%d x i8], [%d x i8]* @.str%d, i32 0, i32 0\n", r, slen, slen, id);
        return r;
    }

    if (strcmp(node->type, "Identifier") == 0) {
        char *name = node->value;
        char *t = node->anot_type ? node->anot_type : "int";
        const char *lt = llvm_type(t);
        int r = new_reg();
        /* check if global or local */
        int is_global = 0;
        if (current_method_table) {
            Symbol *s = current_method_table->symbols;
            while (s) {
                if (strcmp(s->name, name) == 0 && !s->param_types) { is_global = 0; goto found_local; }
                s = s->next;
            }
        }
        /* check global */
        if (global_table) {
            Symbol *s = global_table->symbols;
            while (s) {
                if (strcmp(s->name, name) == 0 && !s->param_types) { is_global = 1; break; }
                s = s->next;
            }
        }
        found_local:;
        if (is_global) {
            printf("  %%r%d = load %s, %s* @%s\n", r, lt, lt, name);
        } else {
            printf("  %%r%d = load %s, %s* %%%s\n", r, lt, lt, name);
        }
        return r;
    }

    if (strcmp(node->type, "Assign") == 0) {
        Node *lhs = node->child;
        Node *rhs = lhs ? lhs->sibling : NULL;
        char *t = lhs ? (lhs->anot_type ? lhs->anot_type : "int") : "int";
        const char *lt = llvm_type(t);
        int rv = codegen_expr(rhs);
        /* if lhs is double and rhs is int, convert */
        if (strcmp(t, "double") == 0 && rhs && rhs->anot_type && strcmp(rhs->anot_type, "int") == 0) {
            rv = emit_to_double(rv, "int");
        }
        /* check if global or local */
        char *name = lhs->value;
        int is_global = 0;
        if (current_method_table) {
            Symbol *s = current_method_table->symbols;
            while (s) {
                if (strcmp(s->name, name) == 0 && !s->param_types) { is_global = 0; goto found_local2; }
                s = s->next;
            }
        }
        if (global_table) {
            Symbol *s = global_table->symbols;
            while (s) {
                if (strcmp(s->name, name) == 0 && !s->param_types) { is_global = 1; break; }
                s = s->next;
            }
        }
        found_local2:;
        if (is_global) {
            printf("  store %s %%r%d, %s* @%s\n", lt, rv, lt, name);
        } else {
            printf("  store %s %%r%d, %s* %%%s\n", lt, rv, lt, name);
        }
        int r = new_reg();
        printf("  %%r%d = load %s, %s* %s%s\n", r, lt, lt, is_global ? "@" : "%", name);
        return r;
    }

    /* Binary arithmetic operators */
    if (strcmp(node->type, "Add") == 0 || strcmp(node->type, "Sub") == 0 ||
        strcmp(node->type, "Mul") == 0 || strcmp(node->type, "Div") == 0 ||
        strcmp(node->type, "Mod") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        int r1 = codegen_expr(c1);
        int r2 = codegen_expr(c2);
        char *t = node->anot_type ? node->anot_type : "int";
        const char *lt = llvm_type(t);
        int is_double = (strcmp(t, "double") == 0);
        /* promote if needed */
        if (is_double) {
            r1 = emit_to_double(r1, c1 ? (c1->anot_type ? c1->anot_type : "int") : "int");
            r2 = emit_to_double(r2, c2 ? (c2->anot_type ? c2->anot_type : "int") : "int");
        }
        int r = new_reg();
        const char *op;
        if (strcmp(node->type, "Add") == 0) op = is_double ? "fadd" : "add";
        else if (strcmp(node->type, "Sub") == 0) op = is_double ? "fsub" : "sub";
        else if (strcmp(node->type, "Mul") == 0) op = is_double ? "fmul" : "mul";
        else if (strcmp(node->type, "Div") == 0) op = is_double ? "fdiv" : "sdiv";
        else op = is_double ? "frem" : "srem";
        printf("  %%r%d = %s %s %%r%d, %%r%d\n", r, op, lt, r1, r2);
        return r;
    }

    /* Bitwise / shift */
    if (strcmp(node->type, "And") == 0 || strcmp(node->type, "Or") == 0 ||
        strcmp(node->type, "Xor") == 0 || strcmp(node->type, "Lshift") == 0 ||
        strcmp(node->type, "Rshift") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        int r1 = codegen_expr(c1);
        int r2 = codegen_expr(c2);
        char *t = node->anot_type ? node->anot_type : "int";
        const char *lt = llvm_type(t);
        int r = new_reg();
        const char *op;
        if (strcmp(node->type, "And") == 0) op = "and";
        else if (strcmp(node->type, "Or") == 0) op = "or";
        else if (strcmp(node->type, "Xor") == 0) op = "xor";
        else if (strcmp(node->type, "Lshift") == 0) op = "shl";
        else op = "ashr";
        printf("  %%r%d = %s %s %%r%d, %%r%d\n", r, op, lt, r1, r2);
        return r;
    }

    /* Comparison operators */
    if (strcmp(node->type, "Eq") == 0 || strcmp(node->type, "Ne") == 0 ||
        strcmp(node->type, "Lt") == 0 || strcmp(node->type, "Gt") == 0 ||
        strcmp(node->type, "Le") == 0 || strcmp(node->type, "Ge") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        int r1 = codegen_expr(c1);
        int r2 = codegen_expr(c2);
        char *t1 = c1 ? (c1->anot_type ? c1->anot_type : "int") : "int";
        char *t2 = c2 ? (c2->anot_type ? c2->anot_type : "int") : "int";
        int is_double = (strcmp(t1, "double") == 0 || strcmp(t2, "double") == 0);
        int is_bool = (strcmp(t1, "boolean") == 0);
        if (is_double) {
            r1 = emit_to_double(r1, t1);
            r2 = emit_to_double(r2, t2);
        }
        const char *op;
        const char *cmp_type = is_double ? "double" : (is_bool ? "i1" : "i32");
        if (is_double) {
            if (strcmp(node->type, "Eq") == 0) op = "fcmp oeq";
            else if (strcmp(node->type, "Ne") == 0) op = "fcmp one";
            else if (strcmp(node->type, "Lt") == 0) op = "fcmp olt";
            else if (strcmp(node->type, "Gt") == 0) op = "fcmp ogt";
            else if (strcmp(node->type, "Le") == 0) op = "fcmp ole";
            else op = "fcmp oge";
        } else {
            if (strcmp(node->type, "Eq") == 0) op = "icmp eq";
            else if (strcmp(node->type, "Ne") == 0) op = "icmp ne";
            else if (strcmp(node->type, "Lt") == 0) op = "icmp slt";
            else if (strcmp(node->type, "Gt") == 0) op = "icmp sgt";
            else if (strcmp(node->type, "Le") == 0) op = "icmp sle";
            else op = "icmp sge";
        }
        int r = new_reg();
        printf("  %%r%d = %s %s %%r%d, %%r%d\n", r, op, cmp_type, r1, r2);
        return r;
    }

    /* Unary operators */
    if (strcmp(node->type, "Minus") == 0) {
        int r1 = codegen_expr(node->child);
        char *t = node->anot_type ? node->anot_type : "int";
        int r = new_reg();
        if (strcmp(t, "double") == 0)
            printf("  %%r%d = fneg double %%r%d\n", r, r1);
        else
            printf("  %%r%d = sub i32 0, %%r%d\n", r, r1);
        return r;
    }

    if (strcmp(node->type, "Plus") == 0) {
        return codegen_expr(node->child);
    }

    if (strcmp(node->type, "Not") == 0) {
        int r1 = codegen_expr(node->child);
        int r = new_reg();
        printf("  %%r%d = xor i1 %%r%d, 1\n", r, r1);
        return r;
    }

    if (strcmp(node->type, "Length") == 0) {
        /* args.length - needs to find the argc parameter */
        /* We store argc as the variable __argc in the main function */
        int r = new_reg();
        printf("  %%r%d = load i32, i32* %%__argc\n", r);
        return r;
    }

    if (strcmp(node->type, "ParseArgs") == 0) {
        /* Integer.parseInt(args[idx]) */
        Node *arr = node->child;
        Node *idx = arr ? arr->sibling : NULL;
        int ridx = codegen_expr(idx);
        /* load args[ridx] */
        char *arrname = arr ? arr->value : "args";
        int rptr = new_reg();
        printf("  %%r%d = load i8**, i8*** %%%s\n", rptr, arrname);
        int relem = new_reg();
        printf("  %%r%d = getelementptr inbounds i8*, i8** %%r%d, i32 %%r%d\n", relem, rptr, ridx);
        int rstr = new_reg();
        printf("  %%r%d = load i8*, i8** %%r%d\n", rstr, relem);
        int r = new_reg();
        printf("  %%r%d = call i32 @atoi(i8* %%r%d)\n", r, rstr);
        return r;
    }

    if (strcmp(node->type, "Call") == 0) {
        Node *fname = node->child;
        char *name = fname->value;
        char *ret_type = node->anot_type ? node->anot_type : "void";
        const char *lrt = llvm_type(ret_type);

        /* Build argument list */
        /* Get formal param types from the annotation on fname node */
        /* fname->anot_type is like "(int,double)" */
        char formal_types_str[8192] = "";
        if (fname->anot_type && strlen(fname->anot_type) > 2) {
            /* remove parens */
            strncpy(formal_types_str, fname->anot_type + 1, strlen(fname->anot_type) - 2);
            formal_types_str[strlen(fname->anot_type) - 2] = 0;
        }
        char *formal_tok_arr[512];
        int num_formals = 0;
        if (strlen(formal_types_str) > 0) {
            char *tmp = strdup(formal_types_str);
            char *tok = strtok(tmp, ",");
            while (tok && num_formals < 512) {
                formal_tok_arr[num_formals++] = strdup(tok);
                tok = strtok(NULL, ",");
            }
            free(tmp);
        }

        Node *arg = fname->sibling;
        int arg_regs[512];
        char *arg_types_arr[512];
        int num_args = 0;
        int fi = 0;
        while (arg && num_args < 512) {
            arg_regs[num_args] = codegen_expr(arg);
            arg_types_arr[num_args] = arg->anot_type ? arg->anot_type : "int";
            /* promote int->double if formal is double */
            if (fi < num_formals && strcmp(formal_tok_arr[fi], "double") == 0 &&
                strcmp(arg_types_arr[num_args], "int") == 0) {
                arg_regs[num_args] = emit_to_double(arg_regs[num_args], "int");
                arg_types_arr[num_args] = "double";
            }
            num_args++;
            fi++;
            arg = arg->sibling;
        }
        for (int i = 0; i < num_formals; i++) free(formal_tok_arr[i]);

        int r = 0;
        if (strcmp(ret_type, "void") != 0) r = new_reg();

        /* Build call instruction */
        if (strcmp(ret_type, "void") != 0)
            printf("  %%r%d = call %s @%s(", r, lrt, name);
        else
            printf("  call void @%s(", name);

        for (int i = 0; i < num_args; i++) {
            if (i > 0) printf(", ");
            printf("%s %%r%d", llvm_type(arg_types_arr[i]), arg_regs[i]);
        }
        printf(")\n");
        return r;
    }

    return 0;
}

/* Emit a print call for a value of given type */
static void emit_print(int reg, const char *t) {
    if (strcmp(t, "int") == 0) {
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %%r%d)\n", reg);
    } else if (strcmp(t, "double") == 0) {
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmt_double, i32 0, i32 0), double %%r%d)\n", reg);
    } else if (strcmp(t, "boolean") == 0) {
        /* print "true" or "false" */
        int lbl_true = new_label();
        int lbl_false = new_label();
        int lbl_end = new_label();
        printf("  br i1 %%r%d, label %%lbl%d, label %%lbl%d\n", reg, lbl_true, lbl_false);
        printf("lbl%d:\n", lbl_true);
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.fmt_true, i32 0, i32 0))\n");
        printf("  br label %%lbl%d\n", lbl_end);
        printf("lbl%d:\n", lbl_false);
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmt_false, i32 0, i32 0))\n");
        printf("  br label %%lbl%d\n", lbl_end);
        printf("lbl%d:\n", lbl_end);
    } else if (strcmp(t, "String") == 0) {
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %%r%d)\n", reg);
    }
}

static void codegen_stmt(Node *node) {
    if (!node) return;

    if (strcmp(node->type, "Block") == 0) {
        Node *c = node->child;
        while (c) { codegen_stmt(c); c = c->sibling; }
        return;
    }

    if (strcmp(node->type, "VarDecl") == 0) {
        /* local variable declarations are handled at method entry as alloca */
        return;
    }

    if (strcmp(node->type, "If") == 0) {
        Node *cond = node->child;
        Node *then_br = cond ? cond->sibling : NULL;
        Node *else_br = then_br ? then_br->sibling : NULL;
        int lbl_then = new_label();
        int lbl_else = new_label();
        int lbl_end = new_label();
        int rcond = codegen_expr(cond);
        printf("  br i1 %%r%d, label %%lbl%d, label %%lbl%d\n", rcond, lbl_then, lbl_else);
        printf("lbl%d:\n", lbl_then);
        codegen_stmt(then_br);
        printf("  br label %%lbl%d\n", lbl_end);
        printf("lbl%d:\n", lbl_else);
        if (else_br) codegen_stmt(else_br);
        printf("  br label %%lbl%d\n", lbl_end);
        printf("lbl%d:\n", lbl_end);
        return;
    }

    if (strcmp(node->type, "While") == 0) {
        Node *cond = node->child;
        Node *body = cond ? cond->sibling : NULL;
        int lbl_cond = new_label();
        int lbl_body = new_label();
        int lbl_end = new_label();
        printf("  br label %%lbl%d\n", lbl_cond);
        printf("lbl%d:\n", lbl_cond);
        int rcond = codegen_expr(cond);
        printf("  br i1 %%r%d, label %%lbl%d, label %%lbl%d\n", rcond, lbl_body, lbl_end);
        printf("lbl%d:\n", lbl_body);
        codegen_stmt(body);
        printf("  br label %%lbl%d\n", lbl_cond);
        printf("lbl%d:\n", lbl_end);
        return;
    }

    if (strcmp(node->type, "Return") == 0) {
        if (node->child) {
            int r = codegen_expr(node->child);
            char *t = node->child->anot_type ? node->child->anot_type : "int";
            /* promote to return type if needed */
            if (current_method_return_type && strcmp(current_method_return_type, "double") == 0
                && strcmp(t, "int") == 0) {
                r = emit_to_double(r, "int");
                t = "double";
            }
            printf("  ret %s %%r%d\n", llvm_type(t), r);
        } else {
            printf("  ret void\n");
        }
        /* emit unreachable block to allow subsequent code */
        int dummy = new_label();
        printf("lbl%d:\n", dummy);
        return;
    }

    if (strcmp(node->type, "Assign") == 0) {
        codegen_expr(node);
        return;
    }

    if (strcmp(node->type, "Print") == 0) {
        Node *arg = node->child;
        if (!arg) return;
        int r = codegen_expr(arg);
        char *t = arg->anot_type ? arg->anot_type : "int";
        emit_print(r, t);
        return;
    }

    if (strcmp(node->type, "ParseArgs") == 0) {
        codegen_expr(node);
        return;
    }

    if (strcmp(node->type, "Call") == 0) {
        codegen_expr(node);
        return;
    }

    /* Expression statement */
    if (is_expression(node->type)) {
        codegen_expr(node);
        return;
    }
}

/* Collect all VarDecl nodes in a MethodBody and emit allocas */
static void emit_allocas(Node *body, SymTable *method_table) {
    if (!body) return;
    Node *c = body->child;
    while (c) {
        if (strcmp(c->type, "VarDecl") == 0) {
            Node *type_node = c->child;
            Node *id_node = type_node ? type_node->sibling : NULL;
            if (id_node) {
                char *jt = get_juc_type(type_node->type);
                printf("  %%%s = alloca %s\n", id_node->value, llvm_type(jt));
                printf("  store %s %s, %s* %%%s\n", llvm_type(jt), default_value(jt), llvm_type(jt), id_node->value);
            }
        }
        c = c->sibling;
    }
}

static void codegen_method(Node *node) {
    /* node = MethodDecl */
    Node *header = node->child;
    Node *body = header ? header->sibling : NULL;
    if (!header) return;

    Node *ret_type_node = header->child;
    Node *name_node = ret_type_node ? ret_type_node->sibling : NULL;
    Node *params_node = name_node ? name_node->sibling : NULL; /* MethodParams */

    char *ret_jt = get_juc_type(ret_type_node ? ret_type_node->type : "Void");
    char *mname = name_node ? name_node->value : "unknown";

    current_method_return_type = ret_jt;

    /* Look up method table */
    char *params_str = build_param_types(params_node);
    char table_name[8192];
    snprintf(table_name, sizeof(table_name), "Method %s(%s)", mname, params_str);
    free(params_str);
    current_method_table = all_tables;
    while (current_method_table) {
        if (strcmp(current_method_table->name, table_name) == 0) break;
        current_method_table = current_method_table->next;
    }

    /* Is this main? */
    int is_main = (strcmp(mname, "main") == 0);

    /* Print function signature */
    if (is_main) {
        printf("\ndefine i32 @main(i32 %%argc, i8** %%argv) {\n");
        printf("  %%args = alloca i8**\n");
        /* Skip argv[0] (program name): args points to &argv[1] */
        printf("  %%__args_base = getelementptr inbounds i8*, i8** %%argv, i32 1\n");
        printf("  store i8** %%__args_base, i8*** %%args\n");
        printf("  %%__argc = alloca i32\n");
        /* subtract 1 from argc to get number of program args */
        printf("  %%r%d = sub i32 %%argc, 1\n", new_reg());
        printf("  store i32 %%r%d, i32* %%__argc\n", reg_counter);
    } else {
        printf("\ndefine %s @%s(", llvm_type(ret_jt), mname);
        /* params */
        if (params_node && params_node->child) {
            Node *p = params_node->child;
            int first = 1;
            while (p) {
                Node *pt = p->child;
                Node *pid = pt ? pt->sibling : NULL;
                if (!first) printf(", ");
                first = 0;
                char *pjt = get_juc_type(pt ? pt->type : "Int");
                printf("%s %%%s_arg", llvm_type(pjt), pid ? pid->value : "p");
                p = p->sibling;
            }
        }
        printf(") {\n");

        /* alloca for params */
        if (params_node && params_node->child) {
            Node *p = params_node->child;
            while (p) {
                Node *pt = p->child;
                Node *pid = pt ? pt->sibling : NULL;
                char *pjt = get_juc_type(pt ? pt->type : "Int");
                if (pid) {
                    printf("  %%%s = alloca %s\n", pid->value, llvm_type(pjt));
                    printf("  store %s %%%s_arg, %s* %%%s\n", llvm_type(pjt), pid->value, llvm_type(pjt), pid->value);
                }
                p = p->sibling;
            }
        }
    }

    /* alloca for local vars */
    emit_allocas(body, current_method_table);

    /* Generate statements */
    if (body) {
        Node *c = body->child;
        while (c) {
            if (strcmp(c->type, "VarDecl") != 0) codegen_stmt(c);
            c = c->sibling;
        }
    }

    /* Default return */
    if (is_main) {
        printf("  ret i32 0\n");
    } else if (strcmp(ret_jt, "void") == 0) {
        printf("  ret void\n");
    } else {
        printf("  ret %s %s\n", llvm_type(ret_jt), default_value(ret_jt));
    }

    printf("}\n");
}

static void prescan_strlits(Node *n) {
    if (!n) return;
    if (strcmp(n->type, "StrLit") == 0) register_strlit(n->value);
    Node *ch = n->child;
    while (ch) { prescan_strlits(ch); ch = ch->sibling; }
}

static void codegen_program(Node *prog) {
    if (!prog) return;

    class_name = prog->child ? prog->child->value : "Main";

    /* Print module header */
    printf("; Juc compiler - LLVM IR output\n");
    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n");
    printf("\n");

    /* Format strings */
    printf("@.fmt_int    = private constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.fmt_double = private constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.fmt_str    = private constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.fmt_true   = private constant [5 x i8] c\"true\\00\"\n");
    printf("@.fmt_false  = private constant [6 x i8] c\"false\\00\"\n");
    printf("\n");

    /* Pre-scan AST for StrLit nodes to register string constants before emitting code */
    Node *c = prog->child ? prog->child->sibling : NULL;
    while (c) {
        prescan_strlits(c);
        c = c->sibling;
    }

    /* Emit string constants */
    for (int i = 0; i < num_str_consts; i++) {
        printf("@.str%d = private constant [%d x i8] c\"%s\"\n", str_const_ids[i], str_const_len[i], str_consts[i]);
    }

    /* Global variables */
    c = prog->child ? prog->child->sibling : NULL;
    while (c) {
        if (strcmp(c->type, "FieldDecl") == 0) {
            Node *type_node = c->child;
            Node *id_node = type_node ? type_node->sibling : NULL;
            if (id_node) {
                char *jt = get_juc_type(type_node->type);
                printf("@%s = global %s %s\n", id_node->value, llvm_type(jt), default_value(jt));
            }
        }
        c = c->sibling;
    }

    /* Methods */
    c = prog->child ? prog->child->sibling : NULL;
    while (c) {
        if (strcmp(c->type, "MethodDecl") == 0) {
            reg_counter = 0;
            label_counter = 0;
            codegen_method(c);
        }
        c = c->sibling;
    }
}

int semantic_errors_count = 0;

%}

%union {
    struct {
        char* str;
        int line;
        int col;
    } token;
    struct _node* node;
}

%token <token> BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE
%token <token> AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR
%token <token> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type <node> Program ProgramElements MethodDecl FieldDecl Type MethodHeader 
%type <node> OptFormalParams FormalParams FormalParamsRest MethodBody 
%type <node> StatementAndVarDeclList VarDecl IdList StatementList Statement 
%type <node> OptExpr ExprOrStrlit MethodInvocation OptExprList ExprList 
%type <node> ParseArgs Expr
%type <token> AnyId

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
%start Program

%%

AnyId: IDENTIFIER { $$ = $1; }
     ;

Program : CLASS AnyId LBRACE ProgramElements RBRACE {
            $$ = create_node("Program", NULL, $1.line, $1.col);
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
            $$ = create_node("MethodDecl", NULL, $1.line, $1.col);
            add_child($$, $3);
            add_sibling($3, $4);
        }
           ;

FieldDecl : PUBLIC STATIC Type AnyId IdList SEMICOLON {
            $$ = create_node("FieldDecl", NULL, $1.line, $1.col);
            add_child($$, $3);
            add_child($$, create_node("Identifier", $4.str, $4.line, $4.col));
            Node* aux = $5;
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_field = create_node("FieldDecl", NULL, $1.line, $1.col);
                add_child(new_field, create_node($3->type, NULL, $3->line, $3->col)); 
                add_child(new_field, aux);
                $$ = add_sibling($$, new_field);
                aux = next;
            }
          }
          | error SEMICOLON { $$ = NULL; }
          ;

VarDecl : Type AnyId IdList SEMICOLON {
            $$ = create_node("VarDecl", NULL, $1->line, $1->col);
            add_child($$, $1);
            add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
            Node* aux = $3;
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_var = create_node("VarDecl", NULL, $1->line, $1->col);
                add_child(new_var, create_node($1->type, NULL, $1->line, $1->col));
                add_child(new_var, aux);
                $$ = add_sibling($$, new_var);
                aux = next;
            }
        }
        ;

IdList :{ $$ = NULL; }
       | IdList COMMA AnyId {   $$ = add_sibling($1, create_node("Identifier", $3.str, $3.line, $3.col)); }
       ;

Type : BOOL   { $$ = create_node("Bool", NULL, $1.line, $1.col); }
     | INT    { $$ = create_node("Int", NULL, $1.line, $1.col); }
     | DOUBLE { $$ = create_node("Double", NULL, $1.line, $1.col); }
     ;

MethodHeader : Type AnyId LPAR OptFormalParams RPAR {
                $$ = create_node("MethodHeader", NULL, $1->line, $1->col);
                add_child($$, $1);
                add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
                
                Node* params = create_node("MethodParams", NULL, $3.line, $3.col);
                if ($4) add_child(params, $4);
                add_child($$, params);
             }
             | VOID AnyId LPAR OptFormalParams RPAR {
                $$ = create_node("MethodHeader", NULL, $1.line, $1.col);
                Node* v = create_node("Void", NULL, $1.line, $1.col);
                add_child($$, v);
                add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
                Node* params = create_node("MethodParams", NULL, $3.line, $3.col);
                if ($4) add_child(params, $4);
                add_child($$, params);
             }
             ;

OptFormalParams : { $$ = NULL; }
                | FormalParams { $$ = $1; }
                ;

FormalParams : Type AnyId FormalParamsRest {
                $$ = create_node("ParamDecl", NULL, $1->line, $1->col);
                add_child($$, $1);
                add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
                if ($3) add_sibling($$, $3);
             }
             | STRING LSQ RSQ AnyId {
                $$ = create_node("ParamDecl", NULL, $1.line, $1.col);
                Node* strArray = create_node("StringArray", NULL, $1.line, $1.col);
                add_child($$, strArray);
                add_child($$, create_node("Identifier", $4.str, $4.line, $4.col));
             }
             ;

FormalParamsRest : { $$ = NULL; }
                 | FormalParamsRest COMMA Type AnyId {
                    Node* param = create_node("ParamDecl", NULL, $3->line, $3->col);
                    add_child(param, $3);
                    add_child(param, create_node("Identifier", $4.str, $4.line, $4.col));
                    $$ = add_sibling($1, param);
                 }
                 ;

MethodBody : LBRACE StatementAndVarDeclList RBRACE {
                $$ = create_node("MethodBody", NULL, $1.line, $1.col);
                if ($2) add_child($$, $2);
           }
           ;

StatementAndVarDeclList : { $$ = NULL; }
                        | StatementAndVarDeclList Statement {
                            if ($2 != NULL) $$ = add_sibling($1, $2);
                            else $$ = $1;
                        }
                        | StatementAndVarDeclList VarDecl { $$ = add_sibling($1, $2); }
                        ;

StatementList : { $$ = NULL; }
              | StatementList Statement {
                  if ($2 != NULL) $$ = add_sibling($1, $2);
                  else $$ = $1;
              }
              ;

Statement : LBRACE StatementList RBRACE {
              if ($2 != NULL && $2->sibling != NULL) {
                  $$ = create_node("Block", NULL, $1.line, $1.col);
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
              add_child($$, $5 ? $5 : create_node("Block", NULL, $1.line, $1.col)); 
              add_child($$, create_node("Block", NULL, $1.line, $1.col));
          }
          | IF LPAR Expr RPAR Statement ELSE Statement {
              $$ = create_node("If", NULL, $1.line, $1.col);
              add_child($$, $3);
              add_child($$, $5 ? $5 : create_node("Block", NULL, $1.line, $1.col));
              add_child($$, $7 ? $7 : create_node("Block", NULL, $1.line, $1.col));
          }
          | WHILE LPAR Expr RPAR Statement {
              $$ = create_node("While", NULL, $1.line, $1.col);
              add_child($$, $3);
              add_child($$, $5 ? $5 : create_node("Block", NULL, $1.line, $1.col));
          }
          | RETURN OptExpr SEMICOLON {
              $$ = create_node("Return", NULL, $1.line, $1.col);
              if ($2) add_child($$, $2);
          }
          | MethodInvocation SEMICOLON { $$ = $1; }
          | ParseArgs SEMICOLON { $$ = $1; }
          | Expr ASSIGN Expr SEMICOLON { 
              if ($1 && strcmp($1->type, "Identifier") != 0) {
                  printf("Line %d, col %d: syntax error: =\n", $2.line, $2.col);
                  syntax_errors_count++;
              }
              $$ = create_node("Assign", NULL, $2.line, $2.col);
              add_child($$, $1);
              add_child($$, $3);
          }
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

MethodInvocation : AnyId LPAR OptExprList RPAR {
                     $$ = create_node("Call", NULL, $1.line, $1.col);
                     Node* id = create_node("Identifier", $1.str, $1.line, $1.col);
                     add_child($$, id);
                     if ($3) add_child($$, $3);
                 }
                 | AnyId LPAR error RPAR { $$ = NULL; }
                 ;

OptExprList : { $$ = NULL; }
            | ExprList { $$ = $1; }
            ;

ExprList : Expr { $$ = $1; }
         | ExprList COMMA Expr { $$ = add_sibling($1, $3); }
         ;

ParseArgs : PARSEINT LPAR AnyId LSQ Expr RSQ RPAR {
              $$ = create_node("ParseArgs", NULL, $1.line, $1.col);
              Node* id = create_node("Identifier", $3.str, $3.line, $3.col);
              add_child($$, id);
              add_child($$, $5);
          }
          | PARSEINT LPAR error RPAR { $$ = NULL; }
          ;

Expr : Expr ASSIGN Expr { 
          if ($1 && strcmp($1->type, "Identifier") != 0) {
              printf("Line %d, col %d: syntax error: =\n", $2.line, $2.col);
              syntax_errors_count++;
          }
          $$ = create_node("Assign", NULL, $2.line, $2.col);
          add_child($$, $1);
          add_child($$, $3);
     }
     | Expr PLUS Expr   { $$ = create_node("Add", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
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
     | Expr NE Expr     { $$ = create_node("Ne", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr LT Expr     { $$ = create_node("Lt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr LE Expr     { $$ = create_node("Le", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr GT Expr     { $$ = create_node("Gt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr GE Expr     { $$ = create_node("Ge", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | PLUS Expr %prec UNARY_PLUS   { $$ = create_node("Plus", NULL, $1.line, $1.col); add_child($$, $2); }
     | MINUS Expr %prec UNARY_MINUS { $$ = create_node("Minus", NULL, $1.line, $1.col); add_child($$, $2); }
     | NOT Expr         { $$ = create_node("Not", NULL, $1.line, $1.col); add_child($$, $2); }
     | MethodInvocation { $$ = $1; }
     | ParseArgs        { $$ = $1; }
     | AnyId            { $$ = create_node("Identifier", $1.str, $1.line, $1.col); }
     | NATURAL          { $$ = create_node("Natural", $1.str, $1.line, $1.col); }
     | DECIMAL          { $$ = create_node("Decimal", $1.str, $1.line, $1.col); }
     | BOOLLIT          { $$ = create_node("BoolLit", $1.str, $1.line, $1.col); }
     | AnyId DOTLENGTH  { $$ = create_node("Length", NULL, $2.line, $2.col); add_child($$, create_node("Identifier", $1.str, $1.line, $1.col)); }
     | LPAR Expr RPAR   { $$ = $2; }
     | LPAR error RPAR  { $$ = NULL; }
     ;
%%
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
    int flag_e3 = 0;
    int flag_codegen = 1; /* generate code if no option given */

    if (argc > 1) {
        flag_codegen = 0;
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
            /* syntax errors only */
        } else if (strcmp(argv[1], "-s") == 0) {
            flag_s = 1;
        } else if (strcmp(argv[1], "-e3") == 0) {
            flag_e3 = 1;
        }
    }

    yyparse();

    if (syntax_errors_count == 0) {
        if (flag_syntax) {
            print_tree(root, 0);
        } else if (flag_s) {
            annotate_ast(root, NULL);
            print_tables();
            print_tree(root, 0);
        } else if (flag_e3) {
            annotate_ast(root, NULL);
        } else if (flag_codegen) {
            /* Run semantics, capture errors. If none, generate LLVM IR. */
            FILE *tmpf = tmpfile();
            if (!tmpf) {
                annotate_ast(root, NULL);
                codegen_program(root);
                return 0;
            }
            int saved_fd = dup(1);
            dup2(fileno(tmpf), 1);
            fflush(stdout);
            annotate_ast(root, NULL);
            fflush(stdout);
            dup2(saved_fd, 1);
            close(saved_fd);
            fseek(tmpf, 0, SEEK_END);
            long fsize = ftell(tmpf);
            fseek(tmpf, 0, SEEK_SET);
            if (fsize > 0) {
                /* Semantic errors present - print them and exit */
                char buf[4096];
                size_t n;
                while ((n = fread(buf, 1, sizeof(buf), tmpf)) > 0) {
                    fwrite(buf, 1, n, stdout);
                }
                fclose(tmpf);
                return 0;
            }
            fclose(tmpf);
            /* No semantic errors - AST is annotated, generate code */
            codegen_program(root);
        }
    }
    return 0;
}