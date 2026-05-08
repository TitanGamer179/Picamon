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

char* get_java_type(char* ast_type) {
    if (!ast_type) return "undef";
    if (strcmp(ast_type, "Int") == 0) return "int";
    if (strcmp(ast_type, "Double") == 0) return "double";
    if (strcmp(ast_type, "Bool") == 0) return "boolean";
    if (strcmp(ast_type, "Void") == 0) return "void";
    if (strcmp(ast_type, "StringArray") == 0) return "String[]";
    return "undef";
}

typedef struct _symbol {
    char name[100];
    char type[100];
    char param_types[200]; 
    int is_param;         
    struct _symbol* next;
} Symbol;

typedef struct _symtable {
    char name[200];        
    Symbol* first;
    Symbol* last;
    struct _symtable* next;
} SymTable;

SymTable* global_table = NULL;
SymTable* local_tables_head = NULL;
SymTable* local_tables_tail = NULL;

int flag_lex = 0;
int flag_syntax = 0;
int flag_errors = 0;
int syntax_errors_count = 0;
int flag_semantic = 0;        // <-- ADD THIS
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

SymTable* create_table(char* name) {
    SymTable* t = (SymTable*)malloc(sizeof(SymTable));
    strcpy(t->name, name);
    t->first = NULL;
    t->last = NULL;
    t->next = NULL;
    return t;
}

