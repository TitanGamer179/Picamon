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

int flag_lex = 0;
int flag_syntax = 0;
int flag_errors = 0;
int syntax_errors_count = 0;

extern int line, col;
extern char* yytext;
extern int yyleng;

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

void add_child(Node *parent, Node *child) {
    if (!parent || !child) return;
    if (!parent->child) {
        parent->child = child;
    } else {
        Node *sibling = parent->child;
        while (sibling->sibling) sibling = sibling->sibling;
        sibling->sibling = child;
    }
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


#line 130 "y.tab.c"

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
    IF_WITHOUT_ELSE = 305,         /* IF_WITHOUT_ELSE  */
    UNARY_PLUS = 306,              /* UNARY_PLUS  */
    UNARY_MINUS = 307              /* UNARY_MINUS  */
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
#define IF_WITHOUT_ELSE 305
#define UNARY_PLUS 306
#define UNARY_MINUS 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 62 "jucompiler.y"

    char* str;
    struct _node* node;

#line 292 "y.tab.c"

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
  YYSYMBOL_IF_WITHOUT_ELSE = 50,           /* IF_WITHOUT_ELSE  */
  YYSYMBOL_UNARY_PLUS = 51,                /* UNARY_PLUS  */
  YYSYMBOL_UNARY_MINUS = 52,               /* UNARY_MINUS  */
  YYSYMBOL_YYACCEPT = 53,                  /* $accept  */
  YYSYMBOL_Program = 54,                   /* Program  */
  YYSYMBOL_ProgramElements = 55,           /* ProgramElements  */
  YYSYMBOL_MethodDecl = 56,                /* MethodDecl  */
  YYSYMBOL_FieldDecl = 57,                 /* FieldDecl  */
  YYSYMBOL_Type = 58,                      /* Type  */
  YYSYMBOL_MethodHeader = 59,              /* MethodHeader  */
  YYSYMBOL_OptFormalParams = 60,           /* OptFormalParams  */
  YYSYMBOL_FormalParams = 61,              /* FormalParams  */
  YYSYMBOL_FormalParamsRest = 62,          /* FormalParamsRest  */
  YYSYMBOL_MethodBody = 63,                /* MethodBody  */
  YYSYMBOL_StatementAndVarDeclList = 64,   /* StatementAndVarDeclList  */
  YYSYMBOL_VarDecl = 65,                   /* VarDecl  */
  YYSYMBOL_IdList = 66,                    /* IdList  */
  YYSYMBOL_StatementList = 67,             /* StatementList  */
  YYSYMBOL_Statement = 68,                 /* Statement  */
  YYSYMBOL_OptExpr = 69,                   /* OptExpr  */
  YYSYMBOL_ExprOrStrlit = 70,              /* ExprOrStrlit  */
  YYSYMBOL_MethodInvocation = 71,          /* MethodInvocation  */
  YYSYMBOL_OptExprList = 72,               /* OptExprList  */
  YYSYMBOL_ExprList = 73,                  /* ExprList  */
  YYSYMBOL_Assignment = 74,                /* Assignment  */
  YYSYMBOL_ParseArgs = 75,                 /* ParseArgs  */
  YYSYMBOL_Expr = 76,                      /* Expr  */
  YYSYMBOL_OptDotLength = 77               /* OptDotLength  */
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
#define YYLAST   471

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   307


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
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    93,    93,   102,   103,   104,   105,   108,   115,   120,
     123,   124,   125,   128,   137,   149,   150,   153,   159,   167,
     168,   176,   182,   183,   190,   193,   200,   201,   204,   205,
     214,   224,   230,   236,   241,   245,   246,   247,   248,   252,
     253,   256,   257,   260,   261,   264,   270,   273,   274,   277,
     278,   281,   289,   295,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   330,
     331,   332,   335,   336
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
  "DECIMAL", "STRLIT", "BOOLLIT", "IF_WITHOUT_ELSE", "UNARY_PLUS",
  "UNARY_MINUS", "$accept", "Program", "ProgramElements", "MethodDecl",
  "FieldDecl", "Type", "MethodHeader", "OptFormalParams", "FormalParams",
  "FormalParamsRest", "MethodBody", "StatementAndVarDeclList", "VarDecl",
  "IdList", "StatementList", "Statement", "OptExpr", "ExprOrStrlit",
  "MethodInvocation", "OptExprList", "ExprList", "Assignment", "ParseArgs",
  "Expr", "OptDotLength", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-48)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       0,   -37,    20,     9,   -57,   -57,     6,    -4,    25,   -57,
     -57,   -57,   -57,   -57,    24,   -57,   -57,   -57,    -3,    -1,
      12,    13,    17,   -57,   -57,   132,   132,   -16,   153,    38,
      23,    32,   -57,    34,    40,   -57,    37,    58,    64,    73,
     201,    76,   -57,   -57,   -57,    -7,    60,   -57,   -57,    56,
      70,    71,    69,   -57,   -57,   -57,   -57,   -57,   201,   175,
      21,   106,   201,   201,   201,     4,   -57,   -57,   -57,    75,
     -57,   -57,   -57,   339,   201,   170,   201,    78,   -57,   -57,
     -57,   -57,    77,    97,   231,   -57,    88,   339,    90,   100,
      92,   258,   -57,   -57,   -57,   -57,   -57,   -57,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   285,   -57,   -57,   339,    98,   101,
     122,   339,   -15,   -57,    28,   191,   104,   -57,   201,   -57,
     -57,    51,   -57,   -57,   415,   428,   428,   428,   428,    68,
     -57,   415,   366,    68,   194,   194,   391,   191,   -57,   -57,
     201,   -57,   103,   138,   -57,   312,   -57,   339,   -57,   191,
     108,   -57,   -57
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     3,     0,     0,     0,     2,
       6,     4,     5,     9,     0,    10,    12,    11,     0,     0,
       0,     0,    26,    22,     7,    15,    15,     0,     0,     0,
       0,     0,    16,     0,     0,     8,     0,     0,     0,     0,
      41,     0,    28,    21,    40,     0,     0,    24,    23,     0,
       0,     0,     0,    19,    14,    13,    27,    39,     0,     0,
       0,     0,     0,     0,     0,    82,    79,    80,    81,     0,
      75,    76,    77,    42,     0,     0,     0,     0,    26,    35,
      36,    37,     0,    17,     0,    44,     0,    43,     0,     0,
       0,     0,    70,    72,    71,    83,    78,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    29,    51,     0,     0,
      48,    49,     0,    18,     0,     0,     0,    53,     0,    74,
      73,    59,    56,    57,    64,    65,    66,    67,    68,    55,
      58,    69,    60,    54,    62,    63,    61,     0,    46,    45,
       0,    25,     0,    31,    38,     0,    33,    50,    20,     0,
       0,    32,    52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   -57,   -57,   -57,   -57,   -11,   -57,   123,   -57,   -57,
     -57,   -57,   -57,    72,   -57,   -56,   -57,   -57,   -28,   -57,
     -57,   -27,   -26,   -48,   -57
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     6,    11,    12,    30,    20,    31,    32,    83,
      24,    28,    47,    27,    75,    48,    69,    86,    70,   119,
     120,    71,    72,    73,    96
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    50,    51,    19,     1,    34,    34,     7,     3,    95,
      84,    87,    76,    91,    92,    93,    94,    46,     8,   116,
       4,    77,    88,    76,    35,   151,   114,    15,   117,   121,
      16,    15,    77,    17,    16,     5,    13,    17,    23,    14,
      18,    25,    21,     9,    22,    26,    10,    49,    50,    51,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    89,    52,    53,   153,
      54,    99,    55,   100,   101,   102,   103,    57,   104,   118,
     155,   105,   106,   107,   108,    56,    58,   110,    99,    39,
     100,   156,    59,   111,   112,   113,    79,    49,    50,    51,
     107,    60,   157,   161,    74,    78,    61,    90,    82,    62,
      80,    81,    63,   152,    64,    97,   -47,    39,   124,    49,
      50,    51,   123,    65,    66,    67,   126,    68,   127,   128,
     129,    49,    50,    51,    61,    15,   148,    62,    16,   149,
      63,    17,    64,   150,   154,   159,   162,    29,   158,    33,
     122,    65,    66,    67,    36,    68,    15,     0,     0,    16,
       0,    37,    17,    38,    39,     0,    40,     0,     0,     0,
      41,    36,     0,     0,     0,     0,     0,     0,    37,    42,
      38,    39,     0,    40,     0,     0,    39,    41,     0,     0,
      43,     0,    36,    44,     0,     0,    42,     0,    45,    37,
       0,    38,    39,    61,    40,     0,    62,   115,    41,    63,
      44,    64,    39,     0,    99,    45,   100,    42,     0,     0,
      65,    66,    67,    85,    68,   106,   107,     0,     0,    61,
     110,    44,    62,     0,     0,    63,    45,    64,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    98,
      68,    99,     0,   100,   101,   102,   103,     0,   104,     0,
       0,   105,   106,   107,   108,     0,   109,   110,     0,   125,
       0,     0,     0,   111,   112,   113,    98,     0,    99,     0,
     100,   101,   102,   103,     0,   104,     0,     0,   105,   106,
     107,   108,     0,   109,   110,     0,   130,     0,     0,     0,
     111,   112,   113,    98,     0,    99,     0,   100,   101,   102,
     103,     0,   104,     0,     0,   105,   106,   107,   108,     0,
     109,   110,     0,   147,     0,     0,     0,   111,   112,   113,
      98,     0,    99,     0,   100,   101,   102,   103,     0,   104,
       0,     0,   105,   106,   107,   108,     0,   109,   110,     0,
       0,   160,     0,     0,   111,   112,   113,    98,     0,    99,
       0,   100,   101,   102,   103,     0,   104,     0,     0,   105,
     106,   107,   108,     0,   109,   110,     0,     0,     0,     0,
       0,   111,   112,   113,    98,     0,    99,     0,   100,   101,
     102,   103,     0,   104,     0,     0,   105,   106,   107,   108,
       0,     0,   110,     0,     0,     0,     0,     0,   111,   112,
     113,    99,     0,   100,   101,   102,   103,     0,   104,     0,
       0,   105,   106,   107,   108,     0,     0,   110,     0,     0,
       0,     0,     0,   111,   112,    99,     0,   100,     0,   102,
     103,     0,   104,     0,     0,   105,   106,   107,    99,     0,
     100,   110,     0,     0,     0,     0,     0,   111,   112,   106,
     107,     0,     0,     0,   110,     0,     0,     0,     0,     0,
     111,   112
};

