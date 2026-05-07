/* Daniel José Mendes Ribeiro nº 2023208712*/
/* Matilde de Oliveira Tavares Ângelo nº2023210357 */
%{

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
    char *res_words[] = {"++","--","null","Integer","System","abstract","assert","break","byte","case","catch","char","const","continue","default","do","enum","extends","final","finally","float","for","goto","implements","import","instanceof","interface","long","native","new","package","private","protected","short","strictfp","super","switch","synchronized","this","throw","throws","transient","try","volatile","_", NULL};
    for (int i=0; res_words[i]; i++) {
        if (strcmp(name, res_words[i]) == 0) {
            printf("Line %d, col %d: Symbol %s is reserved\n", line, col, name);
            return 0;
        }
    }
    Symbol *curr = table->symbols;
    Symbol *prev = NULL;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            if ((!param_types && !curr->param_types) || (param_types && curr->param_types && strcmp(curr->param_types, param_types) == 0)) {
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
            insert_symbol(current_env, id_node->value, get_juc_type(node->child->type), NULL, 0, id_node->line, id_node->col);
        }
        return;
    }

    if (strcmp(node->type, "Program") == 0) {
        global_table = create_table("Class", "Global");
        Node *child = node->child ? node->child->sibling : NULL;
        Node *temp = child;
        while (temp) {
            if (strcmp(temp->type, "MethodDecl") == 0) {
                Node *header = temp->child;
                Node *id_node = header->child->sibling;
                char *params_str = build_param_types(id_node->sibling);
                insert_symbol(global_table, id_node->value, get_juc_type(header->child->type), params_str, 0, id_node->line, id_node->col);
                
                // Build method table here so parameter errors are printed first
                char table_name[8192];
                snprintf(table_name, sizeof(table_name), "Method %s(%s)", id_node->value, params_str);
                
                int redefined = 0;
                Symbol *s = global_table->symbols;
                while (s) {
                    if (strcmp(s->name, id_node->value) == 0 && s->param_types && strcmp(s->param_types, params_str) == 0) {
                        if (s->line < id_node->line || (s->line == id_node->line && s->col < id_node->col)) {
                            redefined = 1;
                            break;
                        }
                    }
                    s = s->next;
                }
                
                if (!redefined) {
                    SymTable *method_table = create_table(table_name, "Method");
                    insert_symbol(method_table, "return", get_juc_type(header->child->type), NULL, 0, id_node->line, id_node->col);
                    Node *params = id_node->sibling->child;
                    while (params) {
                        insert_symbol(method_table, params->child->sibling->value, get_juc_type(params->child->type), NULL, 1, params->child->sibling->line, params->child->sibling->col);
                        params = params->sibling;
                    }
                }
                free(params_str);
            } else if (strcmp(temp->type, "FieldDecl") == 0) {
                Node *id_node = temp->child->sibling;
                insert_symbol(global_table, id_node->value, get_juc_type(temp->child->type), NULL, 0, id_node->line, id_node->col);
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
                if (s->line < id_node->line || (s->line == id_node->line && s->col < id_node->col)) {
                    redefined = 1;
                    break;
                }
            }
            s = s->next;
        }
        
        if (!redefined) {
            SymTable *method_table = all_tables;
            while (method_table) {
                if (strcmp(method_table->name, table_name) == 0) break;
                method_table = method_table->next;
            }
            if (node->child->sibling) annotate_ast(node->child->sibling, method_table);
        }
        free(params_str);
        return;
    }

    Node *child = node->child;
    while (child) {
        if (strcmp(node->type, "Call") == 0 && child == node->child) {
            // Skips Call identifier
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
        
        if (!valid && strcmp(t1, "undef") != 0 && strcmp(t2, "undef") != 0) printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", node->line, node->col, t1, t2);
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
    } else if (strcmp(node->type, "And") == 0 || strcmp(node->type, "Or") == 0 || strcmp(node->type, "Xor") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        int valid = 0;
        char *res = "boolean";
        if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) { valid = 1; res = "boolean"; }
        else if (strcmp(node->type, "Xor") == 0 && strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) { valid = 1; res = "int"; }
        else if (strcmp(node->type, "Xor") == 0) { res = "undef"; }
        if (!valid) printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, get_op_str(node->type), t1, t2);
        node->anot_type = strdup(res);
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
        if (!valid) printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, get_op_str(node->type), t1, t2);
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
        if (strcmp(t1, "boolean") == 0) node->anot_type = strdup("boolean");
        else {
            printf("Line %d, col %d: Operator ! cannot be applied to type %s\n", node->line, node->col, t1);
            node->anot_type = strdup("undef");
        }
    } else if (strcmp(node->type, "Length") == 0) {
        char *t1 = (node->child && node->child->anot_type) ? node->child->anot_type : "none";
        if (strcmp(t1, "String[]") != 0) printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", node->line, node->col, t1);
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
            printf("Line %d, col %d: Reference to method %s is ambiguous\n", el, ec, method_name);
            node->anot_type = strdup("undef");
            node->child->anot_type = strdup("undef");
        } else {
            printf("Line %d, col %d: Cannot find symbol %s\n", el, ec, method_name);
            node->anot_type = strdup("undef");
            node->child->anot_type = strdup("undef");
        }
    } else if (strcmp(node->type, "ParseArgs") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        if (strcmp(t1, "String[]") != 0 || strcmp(t2, "int") != 0) printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", node->line, node->col, t1, t2);
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
    } else if (strcmp(node->type, "Lshift") == 0 || strcmp(node->type, "Rshift") == 0) {
        Node *c1 = node->child;
        Node *c2 = c1 ? c1->sibling : NULL;
        char *t1 = (c1 && c1->anot_type) ? c1->anot_type : "none";
        char *t2 = (c2 && c2->anot_type) ? c2->anot_type : "none";
        if (strcmp(t1, "int") != 0 || strcmp(t2, "int") != 0) {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", node->line, node->col, strcmp(node->type, "Lshift") == 0 ? "<<" : ">>", t1, t2);
        }
        node->anot_type = strdup((strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) ? "int" : "undef");
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
            else printf("%s\t%s%s\n", s->name, s->type, s->is_param ? "\tparam" : "");
            s = s->next;
        }
        printf("\n");
        t = t->next;
    }
}

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
%type <node> ParseArgs Expr OptDotLength
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
     | RESERVED { $$ = $1; }
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
     | Expr GE Expr     { $$ = create_node("Ge", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr GT Expr     { $$ = create_node("Gt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr LE Expr     { $$ = create_node("Le", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr LT Expr     { $$ = create_node("Lt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | Expr NE Expr     { $$ = create_node("Ne", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
     | MINUS Expr %prec UNARY_MINUS { $$ = create_node("Minus", NULL, $1.line, $1.col); add_child($$, $2); }
     | PLUS Expr %prec UNARY_PLUS   { $$ = create_node("Plus", NULL, $1.line, $1.col); add_child($$, $2); }
     | NOT Expr         { $$ = create_node("Not", NULL, $1.line, $1.col); add_child($$, $2); }
     | LPAR Expr RPAR  { $$ = $2; }
     | LPAR error RPAR  { $$ = NULL; }
     | MethodInvocation { $$ = $1; }
     | ParseArgs        { $$ = $1; }
     | AnyId OptDotLength {
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
             | DOTLENGTH { $$ = create_node("DotLength", NULL, $1.line, $1.col); }
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
void print_tree(Node* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("..");
    
    if (node->value) printf("%s(%s)", node->type, node->value);
    else printf("%s", node->type);
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
            // Error detection only, default behavior
        } else if (strcmp(argv[1], "-s") == 0) {
            flag_s = 1;
        } else if (strcmp(argv[1], "-e3") == 0) {
            // Semantic analysis only (errors only), which is the default behavior if no other flags are set
        }
    }
    
    yyparse();
    if (syntax_errors_count == 0) {
        if (flag_syntax) {
            print_tree(root, 0);
        } else {
            annotate_ast(root, NULL);
            if (flag_s) {
                print_tables();
                print_tree(root, 0);
            }
        }
    }
    return 0;
}