void add_symbol(SymTable* table, char* name, char* type, char* param_types, int is_param) {
    Symbol* s = (Symbol*)malloc(sizeof(Symbol));
    strcpy(s->name, name);
    strcpy(s->type, type);
    if (param_types) strcpy(s->param_types, param_types);
    else strcpy(s->param_types, "");
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

void parse_method_declarations(Node* root) {
    if (!root || strcmp(root->type, "Program") != 0) return;

    // 1. Criar Tabela Global
    Node* class_id_node = root->child; // O primeiro filho do Program é o Identifier da Classe
    char global_name[250];
    sprintf(global_name, "Class %s", class_id_node->value);
    global_table = create_table(global_name);

    // 2. Percorrer os filhos do Program à procura de MethodDecl
    Node* curr = class_id_node->sibling;
    while (curr != NULL) {
        if (strcmp(curr->type, "MethodDecl") == 0) {
            Node* method_header = curr->child;
            
            // method_header -> child 1: Type
            // method_header -> child 2: Identifier
            // method_header -> child 3: MethodParams
            Node* return_type_node = method_header->child;
            Node* method_id_node = return_type_node->sibling;
            Node* method_params_node = method_id_node->sibling;

            char* ret_type_java = get_java_type(return_type_node->type);
            char* method_name = method_id_node->value;

            // Extrair tipos dos parâmetros para formatar como "(int, double)"
            char param_str[200] = "(";
            Node* param = method_params_node->child; // Primeiro ParamDecl
            int first_param = 1;

            while (param != NULL) {
                if (!first_param) strcat(param_str, ", ");
                strcat(param_str, get_java_type(param->child->type));
                first_param = 0;
                param = param->sibling;
            }
            strcat(param_str, ")");

            // Adicionar à Tabela Global: name \t (params) \t type
            add_symbol(global_table, method_name, ret_type_java, param_str, 0);

            // 3. Criar Tabela Local para este método
            char local_name[250];
            sprintf(local_name, "Method %s%s", method_name, param_str);
            SymTable* local_table = create_table(local_name);

            // Adicionar o símbolo especial "return" exigido pela Meta 3
            add_symbol(local_table, "return", ret_type_java, NULL, 0);

            // Adicionar os parâmetros à tabela local com a flag 'param' a 1
            param = method_params_node->child;
            while (param != NULL) {
                Node* param_type_node = param->child;
                Node* param_id_node = param_type_node->sibling;
                add_symbol(local_table, param_id_node->value, get_java_type(param_type_node->type), NULL, 1);
                param = param->sibling;
            }

            // Nota: Se quiseres processar as variáveis locais (VarDecl) no MethodBody, 
            // basta fazeres um loop em curr->child->sibling (MethodBody) aqui!
            
            add_local_table(local_table);
        }
        curr = curr->sibling;
    }
}


#line 270 "y.tab.c"

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
#line 202 "jucompiler.y"

    char* str;
    struct _node* node;

#line 434 "y.tab.c"

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
  YYSYMBOL_Program = 55,                   /* Program  */
  YYSYMBOL_ProgramElements = 56,           /* ProgramElements  */
  YYSYMBOL_MethodDecl = 57,                /* MethodDecl  */
  YYSYMBOL_FieldDecl = 58,                 /* FieldDecl  */
  YYSYMBOL_VarDecl = 59,                   /* VarDecl  */
  YYSYMBOL_IdList = 60,                    /* IdList  */
  YYSYMBOL_Type = 61,                      /* Type  */
  YYSYMBOL_MethodHeader = 62,              /* MethodHeader  */
  YYSYMBOL_OptFormalParams = 63,           /* OptFormalParams  */
  YYSYMBOL_FormalParams = 64,              /* FormalParams  */
  YYSYMBOL_FormalParamsRest = 65,          /* FormalParamsRest  */
  YYSYMBOL_MethodBody = 66,                /* MethodBody  */
  YYSYMBOL_StatementAndVarDeclList = 67,   /* StatementAndVarDeclList  */
  YYSYMBOL_StatementList = 68,             /* StatementList  */
  YYSYMBOL_Statement = 69,                 /* Statement  */
  YYSYMBOL_OptExpr = 70,                   /* OptExpr  */
  YYSYMBOL_ExprOrStrlit = 71,              /* ExprOrStrlit  */
  YYSYMBOL_MethodInvocation = 72,          /* MethodInvocation  */
  YYSYMBOL_OptExprList = 73,               /* OptExprList  */
  YYSYMBOL_ExprList = 74,                  /* ExprList  */
  YYSYMBOL_Assignment = 75,                /* Assignment  */
  YYSYMBOL_AExpr = 76,                     /* AExpr  */
  YYSYMBOL_ParseArgs = 77,                 /* ParseArgs  */
  YYSYMBOL_Expr = 78,                      /* Expr  */
  YYSYMBOL_OptDotLength = 79               /* OptDotLength  */
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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   390

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  84
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

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
       0,   233,   233,   240,   241,   242,   243,   246,   253,   268,
     271,   288,   289,   292,   293,   294,   297,   306,   318,   319,
     322,   328,   336,   337,   345,   351,   352,   359,   363,   364,
     373,   383,   389,   395,   400,   404,   405,   406,   407,   411,
     412,   415,   416,   419,   420,   423,   429,   432,   433,   436,
     437,   440,   448,   454,   457,   463,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     497,   498,   499,   502,   503
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
  "UNARY_PLUS", "UNARY_MINUS", "$accept", "Program", "ProgramElements",
  "MethodDecl", "FieldDecl", "VarDecl", "IdList", "Type", "MethodHeader",
  "OptFormalParams", "FormalParams", "FormalParamsRest", "MethodBody",
  "StatementAndVarDeclList", "StatementList", "Statement", "OptExpr",
  "ExprOrStrlit", "MethodInvocation", "OptExprList", "ExprList",
  "Assignment", "AExpr", "ParseArgs", "Expr", "OptDotLength", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-71)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-48)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      25,   -19,    36,    14,   -71,   -71,    22,    10,    30,   -71,
     -71,   -71,   -71,   -71,    11,   -71,   -71,   -71,     1,    15,
      37,    42,    45,   -71,   -71,    52,    52,   -10,   154,    35,
      31,    40,   -71,    41,    32,   -71,    46,    54,    61,    66,
     181,    67,   -71,   -71,   -71,     9,   -71,    51,   -71,    64,
      65,    68,    70,   -71,   -71,   -71,   -71,   -71,   181,   159,
       6,    38,   203,   203,   203,     5,   -71,   -71,   -71,    71,
     -71,   -71,   -71,   233,   181,    80,   181,     7,   -71,   -71,
     -71,   -71,    74,    86,    72,   -71,    75,   -71,    77,    83,
      88,    91,     4,   -71,   -71,   -71,   -71,   181,   -71,   -71,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,    92,   -71,   -71,   -71,
      93,    95,   100,   -71,    -9,   -71,    62,   171,    84,   -71,
     181,   -71,   -71,   -71,   285,   -71,   -71,   334,   347,   347,
     347,   347,    -7,   -71,   334,   260,    -7,    96,    96,   310,
     171,   -71,   -71,   181,   -71,   111,   129,   -71,    98,   -71,
     -71,   -71,   171,   120,   -71,   -71
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     3,     0,     0,     0,     2,
       6,     4,     5,     9,     0,    13,    15,    14,     0,     0,
       0,     0,    11,    25,     7,    18,    18,     0,     0,     0,
       0,     0,    19,     0,     0,     8,     0,     0,     0,     0,
      41,     0,    28,    24,    40,     0,    27,     0,    26,     0,
       0,     0,     0,    22,    17,    16,    12,    39,     0,     0,
       0,     0,     0,     0,     0,    83,    80,    81,    82,     0,
      77,    42,    78,    53,     0,     0,     0,     0,    11,    35,
      36,    37,     0,    20,     0,    44,     0,    43,     0,     0,
       0,     0,    83,    72,    74,    73,    84,     0,    79,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    30,    29,    51,
       0,     0,    48,    49,     0,    21,     0,     0,     0,    55,
       0,    76,    75,    52,    61,    58,    59,    66,    67,    68,
      69,    70,    57,    60,    71,    62,    56,    64,    65,    63,
       0,    46,    45,     0,    10,     0,    31,    38,     0,    33,
      50,    23,     0,     0,    32,    54
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -71,   -71,   -71,   -71,   -71,   -71,    60,   -12,   -71,   133,
     -71,   -71,   -71,   -71,   -71,   -70,   -71,   -71,   -28,   -71,
     -71,   -71,   -55,   -27,    39,   -71
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     6,    11,    12,    46,    27,    30,    20,    31,
      32,    83,    24,    28,    75,    48,    69,    86,    70,   121,
     122,    50,    71,    72,    73,    98
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    51,    19,    84,    87,   118,    91,    88,   120,    96,
      96,    34,    34,   101,    15,   102,    47,    16,    39,   116,
      17,   119,   123,     7,    97,   109,     3,    18,    76,     1,
      35,   154,    77,    77,     8,    61,     4,    77,    62,    90,
       5,    63,   133,    64,    14,   -47,    21,    49,    51,    39,
      13,    89,    65,    66,    67,    15,    68,   156,    16,     9,
      22,    17,    10,    23,    52,    15,    61,    29,    16,    62,
      25,    17,    63,    26,    64,   158,    53,    56,    54,    55,
     159,    36,    58,    65,    66,    67,    57,    68,    37,    59,
      38,    39,   164,    40,    60,    74,    78,    41,   160,    49,
      51,    93,    94,    95,    79,    80,    42,   126,    81,    82,
     127,    99,   130,   128,   155,   129,   101,   117,   102,   125,
      44,   153,    49,    51,   157,    45,   131,   108,   109,   132,
     150,   151,   112,   152,    49,    51,   162,   163,   124,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    36,   161,    15,   165,    33,
      16,     0,    37,    17,    38,    39,     0,    40,     0,     0,
      39,    41,    36,     0,     0,     0,     0,     0,     0,    37,
      42,    38,    39,     0,    40,     0,     0,    61,    41,     0,
      62,    43,    39,    63,    44,    64,     0,    42,     0,    45,
       0,     0,     0,     0,    65,    66,    67,    85,    68,    61,
       0,    44,    62,     0,    39,    63,    45,    64,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,     0,
      68,    61,     0,     0,    62,     0,     0,    63,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    66,
      67,   100,    68,   101,     0,   102,   103,   104,   105,     0,
     106,     0,     0,   107,   108,   109,   110,     0,   111,   112,
       0,     0,     0,     0,     0,   113,   114,   115,   100,     0,
     101,     0,   102,   103,   104,   105,     0,   106,     0,     0,
     107,   108,   109,   110,     0,     0,   112,     0,     0,     0,
       0,     0,   113,   114,   115,   101,     0,   102,   103,   104,
     105,     0,   106,     0,     0,   107,   108,   109,   110,     0,
       0,   112,     0,     0,     0,     0,     0,   113,   114,   115,
     101,     0,   102,   103,   104,   105,     0,   106,     0,     0,
     107,   108,   109,   110,     0,     0,   112,     0,     0,     0,
       0,     0,   113,   114,   101,     0,   102,     0,   104,   105,
       0,   106,     0,     0,   107,   108,   109,   101,     0,   102,
     112,     0,     0,     0,     0,     0,   113,   114,   108,   109,
       0,     0,     0,   112,     0,     0,     0,     0,     0,   113,
     114
};