static const yytype_int16 yycheck[] =
{
      28,    28,    28,    14,     4,    21,    21,     1,    45,     5,
      58,    59,    19,    61,    62,    63,    64,    28,    12,    75,
       0,    28,     1,    19,    40,    40,    74,     3,    76,    77,
       6,     3,    28,     9,     6,    26,    40,     9,    26,    14,
      16,    28,    45,    37,    45,    28,    40,    75,    75,    75,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    45,    29,    45,   125,
      38,    20,    38,    22,    23,    24,    25,    40,    27,     1,
     128,    30,    31,    32,    33,    45,    28,    36,    20,    11,
      22,   147,    28,    42,    43,    44,    40,   125,   125,   125,
      32,    28,   150,   159,    28,    45,    28,     1,    39,    31,
      40,    40,    34,   124,    36,    40,    38,    11,    21,   147,
     147,   147,    45,    45,    46,    47,    38,    49,    38,    29,
      38,   159,   159,   159,    28,     3,    38,    31,     6,    38,
      34,     9,    36,    21,    40,     7,    38,    15,    45,    26,
      78,    45,    46,    47,     1,    49,     3,    -1,    -1,     6,
      -1,     8,     9,    10,    11,    -1,    13,    -1,    -1,    -1,
      17,     1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    26,
      10,    11,    -1,    13,    -1,    -1,    11,    17,    -1,    -1,
      37,    -1,     1,    40,    -1,    -1,    26,    -1,    45,     8,
      -1,    10,    11,    28,    13,    -1,    31,    37,    17,    34,
      40,    36,    11,    -1,    20,    45,    22,    26,    -1,    -1,
      45,    46,    47,    48,    49,    31,    32,    -1,    -1,    28,
      36,    40,    31,    -1,    -1,    34,    45,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    18,
      49,    20,    -1,    22,    23,    24,    25,    -1,    27,    -1,
      -1,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      -1,    -1,    -1,    42,    43,    44,    18,    -1,    20,    -1,
      22,    23,    24,    25,    -1,    27,    -1,    -1,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    -1,    -1,    -1,
      42,    43,    44,    18,    -1,    20,    -1,    22,    23,    24,
      25,    -1,    27,    -1,    -1,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    -1,    -1,    -1,    42,    43,    44,
      18,    -1,    20,    -1,    22,    23,    24,    25,    -1,    27,
      -1,    -1,    30,    31,    32,    33,    -1,    35,    36,    -1,
      -1,    39,    -1,    -1,    42,    43,    44,    18,    -1,    20,
      -1,    22,    23,    24,    25,    -1,    27,    -1,    -1,    30,
      31,    32,    33,    -1,    35,    36,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    18,    -1,    20,    -1,    22,    23,
      24,    25,    -1,    27,    -1,    -1,    30,    31,    32,    33,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    20,    -1,    22,    23,    24,    25,    -1,    27,    -1,
      -1,    30,    31,    32,    33,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    42,    43,    20,    -1,    22,    -1,    24,
      25,    -1,    27,    -1,    -1,    30,    31,    32,    20,    -1,
      22,    36,    -1,    -1,    -1,    -1,    -1,    42,    43,    31,
      32,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      42,    43
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    54,    45,     0,    26,    55,     1,    12,    37,
      40,    56,    57,    40,    14,     3,     6,     9,    16,    58,
      59,    45,    45,    26,    63,    28,    28,    66,    64,    15,
      58,    60,    61,    60,    21,    40,     1,     8,    10,    11,
      13,    17,    26,    37,    40,    45,    58,    65,    68,    71,
      74,    75,    29,    45,    38,    38,    45,    40,    28,    28,
      28,    28,    31,    34,    36,    45,    46,    47,    49,    69,
      71,    74,    75,    76,    28,    67,    19,    28,    45,    40,
      40,    40,    39,    62,    76,    48,    70,    76,     1,    45,
       1,    76,    76,    76,    76,     5,    77,    40,    18,    20,
      22,    23,    24,    25,    27,    30,    31,    32,    33,    35,
      36,    42,    43,    44,    76,    37,    68,    76,     1,    72,
      73,    76,    66,    45,    21,    38,    38,    38,    29,    38,
      38,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    38,    38,    38,
      21,    40,    58,    68,    40,    76,    68,    76,    45,     7,
      39,    68,    38
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    55,    55,    55,    55,    56,    57,    57,
      58,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    64,    64,    64,    65,    66,    66,    67,    67,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    69,    69,    70,    70,    71,    71,    72,    72,    73,
      73,    74,    75,    75,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    77,    77
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     0,     2,     2,     2,     4,     6,     2,
       1,     1,     1,     5,     5,     0,     1,     3,     4,     0,
       4,     3,     0,     2,     2,     4,     0,     3,     0,     2,
       3,     5,     7,     5,     3,     2,     2,     2,     5,     2,
       1,     0,     1,     1,     1,     4,     4,     0,     1,     1,
       3,     3,     7,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     3,     3,     1,     1,     1,     2,     1,
       1,     1,     0,     1
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
#line 93 "jucompiler.y"
                                                         {
            (yyval.node) = create_node("Program", NULL);
            Node* id = create_node("Identifier", (yyvsp[-3].str));
            add_child((yyval.node), id);
            if ((yyvsp[-1].node)) add_sibling(id, (yyvsp[-1].node));
            root = (yyval.node);
        }
#line 1526 "y.tab.c"
    break;

  case 3: /* ProgramElements: %empty  */
#line 102 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1532 "y.tab.c"
    break;

  case 4: /* ProgramElements: ProgramElements MethodDecl  */
#line 103 "jucompiler.y"
                                             { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1538 "y.tab.c"
    break;

  case 5: /* ProgramElements: ProgramElements FieldDecl  */
#line 104 "jucompiler.y"
                                            { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1544 "y.tab.c"
    break;

  case 6: /* ProgramElements: ProgramElements SEMICOLON  */
#line 105 "jucompiler.y"
                                            { (yyval.node) = (yyvsp[-1].node); }
#line 1550 "y.tab.c"
    break;

  case 7: /* MethodDecl: PUBLIC STATIC MethodHeader MethodBody  */
#line 108 "jucompiler.y"
                                                   {
            (yyval.node) = create_node("MethodDecl", NULL);
            add_child((yyval.node), (yyvsp[-1].node));
            add_sibling((yyvsp[-1].node), (yyvsp[0].node));
        }
#line 1560 "y.tab.c"
    break;

  case 8: /* FieldDecl: PUBLIC STATIC Type IDENTIFIER IdList SEMICOLON  */
#line 115 "jucompiler.y"
                                                           {
            (yyval.node) = create_node("FieldDecl", NULL);
            add_child((yyval.node), (yyvsp[-3].node));
            add_sibling((yyvsp[-3].node), create_node("Identifier", (yyvsp[-2].str)));
          }
#line 1570 "y.tab.c"
    break;

  case 9: /* FieldDecl: error SEMICOLON  */
#line 120 "jucompiler.y"
                            { (yyval.node) = NULL; }
#line 1576 "y.tab.c"
    break;

  case 10: /* Type: BOOL  */
#line 123 "jucompiler.y"
              { (yyval.node) = create_node("Bool", NULL); }
#line 1582 "y.tab.c"
    break;

  case 11: /* Type: INT  */
#line 124 "jucompiler.y"
              { (yyval.node) = create_node("Int", NULL); }
#line 1588 "y.tab.c"
    break;

  case 12: /* Type: DOUBLE  */
#line 125 "jucompiler.y"
              { (yyval.node) = create_node("Double", NULL); }
#line 1594 "y.tab.c"
    break;

  case 13: /* MethodHeader: Type IDENTIFIER LPAR OptFormalParams RPAR  */
#line 128 "jucompiler.y"
                                                         {
                (yyval.node) = create_node("MethodHeader", NULL);
                add_child((yyval.node), (yyvsp[-4].node));
                add_sibling((yyvsp[-4].node), create_node("Identifier", (yyvsp[-3].str)));
                
                Node* params = create_node("MethodParams", NULL);
                if ((yyvsp[-1].node)) add_child(params, (yyvsp[-1].node));
                add_sibling((yyvsp[-4].node), params);
             }
#line 1608 "y.tab.c"
    break;

  case 14: /* MethodHeader: VOID IDENTIFIER LPAR OptFormalParams RPAR  */
#line 137 "jucompiler.y"
                                                         {
                (yyval.node) = create_node("MethodHeader", NULL);
                Node* v = create_node("Void", NULL);
                add_child((yyval.node), v);
                add_sibling(v, create_node("Identifier", (yyvsp[-3].str)));
                
                Node* params = create_node("MethodParams", NULL);
                if ((yyvsp[-1].node)) add_child(params, (yyvsp[-1].node));
                add_sibling(v, params);
             }
#line 1623 "y.tab.c"
    break;

  case 15: /* OptFormalParams: %empty  */
#line 149 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1629 "y.tab.c"
    break;

  case 16: /* OptFormalParams: FormalParams  */
#line 150 "jucompiler.y"
                               { (yyval.node) = (yyvsp[0].node); }
#line 1635 "y.tab.c"
    break;

  case 17: /* FormalParams: Type IDENTIFIER FormalParamsRest  */
#line 153 "jucompiler.y"
                                                {
                (yyval.node) = create_node("ParamDecl", NULL);
                add_child((yyval.node), (yyvsp[-2].node));
                add_sibling((yyvsp[-2].node), create_node("Identifier", (yyvsp[-1].str)));
                if ((yyvsp[0].node)) add_sibling((yyval.node), (yyvsp[0].node));
             }
#line 1646 "y.tab.c"
    break;

  case 18: /* FormalParams: STRING LSQ RSQ IDENTIFIER  */
#line 159 "jucompiler.y"
                                         {
                (yyval.node) = create_node("ParamDecl", NULL);
                Node* strArray = create_node("StringArray", NULL);
                add_child((yyval.node), strArray);
                add_sibling(strArray, create_node("Identifier", (yyvsp[0].str)));
             }
#line 1657 "y.tab.c"
    break;

  case 19: /* FormalParamsRest: %empty  */
#line 167 "jucompiler.y"
                   { (yyval.node) = NULL; }
#line 1663 "y.tab.c"
    break;

  case 20: /* FormalParamsRest: FormalParamsRest COMMA Type IDENTIFIER  */
#line 168 "jucompiler.y"
                                                          {
                    Node* param = create_node("ParamDecl", NULL);
                    add_child(param, (yyvsp[-1].node));
                    add_sibling((yyvsp[-1].node), create_node("Identifier", (yyvsp[0].str)));
                    (yyval.node) = add_sibling((yyvsp[-3].node), param);
                 }
#line 1674 "y.tab.c"
    break;

  case 21: /* MethodBody: LBRACE StatementAndVarDeclList RBRACE  */
#line 176 "jucompiler.y"
                                                   {
                (yyval.node) = create_node("MethodBody", NULL);
                if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
           }
#line 1683 "y.tab.c"
    break;

  case 22: /* StatementAndVarDeclList: %empty  */
#line 182 "jucompiler.y"
                          { (yyval.node) = NULL; }
#line 1689 "y.tab.c"
    break;

  case 23: /* StatementAndVarDeclList: StatementAndVarDeclList Statement  */
#line 183 "jucompiler.y"
                                                            {
                            if ((yyvsp[0].node) != NULL) { 
                                (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node));
                            } else {
                                (yyval.node) = (yyvsp[-1].node);
                            }
                        }
#line 1701 "y.tab.c"
    break;

  case 24: /* StatementAndVarDeclList: StatementAndVarDeclList VarDecl  */
#line 190 "jucompiler.y"
                                                          { (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1707 "y.tab.c"
    break;

  case 25: /* VarDecl: Type IDENTIFIER IdList SEMICOLON  */
#line 193 "jucompiler.y"
                                           {
            (yyval.node) = create_node("VarDecl", NULL);
            add_child((yyval.node), (yyvsp[-3].node));
            add_sibling((yyvsp[-3].node), create_node("Identifier", (yyvsp[-2].str)));
        }
#line 1717 "y.tab.c"
    break;

  case 26: /* IdList: %empty  */
#line 200 "jucompiler.y"
         { (yyval.node) = NULL; }
#line 1723 "y.tab.c"
    break;

  case 27: /* IdList: IdList COMMA IDENTIFIER  */
#line 201 "jucompiler.y"
                                 {  (yyval.node) = NULL; }
#line 1729 "y.tab.c"
    break;

  case 28: /* StatementList: %empty  */
#line 204 "jucompiler.y"
                            { (yyval.node) = NULL; }
#line 1735 "y.tab.c"
    break;

  case 29: /* StatementList: StatementList Statement  */
#line 205 "jucompiler.y"
                                        {
                  if ((yyvsp[0].node) != NULL) {
                      (yyval.node) = add_sibling((yyvsp[-1].node), (yyvsp[0].node));
                  } else {
                      (yyval.node) = (yyvsp[-1].node);
                  }
              }
#line 1747 "y.tab.c"
    break;

  case 30: /* Statement: LBRACE StatementList RBRACE  */
#line 214 "jucompiler.y"
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
#line 1762 "y.tab.c"
    break;

  case 31: /* Statement: IF LPAR Expr RPAR Statement  */
#line 224 "jucompiler.y"
                                                              {
              (yyval.node) = create_node("If", NULL);
              add_child((yyval.node), (yyvsp[-2].node));
              add_sibling((yyvsp[-2].node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
              add_sibling((yyvsp[-2].node)->sibling, create_node("Block", NULL));
          }
#line 1773 "y.tab.c"
    break;

  case 32: /* Statement: IF LPAR Expr RPAR Statement ELSE Statement  */
#line 230 "jucompiler.y"
                                                       {
              (yyval.node) = create_node("If", NULL);
              add_child((yyval.node), (yyvsp[-4].node));
              add_sibling((yyvsp[-4].node), (yyvsp[-2].node) ? (yyvsp[-2].node) : create_node("Block", NULL));
              add_sibling((yyvsp[-4].node)->sibling, (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
          }
#line 1784 "y.tab.c"
    break;

  case 33: /* Statement: WHILE LPAR Expr RPAR Statement  */
#line 236 "jucompiler.y"
                                           {
              (yyval.node) = create_node("While", NULL);
              add_child((yyval.node), (yyvsp[-2].node));
              add_sibling((yyvsp[-2].node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
          }
#line 1794 "y.tab.c"
    break;

  case 34: /* Statement: RETURN OptExpr SEMICOLON  */
#line 241 "jucompiler.y"
                                     {
              (yyval.node) = create_node("Return", NULL);
              if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
          }
#line 1803 "y.tab.c"
    break;

  case 35: /* Statement: MethodInvocation SEMICOLON  */
#line 245 "jucompiler.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 1809 "y.tab.c"
    break;

  case 36: /* Statement: Assignment SEMICOLON  */
#line 246 "jucompiler.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1815 "y.tab.c"
    break;

  case 37: /* Statement: ParseArgs SEMICOLON  */
#line 247 "jucompiler.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 1821 "y.tab.c"
    break;

  case 38: /* Statement: PRINT LPAR ExprOrStrlit RPAR SEMICOLON  */
#line 248 "jucompiler.y"
                                                   {
              (yyval.node) = create_node("Print", NULL);
              add_child((yyval.node), (yyvsp[-2].node));
          }
#line 1830 "y.tab.c"
    break;

  case 39: /* Statement: error SEMICOLON  */
#line 252 "jucompiler.y"
                            { (yyval.node) = NULL; }
#line 1836 "y.tab.c"
    break;

  case 40: /* Statement: SEMICOLON  */
#line 253 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 1842 "y.tab.c"
    break;

  case 41: /* OptExpr: %empty  */
#line 256 "jucompiler.y"
          { (yyval.node) = NULL; }
#line 1848 "y.tab.c"
    break;

  case 42: /* OptExpr: Expr  */
#line 257 "jucompiler.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1854 "y.tab.c"
    break;

  case 43: /* ExprOrStrlit: Expr  */
#line 260 "jucompiler.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1860 "y.tab.c"
    break;

  case 44: /* ExprOrStrlit: STRLIT  */
#line 261 "jucompiler.y"
                      { (yyval.node) = create_node("StrLit", (yyvsp[0].str)); }
#line 1866 "y.tab.c"
    break;

  case 45: /* MethodInvocation: IDENTIFIER LPAR OptExprList RPAR  */
#line 264 "jucompiler.y"
                                                    {
                     (yyval.node) = create_node("Call", NULL);
                     Node* id = create_node("Identifier", (yyvsp[-3].str));
                     add_child((yyval.node), id);
                     if ((yyvsp[-1].node)) add_sibling(id, (yyvsp[-1].node));
                 }
#line 1877 "y.tab.c"
    break;

  case 46: /* MethodInvocation: IDENTIFIER LPAR error RPAR  */
#line 270 "jucompiler.y"
                                              { (yyval.node) = NULL; }
#line 1883 "y.tab.c"
    break;

  case 47: /* OptExprList: %empty  */
#line 273 "jucompiler.y"
              { (yyval.node) = NULL; }
#line 1889 "y.tab.c"
    break;

  case 48: /* OptExprList: ExprList  */
#line 274 "jucompiler.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1895 "y.tab.c"
    break;

  case 49: /* ExprList: Expr  */
#line 277 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1901 "y.tab.c"
    break;

  case 50: /* ExprList: ExprList COMMA Expr  */
#line 278 "jucompiler.y"
                               { (yyval.node) = add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1907 "y.tab.c"
    break;

  case 51: /* Assignment: IDENTIFIER ASSIGN Expr  */
#line 281 "jucompiler.y"
                                    {
               (yyval.node) = create_node("Assign", NULL);
               Node* id = create_node("Identifier", (yyvsp[-2].str));
               add_child((yyval.node), id);
               add_sibling(id, (yyvsp[0].node));
           }
#line 1918 "y.tab.c"
    break;

  case 52: /* ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR  */
#line 289 "jucompiler.y"
                                                       {
              (yyval.node) = create_node("ParseArgs", NULL);
              Node* id = create_node("Identifier", (yyvsp[-4].str));
              add_child((yyval.node), id);
              add_sibling(id, (yyvsp[-2].node));
          }
#line 1929 "y.tab.c"
    break;

  case 53: /* ParseArgs: PARSEINT LPAR error RPAR  */
#line 295 "jucompiler.y"
                                     { (yyval.node) = NULL; }
#line 1935 "y.tab.c"
    break;

  case 54: /* Expr: Expr PLUS Expr  */
#line 298 "jucompiler.y"
                        { (yyval.node) = create_node("Add", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1941 "y.tab.c"
    break;

  case 55: /* Expr: Expr MINUS Expr  */
#line 299 "jucompiler.y"
                        { (yyval.node) = create_node("Sub", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1947 "y.tab.c"
    break;

  case 56: /* Expr: Expr STAR Expr  */
#line 300 "jucompiler.y"
                        { (yyval.node) = create_node("Mul", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1953 "y.tab.c"
    break;

  case 57: /* Expr: Expr DIV Expr  */
#line 301 "jucompiler.y"
                        { (yyval.node) = create_node("Div", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1959 "y.tab.c"
    break;

  case 58: /* Expr: Expr MOD Expr  */
#line 302 "jucompiler.y"
                        { (yyval.node) = create_node("Mod", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1965 "y.tab.c"
    break;

  case 59: /* Expr: Expr AND Expr  */
#line 303 "jucompiler.y"
                        { (yyval.node) = create_node("And", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1971 "y.tab.c"
    break;

  case 60: /* Expr: Expr OR Expr  */
#line 304 "jucompiler.y"
                        { (yyval.node) = create_node("Or", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1977 "y.tab.c"
    break;

  case 61: /* Expr: Expr XOR Expr  */
#line 305 "jucompiler.y"
                        { (yyval.node) = create_node("Xor", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1983 "y.tab.c"
    break;

  case 62: /* Expr: Expr LSHIFT Expr  */
#line 306 "jucompiler.y"
                        { (yyval.node) = create_node("Lshift", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1989 "y.tab.c"
    break;

  case 63: /* Expr: Expr RSHIFT Expr  */
#line 307 "jucompiler.y"
                        { (yyval.node) = create_node("Rshift", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1995 "y.tab.c"
    break;

  case 64: /* Expr: Expr EQ Expr  */
#line 308 "jucompiler.y"
                        { (yyval.node) = create_node("Eq", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2001 "y.tab.c"
    break;

  case 65: /* Expr: Expr GE Expr  */
#line 309 "jucompiler.y"
                        { (yyval.node) = create_node("Ge", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2007 "y.tab.c"
    break;

  case 66: /* Expr: Expr GT Expr  */
#line 310 "jucompiler.y"
                        { (yyval.node) = create_node("Gt", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2013 "y.tab.c"
    break;

  case 67: /* Expr: Expr LE Expr  */
#line 311 "jucompiler.y"
                        { (yyval.node) = create_node("Le", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2019 "y.tab.c"
    break;

  case 68: /* Expr: Expr LT Expr  */
#line 312 "jucompiler.y"
                        { (yyval.node) = create_node("Lt", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2025 "y.tab.c"
    break;

  case 69: /* Expr: Expr NE Expr  */
#line 313 "jucompiler.y"
                        { (yyval.node) = create_node("Ne", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2031 "y.tab.c"
    break;

  case 70: /* Expr: MINUS Expr  */
#line 314 "jucompiler.y"
                                    { (yyval.node) = create_node("Minus", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2037 "y.tab.c"
    break;

  case 71: /* Expr: PLUS Expr  */
#line 315 "jucompiler.y"
                                    { (yyval.node) = create_node("Plus", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2043 "y.tab.c"
    break;

  case 72: /* Expr: NOT Expr  */
#line 316 "jucompiler.y"
                        { (yyval.node) = create_node("Not", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2049 "y.tab.c"
    break;

  case 73: /* Expr: LPAR Expr RPAR  */
#line 317 "jucompiler.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 2055 "y.tab.c"
    break;

  case 74: /* Expr: LPAR error RPAR  */
#line 318 "jucompiler.y"
                        { (yyval.node) = NULL; }
#line 2061 "y.tab.c"
    break;

  case 75: /* Expr: MethodInvocation  */
#line 319 "jucompiler.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2067 "y.tab.c"
    break;

  case 76: /* Expr: Assignment  */
#line 320 "jucompiler.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2073 "y.tab.c"
    break;

  case 77: /* Expr: ParseArgs  */
#line 321 "jucompiler.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2079 "y.tab.c"
    break;

  case 78: /* Expr: IDENTIFIER OptDotLength  */
#line 322 "jucompiler.y"
                               {
         if ((yyvsp[0].node)) {
             (yyval.node) = create_node("Length", NULL);
             add_child((yyval.node), create_node("Identifier", (yyvsp[-1].str)));
         } else {
             (yyval.node) = create_node("Identifier", (yyvsp[-1].str));
         }
     }
#line 2092 "y.tab.c"
    break;

  case 79: /* Expr: NATURAL  */
#line 330 "jucompiler.y"
                        { (yyval.node) = create_node("Natural", (yyvsp[0].str)); }
#line 2098 "y.tab.c"
    break;

  case 80: /* Expr: DECIMAL  */
#line 331 "jucompiler.y"
                        { (yyval.node) = create_node("Decimal", (yyvsp[0].str)); }
#line 2104 "y.tab.c"
    break;

  case 81: /* Expr: BOOLLIT  */
#line 332 "jucompiler.y"
                        { (yyval.node) = create_node("Bool", (yyvsp[0].str)); }
#line 2110 "y.tab.c"
    break;

  case 82: /* OptDotLength: %empty  */
#line 335 "jucompiler.y"
               { (yyval.node) = NULL; }
#line 2116 "y.tab.c"
    break;

  case 83: /* OptDotLength: DOTLENGTH  */
#line 336 "jucompiler.y"
                         { (yyval.node) = create_node("DotLength", NULL); }
#line 2122 "y.tab.c"
    break;


#line 2126 "y.tab.c"

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

#line 339 "jucompiler.y"

int get_error_col() { return col - yyleng; }

void yyerror(char *s) {
    syntax_errors_count++;
    printf("Line %d, col %d: %s: %s\n", line, get_error_col(), s, yytext);
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