static const yytype_int16 yycheck[] =
{
      28,    28,    14,    58,    59,    75,    61,     1,     1,     5,
       5,    21,    21,    20,     3,    22,    28,     6,    11,    74,
       9,    76,    77,     1,    19,    32,    45,    16,    19,     4,
      40,    40,    28,    28,    12,    28,     0,    28,    31,     1,
      26,    34,    97,    36,    14,    38,    45,    75,    75,    11,
      40,    45,    45,    46,    47,     3,    49,   127,     6,    37,
      45,     9,    40,    26,    29,     3,    28,    15,     6,    31,
      28,     9,    34,    28,    36,   130,    45,    45,    38,    38,
     150,     1,    28,    45,    46,    47,    40,    49,     8,    28,
      10,    11,   162,    13,    28,    28,    45,    17,   153,   127,
     127,    62,    63,    64,    40,    40,    26,    21,    40,    39,
      38,    40,    29,    38,   126,    38,    20,    37,    22,    45,
      40,    21,   150,   150,    40,    45,    38,    31,    32,    38,
      38,    38,    36,    38,   162,   162,     7,    39,    78,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,     1,    45,     3,    38,    26,
       6,    -1,     8,     9,    10,    11,    -1,    13,    -1,    -1,
      11,    17,     1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      26,    10,    11,    -1,    13,    -1,    -1,    28,    17,    -1,
      31,    37,    11,    34,    40,    36,    -1,    26,    -1,    45,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    28,
      -1,    40,    31,    -1,    11,    34,    45,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    -1,
      49,    28,    -1,    -1,    31,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    18,    49,    20,    -1,    22,    23,    24,    25,    -1,
      27,    -1,    -1,    30,    31,    32,    33,    -1,    35,    36,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    18,    -1,
      20,    -1,    22,    23,    24,    25,    -1,    27,    -1,    -1,
      30,    31,    32,    33,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    20,    -1,    22,    23,    24,
      25,    -1,    27,    -1,    -1,    30,    31,    32,    33,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      20,    -1,    22,    23,    24,    25,    -1,    27,    -1,    -1,
      30,    31,    32,    33,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    42,    43,    20,    -1,    22,    -1,    24,    25,
      -1,    27,    -1,    -1,    30,    31,    32,    20,    -1,    22,
      36,    -1,    -1,    -1,    -1,    -1,    42,    43,    31,    32,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    42,
      43
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    55,    45,     0,    26,    56,     1,    12,    37,
      40,    57,    58,    40,    14,     3,     6,     9,    16,    61,
      62,    45,    45,    26,    66,    28,    28,    60,    67,    15,
      61,    63,    64,    63,    21,    40,     1,     8,    10,    11,
      13,    17,    26,    37,    40,    45,    59,    61,    69,    72,
      75,    77,    29,    45,    38,    38,    45,    40,    28,    28,
      28,    28,    31,    34,    36,    45,    46,    47,    49,    70,
      72,    76,    77,    78,    28,    68,    19,    28,    45,    40,
      40,    40,    39,    65,    76,    48,    71,    76,     1,    45,
       1,    76,    45,    78,    78,    78,     5,    19,    79,    40,
      18,    20,    22,    23,    24,    25,    27,    30,    31,    32,
      33,    35,    36,    42,    43,    44,    76,    37,    69,    76,
       1,    73,    74,    76,    60,    45,    21,    38,    38,    38,
      29,    38,    38,    76,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      38,    38,    38,    21,    40,    61,    69,    40,    76,    69,
      76,    45,     7,    39,    69,    38
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    56,    56,    56,    56,    57,    58,    58,
      59,    60,    60,    61,    61,    61,    62,    62,    63,    63,
      64,    64,    65,    65,    66,    67,    67,    67,    68,    68,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    70,    70,    71,    71,    72,    72,    73,    73,    74,
      74,    75,    76,    76,    77,    77,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     0,     2,     2,     2,     4,     6,     2,
       4,     0,     3,     1,     1,     1,     5,     5,     0,     1,
       3,     4,     0,     4,     3,     0,     2,     2,     0,     2,
       3,     5,     7,     5,     3,     2,     2,     2,     5,     2,
       1,     0,     1,     1,     1,     4,     4,     0,     1,     1,
       3,     3,     3,     1,     7,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     3,     3,     1,     1,     2,
       1,     1,     1,     0,     1
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
  case 2: /* Program: CLASS IDENTIFIER LBRACE ProgramElements RBRACE  */
#line 233 "jucompiler.y"
                                                         {
            (yyval.node) = create_node("Program", NULL);
            add_child((yyval.node), create_node("Identifier", (yyvsp[-3].str)));
            if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node)); 
            root = (yyval.node); 
        }
#line 1653 "y.tab.c"
    break;

  case 3: /* ProgramElements: %empty  */
#line 240 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1659 "y.tab.c"
    break;

  case 4: /* ProgramElements: ProgramElements MethodDecl  */
#line 241 "jucompiler.y"
                                             { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1665 "y.tab.c"
    break;

  case 5: /* ProgramElements: ProgramElements FieldDecl  */
#line 242 "jucompiler.y"
                                            { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1671 "y.tab.c"
    break;

  case 6: /* ProgramElements: ProgramElements SEMICOLON  */
#line 243 "jucompiler.y"
                                            { (yyval.node) = (yyvsp[-1].node); }
#line 1677 "y.tab.c"
    break;

  case 7: /* MethodDecl: PUBLIC STATIC MethodHeader MethodBody  */
#line 246 "jucompiler.y"
                                                   {
            (yyval.node) = create_node("MethodDecl", NULL);
            add_child((yyval.node), (yyvsp[-1].node));
            add_sibling((yyvsp[-1].node), (yyvsp[0].node));
        }
#line 1687 "y.tab.c"
    break;

  case 8: /* FieldDecl: PUBLIC STATIC Type IDENTIFIER IdList SEMICOLON  */
#line 253 "jucompiler.y"
                                                           {
            (yyval.node) = create_node("FieldDecl", NULL);
            add_child((yyval.node), (yyvsp[-3].node));
            add_child((yyval.node), create_node("Identifier", (yyvsp[-2].str)));
            Node* aux = (yyvsp[-1].node);
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_field = create_node("FieldDecl", NULL);
                add_child(new_field, create_node((yyvsp[-3].node)->type, NULL)); 
                add_child(new_field, aux);
                (yyval.node) = add_sibling((yyval.node), new_field);
                aux = next;
            }
          }
#line 1707 "y.tab.c"
    break;

  case 9: /* FieldDecl: error SEMICOLON  */
#line 268 "jucompiler.y"
                            { (yyval.node) = NULL; }
#line 1713 "y.tab.c"
    break;

  case 10: /* VarDecl: Type IDENTIFIER IdList SEMICOLON  */
#line 271 "jucompiler.y"
                                           {
            (yyval.node) = create_node("VarDecl", NULL);
            add_child((yyval.node), (yyvsp[-3].node));
            add_child((yyval.node), create_node("Identifier", (yyvsp[-2].str)));
            Node* aux = (yyvsp[-1].node);
            while(aux != NULL) {
                Node* next = aux->sibling;
                aux->sibling = NULL;
                Node* new_var = create_node("VarDecl", NULL);
                add_child(new_var, create_node((yyvsp[-3].node)->type, NULL));
                add_child(new_var, aux);
                (yyval.node) = add_sibling((yyval.node), new_var);
                aux = next;
            }
        }
#line 1733 "y.tab.c"
    break;

  case 11: /* IdList: %empty  */
#line 288 "jucompiler.y"
        { (yyval.node) = NULL; }
#line 1739 "y.tab.c"
    break;

  case 12: /* IdList: IdList COMMA IDENTIFIER  */
#line 289 "jucompiler.y"
                                 {   (yyval.node) = add_sibling((yyvsp[-2].node), create_node("Identifier", (yyvsp[0].str))); }
#line 1745 "y.tab.c"
    break;

  case 13: /* Type: BOOL  */
#line 292 "jucompiler.y"
              { (yyval.node) = create_node("Bool", NULL); }
#line 1751 "y.tab.c"
    break;

  case 14: /* Type: INT  */
#line 293 "jucompiler.y"
              { (yyval.node) = create_node("Int", NULL); }
#line 1757 "y.tab.c"
    break;

  case 15: /* Type: DOUBLE  */
#line 294 "jucompiler.y"
              { (yyval.node) = create_node("Double", NULL); }
#line 1763 "y.tab.c"
    break;

  case 16: /* MethodHeader: Type IDENTIFIER LPAR OptFormalParams RPAR  */
#line 297 "jucompiler.y"
                                                         {
                (yyval.node) = create_node("MethodHeader", NULL);
                add_child((yyval.node), (yyvsp[-4].node));
                add_sibling((yyvsp[-4].node), create_node("Identifier", (yyvsp[-3].str)));
                
                Node* params = create_node("MethodParams", NULL);
                if ((yyvsp[-1].node)) add_child(params, (yyvsp[-1].node));
                add_sibling((yyvsp[-4].node), params);
             }
#line 1777 "y.tab.c"
    break;

  case 17: /* MethodHeader: VOID IDENTIFIER LPAR OptFormalParams RPAR  */
#line 306 "jucompiler.y"
                                                         {
                (yyval.node) = create_node("MethodHeader", NULL);
                Node* v = create_node("Void", NULL);
                add_child((yyval.node), v);
                add_sibling(v, create_node("Identifier", (yyvsp[-3].str)));
                
                Node* params = create_node("MethodParams", NULL);
                if ((yyvsp[-1].node)) add_child(params, (yyvsp[-1].node));
                add_sibling(v, params);
             }
#line 1792 "y.tab.c"
    break;

  case 18: /* OptFormalParams: %empty  */
#line 318 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1798 "y.tab.c"
    break;

  case 19: /* OptFormalParams: FormalParams  */
#line 319 "jucompiler.y"
                               { (yyval.node) = (yyvsp[0].node); }
#line 1804 "y.tab.c"
    break;

  case 20: /* FormalParams: Type IDENTIFIER FormalParamsRest  */
#line 322 "jucompiler.y"
                                                {
                (yyval.node) = create_node("ParamDecl", NULL);
                add_child((yyval.node), (yyvsp[-2].node));
                add_sibling((yyvsp[-2].node), create_node("Identifier", (yyvsp[-1].str)));
                if ((yyvsp[0].node)) add_sibling((yyval.node), (yyvsp[0].node));
             }
#line 1815 "y.tab.c"
    break;

  case 21: /* FormalParams: STRING LSQ RSQ IDENTIFIER  */
#line 328 "jucompiler.y"
                                         {
                (yyval.node) = create_node("ParamDecl", NULL);
                Node* strArray = create_node("StringArray", NULL);
                add_child((yyval.node), strArray);
                add_sibling(strArray, create_node("Identifier", (yyvsp[0].str)));
             }
#line 1826 "y.tab.c"
    break;

  case 22: /* FormalParamsRest: %empty  */
#line 336 "jucompiler.y"
                   { (yyval.node) = NULL; }
#line 1832 "y.tab.c"
    break;

  case 23: /* FormalParamsRest: FormalParamsRest COMMA Type IDENTIFIER  */
#line 337 "jucompiler.y"
                                                          {
                    Node* param = create_node("ParamDecl", NULL);
                    add_child(param, (yyvsp[-1].node));
                    add_sibling((yyvsp[-1].node), create_node("Identifier", (yyvsp[0].str)));
                    (yyval.node) = add_sibling((yyvsp[-3].node), param);
                 }
#line 1843 "y.tab.c"
    break;

  case 24: /* MethodBody: LBRACE StatementAndVarDeclList RBRACE  */
#line 345 "jucompiler.y"
                                                   {
                (yyval.node) = create_node("MethodBody", NULL);
                if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
           }
#line 1852 "y.tab.c"
    break;

  case 25: /* StatementAndVarDeclList: %empty  */
#line 351 "jucompiler.y"
                          { (yyval.node) = NULL; }
#line 1858 "y.tab.c"
    break;

  case 26: /* StatementAndVarDeclList: StatementAndVarDeclList Statement  */
#line 352 "jucompiler.y"
                                                            {
                            if ((yyvsp[0].node) != NULL) { 
                                (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node));
                            } else {
                                (yyval.node) = (yyvsp[-1].node);
                            }
                        }
#line 1870 "y.tab.c"
    break;

  case 27: /* StatementAndVarDeclList: StatementAndVarDeclList VarDecl  */
#line 359 "jucompiler.y"
                                                          { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1876 "y.tab.c"
    break;

  case 28: /* StatementList: %empty  */
#line 363 "jucompiler.y"
                { (yyval.node) = NULL; }
#line 1882 "y.tab.c"
    break;

  case 29: /* StatementList: StatementList Statement  */
#line 364 "jucompiler.y"
                                        {
                  if ((yyvsp[0].node) != NULL) {
                      (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node));
                  } else {
                      (yyval.node) = (yyvsp[-1].node);
                  }
              }
#line 1894 "y.tab.c"
    break;

  case 30: /* Statement: LBRACE StatementList RBRACE  */
#line 373 "jucompiler.y"
                                        {
              if ((yyvsp[-1].node) != NULL && (yyvsp[-1].node)->sibling != NULL) {
                  (yyval.node) = create_node("Block", NULL);
                  add_child((yyval.node), (yyvsp[-1].node));
              } else if ((yyvsp[-1].node) != NULL) {
                  (yyval.node) = (yyvsp[-1].node); 
              } else {
                  (yyval.node) = NULL; 
              }
          }
#line 1909 "y.tab.c"
    break;

  case 31: /* Statement: IF LPAR AExpr RPAR Statement  */
#line 383 "jucompiler.y"
                                                               {
              (yyval.node) = create_node("If", NULL);
              add_child((yyval.node), (yyvsp[-2].node)); 
              add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL)); 
              add_child((yyval.node), create_node("Block", NULL)); 
          }
#line 1920 "y.tab.c"
    break;

  case 32: /* Statement: IF LPAR AExpr RPAR Statement ELSE Statement  */
#line 389 "jucompiler.y"
                                                        {
              (yyval.node) = create_node("If", NULL);
              add_child((yyval.node), (yyvsp[-4].node));
              add_child((yyval.node), (yyvsp[-2].node) ? (yyvsp[-2].node) : create_node("Block", NULL));
              add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
          }
#line 1931 "y.tab.c"
    break;

  case 33: /* Statement: WHILE LPAR AExpr RPAR Statement  */
#line 395 "jucompiler.y"
                                            {
              (yyval.node) = create_node("While", NULL);
              add_child((yyval.node), (yyvsp[-2].node));
              add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
          }
#line 1941 "y.tab.c"
    break;

  case 34: /* Statement: RETURN OptExpr SEMICOLON  */
#line 400 "jucompiler.y"
                                     {
              (yyval.node) = create_node("Return", NULL);
              if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
          }
#line 1950 "y.tab.c"
    break;

  case 35: /* Statement: MethodInvocation SEMICOLON  */
#line 404 "jucompiler.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 1956 "y.tab.c"
    break;

  case 36: /* Statement: Assignment SEMICOLON  */
#line 405 "jucompiler.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1962 "y.tab.c"
    break;

  case 37: /* Statement: ParseArgs SEMICOLON  */
#line 406 "jucompiler.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 1968 "y.tab.c"
    break;

  case 38: /* Statement: PRINT LPAR ExprOrStrlit RPAR SEMICOLON  */
#line 407 "jucompiler.y"
                                                   {
              (yyval.node) = create_node("Print", NULL);
              add_child((yyval.node), (yyvsp[-2].node));
          }
#line 1977 "y.tab.c"
    break;

  case 39: /* Statement: error SEMICOLON  */
#line 411 "jucompiler.y"
                            { (yyval.node) = NULL; }
#line 1983 "y.tab.c"
    break;

  case 40: /* Statement: SEMICOLON  */
#line 412 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 1989 "y.tab.c"
    break;

  case 41: /* OptExpr: %empty  */
#line 415 "jucompiler.y"
          { (yyval.node) = NULL; }
#line 1995 "y.tab.c"
    break;

  case 42: /* OptExpr: AExpr  */
#line 416 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2001 "y.tab.c"
    break;

  case 43: /* ExprOrStrlit: AExpr  */
#line 419 "jucompiler.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2007 "y.tab.c"
    break;

  case 44: /* ExprOrStrlit: STRLIT  */
#line 420 "jucompiler.y"
                      { (yyval.node) = create_node("StrLit", (yyvsp[0].str)); }
#line 2013 "y.tab.c"
    break;

  case 45: /* MethodInvocation: IDENTIFIER LPAR OptExprList RPAR  */
#line 423 "jucompiler.y"
                                                    {
                     (yyval.node) = create_node("Call", NULL);
                     Node* id = create_node("Identifier", (yyvsp[-3].str));
                     add_child((yyval.node), id);
                     if ((yyvsp[-1].node)) add_sibling(id, (yyvsp[-1].node));
                 }
#line 2024 "y.tab.c"
    break;

  case 46: /* MethodInvocation: IDENTIFIER LPAR error RPAR  */
#line 429 "jucompiler.y"
                                              { (yyval.node) = NULL; }
#line 2030 "y.tab.c"
    break;

  case 47: /* OptExprList: %empty  */
#line 432 "jucompiler.y"
              { (yyval.node) = NULL; }
#line 2036 "y.tab.c"
    break;

  case 48: /* OptExprList: ExprList  */
#line 433 "jucompiler.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2042 "y.tab.c"
    break;

  case 49: /* ExprList: AExpr  */
#line 436 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2048 "y.tab.c"
    break;

  case 50: /* ExprList: ExprList COMMA AExpr  */
#line 437 "jucompiler.y"
                                { (yyval.node) = add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2054 "y.tab.c"
    break;

  case 51: /* Assignment: IDENTIFIER ASSIGN AExpr  */
#line 440 "jucompiler.y"
                                     {
               (yyval.node) = create_node("Assign", NULL);
               Node* id = create_node("Identifier", (yyvsp[-2].str));
               add_child((yyval.node), id);
               add_sibling(id, (yyvsp[0].node));
           }
#line 2065 "y.tab.c"
    break;

  case 52: /* AExpr: IDENTIFIER ASSIGN AExpr  */
#line 448 "jucompiler.y"
                                {
            (yyval.node) = create_node("Assign", NULL);
            Node* id = create_node("Identifier", (yyvsp[-2].str));
            add_child((yyval.node), id);
            add_sibling(id, (yyvsp[0].node));
        }
#line 2076 "y.tab.c"
    break;

  case 53: /* AExpr: Expr  */
#line 454 "jucompiler.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2082 "y.tab.c"
    break;

  case 54: /* ParseArgs: PARSEINT LPAR IDENTIFIER LSQ AExpr RSQ RPAR  */
#line 457 "jucompiler.y"
                                                        {
              (yyval.node) = create_node("ParseArgs", NULL);
              Node* id = create_node("Identifier", (yyvsp[-4].str));
              add_child((yyval.node), id);
              add_sibling(id, (yyvsp[-2].node));
          }
#line 2093 "y.tab.c"
    break;

  case 55: /* ParseArgs: PARSEINT LPAR error RPAR  */
#line 463 "jucompiler.y"
                                     { (yyval.node) = NULL; }
#line 2099 "y.tab.c"
    break;

  case 56: /* Expr: Expr PLUS Expr  */
#line 466 "jucompiler.y"
                        { (yyval.node) = create_node("Add", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2105 "y.tab.c"
    break;

  case 57: /* Expr: Expr MINUS Expr  */
#line 467 "jucompiler.y"
                        { (yyval.node) = create_node("Sub", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2111 "y.tab.c"
    break;

  case 58: /* Expr: Expr STAR Expr  */
#line 468 "jucompiler.y"
                        { (yyval.node) = create_node("Mul", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2117 "y.tab.c"
    break;

  case 59: /* Expr: Expr DIV Expr  */
#line 469 "jucompiler.y"
                        { (yyval.node) = create_node("Div", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2123 "y.tab.c"
    break;

  case 60: /* Expr: Expr MOD Expr  */
#line 470 "jucompiler.y"
                        { (yyval.node) = create_node("Mod", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2129 "y.tab.c"
    break;

  case 61: /* Expr: Expr AND Expr  */
#line 471 "jucompiler.y"
                        { (yyval.node) = create_node("And", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2135 "y.tab.c"
    break;

  case 62: /* Expr: Expr OR Expr  */
#line 472 "jucompiler.y"
                        { (yyval.node) = create_node("Or", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2141 "y.tab.c"
    break;

  case 63: /* Expr: Expr XOR Expr  */
#line 473 "jucompiler.y"
                        { (yyval.node) = create_node("Xor", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2147 "y.tab.c"
    break;

  case 64: /* Expr: Expr LSHIFT Expr  */
#line 474 "jucompiler.y"
                        { (yyval.node) = create_node("Lshift", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2153 "y.tab.c"
    break;

  case 65: /* Expr: Expr RSHIFT Expr  */
#line 475 "jucompiler.y"
                        { (yyval.node) = create_node("Rshift", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2159 "y.tab.c"
    break;

  case 66: /* Expr: Expr EQ Expr  */
#line 476 "jucompiler.y"
                        { (yyval.node) = create_node("Eq", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2165 "y.tab.c"
    break;

  case 67: /* Expr: Expr GE Expr  */
#line 477 "jucompiler.y"
                        { (yyval.node) = create_node("Ge", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2171 "y.tab.c"
    break;

  case 68: /* Expr: Expr GT Expr  */
#line 478 "jucompiler.y"
                        { (yyval.node) = create_node("Gt", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2177 "y.tab.c"
    break;

  case 69: /* Expr: Expr LE Expr  */
#line 479 "jucompiler.y"
                        { (yyval.node) = create_node("Le", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2183 "y.tab.c"
    break;

  case 70: /* Expr: Expr LT Expr  */
#line 480 "jucompiler.y"
                        { (yyval.node) = create_node("Lt", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2189 "y.tab.c"
    break;

  case 71: /* Expr: Expr NE Expr  */
#line 481 "jucompiler.y"
                        { (yyval.node) = create_node("Ne", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2195 "y.tab.c"
    break;

  case 72: /* Expr: MINUS Expr  */
#line 482 "jucompiler.y"
                                    { (yyval.node) = create_node("Minus", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2201 "y.tab.c"
    break;

  case 73: /* Expr: PLUS Expr  */
#line 483 "jucompiler.y"
                                    { (yyval.node) = create_node("Plus", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2207 "y.tab.c"
    break;

  case 74: /* Expr: NOT Expr  */
#line 484 "jucompiler.y"
                        { (yyval.node) = create_node("Not", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2213 "y.tab.c"
    break;

  case 75: /* Expr: LPAR AExpr RPAR  */
#line 485 "jucompiler.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 2219 "y.tab.c"
    break;

  case 76: /* Expr: LPAR error RPAR  */
#line 486 "jucompiler.y"
                        { (yyval.node) = NULL; }
#line 2225 "y.tab.c"
    break;

  case 77: /* Expr: MethodInvocation  */
#line 487 "jucompiler.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2231 "y.tab.c"
    break;

  case 78: /* Expr: ParseArgs  */
#line 488 "jucompiler.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2237 "y.tab.c"
    break;

  case 79: /* Expr: IDENTIFIER OptDotLength  */
#line 489 "jucompiler.y"
                               {
         if ((yyvsp[0].node)) {
             (yyval.node) = create_node("Length", NULL);
             add_child((yyval.node), create_node("Identifier", (yyvsp[-1].str)));
         } else {
             (yyval.node) = create_node("Identifier", (yyvsp[-1].str));
         }
     }
#line 2250 "y.tab.c"
    break;

  case 80: /* Expr: NATURAL  */
#line 497 "jucompiler.y"
                        { (yyval.node) = create_node("Natural", (yyvsp[0].str)); }
#line 2256 "y.tab.c"
    break;

  case 81: /* Expr: DECIMAL  */
#line 498 "jucompiler.y"
                        { (yyval.node) = create_node("Decimal", (yyvsp[0].str)); }
#line 2262 "y.tab.c"
    break;

  case 82: /* Expr: BOOLLIT  */
#line 499 "jucompiler.y"
                        { (yyval.node) = create_node("BoolLit", (yyvsp[0].str)); }
#line 2268 "y.tab.c"
    break;

  case 83: /* OptDotLength: %empty  */
#line 502 "jucompiler.y"
               { (yyval.node) = NULL; }
#line 2274 "y.tab.c"
    break;

  case 84: /* OptDotLength: DOTLENGTH  */
#line 503 "jucompiler.y"
                         { (yyval.node) = create_node("DotLength", NULL); }
#line 2280 "y.tab.c"
    break;


#line 2284 "y.tab.c"

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

#line 506 "jucompiler.y"

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
