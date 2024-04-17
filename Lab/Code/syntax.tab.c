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
#line 2 "./syntax.y"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"


// 插入节点函数，argc表示参数个数，argv为参数数组
TreeNode* insertNode(int linenum, NodeType type, char* name, char* val, int argc, ...) {
    
    if (argc < 0) {
        return NULL;
    }
    
    Data* data = (Data*)malloc(sizeof(Data));
    data-> linenum = linenum;
    data-> type = type;
    data-> name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    data-> name = name;
    if(val != NULL){
        data-> val = (char*)malloc(sizeof(char) * (strlen(val) + 1));
        strncpy(data->val, val, sizeof(char) * (strlen(val) + 1));
    }
    else data-> val = NULL;

    TreeNode* parent = (TreeNode*)malloc(sizeof(TreeNode));
    parent->data = data;
    parent->firstChild = NULL;
    parent->nextSibling = NULL;

    va_list argv;
    va_start(argv, argc);

    if(argc > 0){
        TreeNode* tempNode = va_arg(argv, TreeNode*);
        parent->firstChild = tempNode;
        //printf("arg0:%x\n",(void*)tempNode);
        for (int i = 1; i < argc; i++) {
            if(tempNode == NULL){printf("arg %d wrong at insertNode %s, line:%d!\n",i-1,name,linenum); continue;}
            tempNode->nextSibling = va_arg(argv, TreeNode*);
            if (tempNode->nextSibling != NULL) {
                tempNode = tempNode->nextSibling;
            }
            //printf("arg%d:%x\n",i,(void*)tempNode);
        }
    }

    va_end(argv);

    //printf("insertNode %s, line:%d ,of %x, val %s\n",name,linenum,(void*)parent,val);

    return parent;
}

// 删除节点函数
void deleteNode(TreeNode *node) {
    if (node == NULL) {
        return;
    }

    deleteNode(node->firstChild); // 递归删除子节点
    deleteNode(node->nextSibling); // 递归删除兄弟节点

    free(node);
}

// 打印节点函数
void printTree(TreeNode *node, int height) {
    if (node == NULL) {
        return;
    }

    if(node->data == NULL) assert(0);
    if(node->data->name == NULL) assert(0);
    for(int i = 1; i <= height; i++){
        printf("  ");
    }
    printf("%s", node->data->name);
    //printf("%s,%x,%d", node->data->name,(void*)node,height);
    NodeType curtype = node->data->type;
    if (curtype == TYPE_NO) {
        printf(" (%d)", node->data->linenum);
    } else if (curtype == TYPE_TYPE || curtype == TYPE_ID || curtype == TYPE_INT) {
        printf(": %s", node->data->val);
    } else if (curtype == TYPE_FLOAT) {
        printf(": %f", strtof(node->data->val, NULL));
    }
    printf("\n");
    printTree(node->firstChild, height + 1); // 打印子节点
    //printf("%s,%x,%d\n", node->data->name,(void*)node,height);
    printTree(node->nextSibling, height); // 打印兄弟节点
}

TreeNode* root = NULL; //根节点

#include "lex.yy.c"
// int yylex();
extern void semantic_check(TreeNode* root);
bool has_error = 0; // 存在错误
void yyerror(char *s){
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, s);
};


#line 180 "./syntax.tab.c"

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

#include "syntax.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TYPE = 3,                       /* TYPE  */
  YYSYMBOL_PLUS = 4,                       /* PLUS  */
  YYSYMBOL_MINUS = 5,                      /* MINUS  */
  YYSYMBOL_STAR = 6,                       /* STAR  */
  YYSYMBOL_DIV = 7,                        /* DIV  */
  YYSYMBOL_LB = 8,                         /* LB  */
  YYSYMBOL_LC = 9,                         /* LC  */
  YYSYMBOL_LP = 10,                        /* LP  */
  YYSYMBOL_RB = 11,                        /* RB  */
  YYSYMBOL_RC = 12,                        /* RC  */
  YYSYMBOL_RP = 13,                        /* RP  */
  YYSYMBOL_SEMI = 14,                      /* SEMI  */
  YYSYMBOL_ASSIGNOP = 15,                  /* ASSIGNOP  */
  YYSYMBOL_COMMA = 16,                     /* COMMA  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_NOT = 19,                       /* NOT  */
  YYSYMBOL_RELOP = 20,                     /* RELOP  */
  YYSYMBOL_DOT = 21,                       /* DOT  */
  YYSYMBOL_STRUCT = 22,                    /* STRUCT  */
  YYSYMBOL_RETURN = 23,                    /* RETURN  */
  YYSYMBOL_IF = 24,                        /* IF  */
  YYSYMBOL_ELSE = 25,                      /* ELSE  */
  YYSYMBOL_WHILE = 26,                     /* WHILE  */
  YYSYMBOL_INT = 27,                       /* INT  */
  YYSYMBOL_FLOAT = 28,                     /* FLOAT  */
  YYSYMBOL_ID = 29,                        /* ID  */
  YYSYMBOL_UNMINUS = 30,                   /* UNMINUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 31,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_Program = 33,                   /* Program  */
  YYSYMBOL_ExtDefList = 34,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 35,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 36,                /* ExtDecList  */
  YYSYMBOL_Specifier = 37,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 38,           /* StructSpecifier  */
  YYSYMBOL_OptTag = 39,                    /* OptTag  */
  YYSYMBOL_Tag = 40,                       /* Tag  */
  YYSYMBOL_VarDec = 41,                    /* VarDec  */
  YYSYMBOL_FunDec = 42,                    /* FunDec  */
  YYSYMBOL_VarList = 43,                   /* VarList  */
  YYSYMBOL_ParamDec = 44,                  /* ParamDec  */
  YYSYMBOL_CompSt = 45,                    /* CompSt  */
  YYSYMBOL_StmtList = 46,                  /* StmtList  */
  YYSYMBOL_Stmt = 47,                      /* Stmt  */
  YYSYMBOL_DefList = 48,                   /* DefList  */
  YYSYMBOL_Def = 49,                       /* Def  */
  YYSYMBOL_DecList = 50,                   /* DecList  */
  YYSYMBOL_Dec = 51,                       /* Dec  */
  YYSYMBOL_Exp = 52,                       /* Exp  */
  YYSYMBOL_Args = 53                       /* Args  */
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
typedef yytype_int8 yy_state_t;

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   270

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   286


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
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   145,   145,   148,   149,   152,   153,   154,   155,   156,
     159,   160,   165,   166,   169,   170,   173,   174,   177,   182,
     183,   184,   187,   188,   189,   192,   193,   196,   201,   202,
     205,   206,   209,   210,   211,   212,   213,   214,   215,   220,
     221,   224,   227,   228,   231,   232,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   256,   257
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
  "\"end of file\"", "error", "\"invalid token\"", "TYPE", "PLUS",
  "MINUS", "STAR", "DIV", "LB", "LC", "LP", "RB", "RC", "RP", "SEMI",
  "ASSIGNOP", "COMMA", "AND", "OR", "NOT", "RELOP", "DOT", "STRUCT",
  "RETURN", "IF", "ELSE", "WHILE", "INT", "FLOAT", "ID", "UNMINUS",
  "LOWER_THAN_ELSE", "$accept", "Program", "ExtDefList", "ExtDef",
  "ExtDecList", "Specifier", "StructSpecifier", "OptTag", "Tag", "VarDec",
  "FunDec", "VarList", "ParamDec", "CompSt", "StmtList", "Stmt", "DefList",
  "Def", "DecList", "Dec", "Exp", "Args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-67)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-32)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      15,     6,   -67,   -19,    21,   -67,    15,    24,   -67,   -67,
      32,    33,   -67,   -67,   -67,    72,   -67,    51,    57,    -4,
     256,    42,   -67,   -67,     4,   -67,    47,    10,    70,    42,
     -67,   -67,    10,    74,    42,   -67,    10,    76,    85,    97,
      98,   -67,   -67,   -67,    39,    64,   101,   102,   -67,   -67,
     118,   -67,    42,   -67,    38,    83,    83,    83,    83,   119,
     121,   -67,   -67,   123,   -67,   115,    39,    99,    83,   -67,
      10,   -67,   -67,    -2,   117,    -2,   135,    83,    83,    71,
     -67,   -67,    83,    83,    83,    83,    83,   -67,    83,    83,
      83,    83,   107,   225,   -67,   -67,   -67,   153,   171,   -67,
     189,   131,    49,    49,    -2,    -2,   207,   225,   248,   230,
     195,   -67,    68,    68,    83,   -67,   -67,   103,   -67,   -67,
      68,   -67
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    12,    17,     0,     2,     0,     0,    13,     9,
      18,     0,    15,     1,     3,     0,     6,    19,     0,    10,
       0,    40,    21,    24,     0,     5,     0,     0,     0,    40,
       8,     7,     0,     0,    40,    23,     0,     0,    26,     0,
       0,    19,    11,    29,     0,    44,     0,    42,    14,    39,
      27,    22,     0,    20,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    61,    33,     0,     0,     0,     0,    41,
       0,    25,    38,    55,     0,    56,     0,     0,     0,     0,
      28,    30,     0,     0,     0,     0,     0,    32,     0,     0,
       0,     0,     0,    45,    43,    54,    34,     0,     0,    58,
      65,     0,    50,    51,    52,    53,     0,    46,    47,    48,
      49,    60,     0,     0,     0,    57,    59,    35,    37,    64,
       0,    36
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -67,   -67,   139,   -67,   120,     8,   -67,   -67,   -67,   -27,
     -67,    94,   -67,   128,    88,   -66,    46,   -67,    81,   -67,
     -55,    48
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,    18,    32,     8,    11,    12,    19,
      20,    37,    38,    64,    65,    66,    33,    34,    46,    47,
      67,   101
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      73,    74,    75,    76,    26,    45,    86,     2,     7,    50,
      10,    40,    27,    93,     7,    -4,     1,    35,     2,    92,
       9,    13,    97,    98,   100,    15,     3,   102,   103,   104,
     105,   106,    36,   107,   108,   109,   110,     3,    16,    41,
      54,   -16,    21,    45,    55,     2,   117,   118,    29,    56,
      43,   -31,    72,    17,   121,    84,    85,    86,    57,   100,
      36,    24,    58,    59,     3,    60,    61,    62,    63,    54,
      92,    25,    26,    55,    39,    44,    55,    29,    56,    68,
      49,    56,    43,    22,    99,    23,    48,    57,    55,    51,
      57,    58,    59,    56,    60,    61,    62,    63,    61,    62,
      63,    52,    57,    82,    83,    84,    85,    86,    53,    22,
      61,    62,    63,    87,    88,    69,    89,    90,    70,    91,
      92,    82,    83,    84,    85,    86,    26,    80,   120,    77,
      95,    78,    88,    79,    89,    90,   111,    91,    92,    82,
      83,    84,    85,    86,   115,    14,    71,    42,    31,    96,
      88,    94,    89,    90,    81,    91,    92,    82,    83,    84,
      85,    86,   119,     0,     0,     0,   112,     0,    88,     0,
      89,    90,     0,    91,    92,    82,    83,    84,    85,    86,
       0,     0,     0,     0,   113,     0,    88,     0,    89,    90,
       0,    91,    92,    82,    83,    84,    85,    86,     0,    82,
      83,    84,    85,    86,    88,   114,    89,    90,     0,    91,
      92,    82,    83,    84,    85,    86,    92,     0,   116,     0,
       0,     0,    88,     0,    89,    90,     0,    91,    92,    82,
      83,    84,    85,    86,    82,    83,    84,    85,    86,     0,
      88,     0,    89,    90,     0,    91,    92,    89,     0,     0,
      91,    92,    82,    83,    84,    85,    86,    28,     0,     0,
       0,     0,     0,     0,     0,    29,     0,     0,    91,    92,
      30
};

static const yytype_int8 yycheck[] =
{
      55,    56,    57,    58,     8,    32,     8,     3,     0,    36,
      29,     1,    16,    68,     6,     0,     1,    13,     3,    21,
      14,     0,    77,    78,    79,     1,    22,    82,    83,    84,
      85,    86,    24,    88,    89,    90,    91,    22,    14,    29,
       1,     9,     9,    70,     5,     3,   112,   113,     9,    10,
      12,    12,    14,    29,   120,     6,     7,     8,    19,   114,
      52,    10,    23,    24,    22,    26,    27,    28,    29,     1,
      21,    14,     8,     5,    27,    29,     5,     9,    10,    15,
      34,    10,    12,    11,    13,    13,    12,    19,     5,    13,
      19,    23,    24,    10,    26,    27,    28,    29,    27,    28,
      29,    16,    19,     4,     5,     6,     7,     8,    11,    11,
      27,    28,    29,    14,    15,    14,    17,    18,    16,    20,
      21,     4,     5,     6,     7,     8,     8,    12,    25,    10,
      13,    10,    15,    10,    17,    18,    29,    20,    21,     4,
       5,     6,     7,     8,    13,     6,    52,    27,    20,    14,
      15,    70,    17,    18,    66,    20,    21,     4,     5,     6,
       7,     8,   114,    -1,    -1,    -1,    13,    -1,    15,    -1,
      17,    18,    -1,    20,    21,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    13,    -1,    15,    -1,    17,    18,
      -1,    20,    21,     4,     5,     6,     7,     8,    -1,     4,
       5,     6,     7,     8,    15,    16,    17,    18,    -1,    20,
      21,     4,     5,     6,     7,     8,    21,    -1,    11,    -1,
      -1,    -1,    15,    -1,    17,    18,    -1,    20,    21,     4,
       5,     6,     7,     8,     4,     5,     6,     7,     8,    -1,
      15,    -1,    17,    18,    -1,    20,    21,    17,    -1,    -1,
      20,    21,     4,     5,     6,     7,     8,     1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    20,    21,
      14
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,    22,    33,    34,    35,    37,    38,    14,
      29,    39,    40,     0,    34,     1,    14,    29,    36,    41,
      42,     9,    11,    13,    10,    14,     8,    16,     1,     9,
      14,    45,    37,    48,    49,    13,    37,    43,    44,    27,
       1,    29,    36,    12,    48,    41,    50,    51,    12,    48,
      41,    13,    16,    11,     1,     5,    10,    19,    23,    24,
      26,    27,    28,    29,    45,    46,    47,    52,    15,    14,
      16,    43,    14,    52,    52,    52,    52,    10,    10,    10,
      12,    46,     4,     5,     6,     7,     8,    14,    15,    17,
      18,    20,    21,    52,    50,    13,    14,    52,    52,    13,
      52,    53,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    29,    13,    13,    16,    13,    11,    47,    47,    53,
      25,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    35,    35,
      36,    36,    37,    37,    38,    38,    39,    39,    40,    41,
      41,    41,    42,    42,    42,    43,    43,    44,    45,    45,
      46,    46,    47,    47,    47,    47,    47,    47,    47,    48,
      48,    49,    50,    50,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     3,     2,
       1,     3,     1,     1,     5,     2,     1,     0,     1,     1,
       4,     2,     4,     3,     2,     3,     1,     2,     4,     2,
       2,     0,     2,     1,     3,     5,     7,     5,     2,     2,
       0,     3,     1,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     4,     3,     4,
       3,     1,     1,     1,     3,     1
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: ExtDefList  */
#line 145 "./syntax.y"
                     {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Program",NULL,1,(yyvsp[0].type_node)); root = (yyval.type_node); if(!has_error){printTree(root, 0);semantic_check(root); }}
#line 1443 "./syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 148 "./syntax.y"
                               {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ExtDefList",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1449 "./syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 149 "./syntax.y"
      {(yyval.type_node) = NULL;}
#line 1455 "./syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 152 "./syntax.y"
                                   {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ExtDef",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1461 "./syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 153 "./syntax.y"
                     {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ExtDef",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1467 "./syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FunDec CompSt  */
#line 154 "./syntax.y"
                              {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ExtDef",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1473 "./syntax.tab.c"
    break;

  case 8: /* ExtDef: Specifier FunDec SEMI  */
#line 155 "./syntax.y"
                            {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ExtDef",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1479 "./syntax.tab.c"
    break;

  case 9: /* ExtDef: error SEMI  */
#line 156 "./syntax.y"
                                              { has_error = 1; }
#line 1485 "./syntax.tab.c"
    break;

  case 10: /* ExtDecList: VarDec  */
#line 159 "./syntax.y"
                    {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ExtDecList",NULL,1,(yyvsp[0].type_node)); }
#line 1491 "./syntax.tab.c"
    break;

  case 11: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 160 "./syntax.y"
                              {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ExtDecList",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1497 "./syntax.tab.c"
    break;

  case 12: /* Specifier: TYPE  */
#line 165 "./syntax.y"
                 {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Specifier",NULL,1,(yyvsp[0].type_node)); }
#line 1503 "./syntax.tab.c"
    break;

  case 13: /* Specifier: StructSpecifier  */
#line 166 "./syntax.y"
                      {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Specifier",NULL,1,(yyvsp[0].type_node)); }
#line 1509 "./syntax.tab.c"
    break;

  case 14: /* StructSpecifier: STRUCT OptTag LC DefList RC  */
#line 169 "./syntax.y"
                                              {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"StructSpecifier",NULL,5,(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1515 "./syntax.tab.c"
    break;

  case 15: /* StructSpecifier: STRUCT Tag  */
#line 170 "./syntax.y"
                 {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"StructSpecifier",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1521 "./syntax.tab.c"
    break;

  case 16: /* OptTag: ID  */
#line 173 "./syntax.y"
            {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"OptTag",NULL,1,(yyvsp[0].type_node)); }
#line 1527 "./syntax.tab.c"
    break;

  case 17: /* OptTag: %empty  */
#line 174 "./syntax.y"
      {(yyval.type_node) = NULL;}
#line 1533 "./syntax.tab.c"
    break;

  case 18: /* Tag: ID  */
#line 177 "./syntax.y"
         {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Tag",NULL,1,(yyvsp[0].type_node)); }
#line 1539 "./syntax.tab.c"
    break;

  case 19: /* VarDec: ID  */
#line 182 "./syntax.y"
            {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"VarDec",NULL,1,(yyvsp[0].type_node)); }
#line 1545 "./syntax.tab.c"
    break;

  case 20: /* VarDec: VarDec LB INT RB  */
#line 183 "./syntax.y"
                       {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"VarDec",NULL,4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1551 "./syntax.tab.c"
    break;

  case 21: /* VarDec: error RB  */
#line 184 "./syntax.y"
                                              { has_error = 1; }
#line 1557 "./syntax.tab.c"
    break;

  case 22: /* FunDec: ID LP VarList RP  */
#line 187 "./syntax.y"
                          {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"FunDec",NULL,4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1563 "./syntax.tab.c"
    break;

  case 23: /* FunDec: ID LP RP  */
#line 188 "./syntax.y"
               {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"FunDec",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1569 "./syntax.tab.c"
    break;

  case 24: /* FunDec: error RP  */
#line 189 "./syntax.y"
                                              { has_error = 1; }
#line 1575 "./syntax.tab.c"
    break;

  case 25: /* VarList: ParamDec COMMA VarList  */
#line 192 "./syntax.y"
                                 {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"VarList",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1581 "./syntax.tab.c"
    break;

  case 26: /* VarList: ParamDec  */
#line 193 "./syntax.y"
               {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"VarList",NULL,1,(yyvsp[0].type_node)); }
#line 1587 "./syntax.tab.c"
    break;

  case 27: /* ParamDec: Specifier VarDec  */
#line 196 "./syntax.y"
                            {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"ParamDec",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1593 "./syntax.tab.c"
    break;

  case 28: /* CompSt: LC DefList StmtList RC  */
#line 201 "./syntax.y"
                                {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"CompSt",NULL,4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1599 "./syntax.tab.c"
    break;

  case 29: /* CompSt: error RC  */
#line 202 "./syntax.y"
                                              { has_error = 1; }
#line 1605 "./syntax.tab.c"
    break;

  case 30: /* StmtList: Stmt StmtList  */
#line 205 "./syntax.y"
                         {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"StmtList",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1611 "./syntax.tab.c"
    break;

  case 31: /* StmtList: %empty  */
#line 206 "./syntax.y"
      {(yyval.type_node) = NULL;}
#line 1617 "./syntax.tab.c"
    break;

  case 32: /* Stmt: Exp SEMI  */
#line 209 "./syntax.y"
                {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Stmt",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1623 "./syntax.tab.c"
    break;

  case 33: /* Stmt: CompSt  */
#line 210 "./syntax.y"
             {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Stmt",NULL,1,(yyvsp[0].type_node)); }
#line 1629 "./syntax.tab.c"
    break;

  case 34: /* Stmt: RETURN Exp SEMI  */
#line 211 "./syntax.y"
                      {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Stmt",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1635 "./syntax.tab.c"
    break;

  case 35: /* Stmt: IF LP Exp RP Stmt  */
#line 212 "./syntax.y"
                        {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Stmt",NULL,5,(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1641 "./syntax.tab.c"
    break;

  case 36: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 213 "./syntax.y"
                                  {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Stmt",NULL,7,(yyvsp[-6].type_node),(yyvsp[-5].type_node),(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1647 "./syntax.tab.c"
    break;

  case 37: /* Stmt: WHILE LP Exp RP Stmt  */
#line 214 "./syntax.y"
                           {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Stmt",NULL,5,(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1653 "./syntax.tab.c"
    break;

  case 38: /* Stmt: error SEMI  */
#line 215 "./syntax.y"
                                              { has_error = 1; }
#line 1659 "./syntax.tab.c"
    break;

  case 39: /* DefList: Def DefList  */
#line 220 "./syntax.y"
                      {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"DefList",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1665 "./syntax.tab.c"
    break;

  case 40: /* DefList: %empty  */
#line 221 "./syntax.y"
      {(yyval.type_node) = NULL;}
#line 1671 "./syntax.tab.c"
    break;

  case 41: /* Def: Specifier DecList SEMI  */
#line 224 "./syntax.y"
                             {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Def",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1677 "./syntax.tab.c"
    break;

  case 42: /* DecList: Dec  */
#line 227 "./syntax.y"
              {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"DecList",NULL,1,(yyvsp[0].type_node)); }
#line 1683 "./syntax.tab.c"
    break;

  case 43: /* DecList: Dec COMMA DecList  */
#line 228 "./syntax.y"
                        {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"DecList",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1689 "./syntax.tab.c"
    break;

  case 44: /* Dec: VarDec  */
#line 231 "./syntax.y"
             {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Dec",NULL,1,(yyvsp[0].type_node)); }
#line 1695 "./syntax.tab.c"
    break;

  case 45: /* Dec: VarDec ASSIGNOP Exp  */
#line 232 "./syntax.y"
                          {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Dec",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1701 "./syntax.tab.c"
    break;

  case 46: /* Exp: Exp ASSIGNOP Exp  */
#line 236 "./syntax.y"
                       {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1707 "./syntax.tab.c"
    break;

  case 47: /* Exp: Exp AND Exp  */
#line 237 "./syntax.y"
                  {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1713 "./syntax.tab.c"
    break;

  case 48: /* Exp: Exp OR Exp  */
#line 238 "./syntax.y"
                 {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1719 "./syntax.tab.c"
    break;

  case 49: /* Exp: Exp RELOP Exp  */
#line 239 "./syntax.y"
                    {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1725 "./syntax.tab.c"
    break;

  case 50: /* Exp: Exp PLUS Exp  */
#line 240 "./syntax.y"
                   {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1731 "./syntax.tab.c"
    break;

  case 51: /* Exp: Exp MINUS Exp  */
#line 241 "./syntax.y"
                    {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1737 "./syntax.tab.c"
    break;

  case 52: /* Exp: Exp STAR Exp  */
#line 242 "./syntax.y"
                   {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1743 "./syntax.tab.c"
    break;

  case 53: /* Exp: Exp DIV Exp  */
#line 243 "./syntax.y"
                  {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1749 "./syntax.tab.c"
    break;

  case 54: /* Exp: LP Exp RP  */
#line 244 "./syntax.y"
                {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1755 "./syntax.tab.c"
    break;

  case 55: /* Exp: MINUS Exp  */
#line 245 "./syntax.y"
                              {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1761 "./syntax.tab.c"
    break;

  case 56: /* Exp: NOT Exp  */
#line 246 "./syntax.y"
              {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,2,(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1767 "./syntax.tab.c"
    break;

  case 57: /* Exp: ID LP Args RP  */
#line 247 "./syntax.y"
                    {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1773 "./syntax.tab.c"
    break;

  case 58: /* Exp: ID LP RP  */
#line 248 "./syntax.y"
               {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1779 "./syntax.tab.c"
    break;

  case 59: /* Exp: Exp LB Exp RB  */
#line 249 "./syntax.y"
                    {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1785 "./syntax.tab.c"
    break;

  case 60: /* Exp: Exp DOT ID  */
#line 250 "./syntax.y"
                 {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1791 "./syntax.tab.c"
    break;

  case 61: /* Exp: ID  */
#line 251 "./syntax.y"
         {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,1,(yyvsp[0].type_node)); }
#line 1797 "./syntax.tab.c"
    break;

  case 62: /* Exp: INT  */
#line 252 "./syntax.y"
          {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,1,(yyvsp[0].type_node)); }
#line 1803 "./syntax.tab.c"
    break;

  case 63: /* Exp: FLOAT  */
#line 253 "./syntax.y"
            {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Exp",NULL,1,(yyvsp[0].type_node)); }
#line 1809 "./syntax.tab.c"
    break;

  case 64: /* Args: Exp COMMA Args  */
#line 256 "./syntax.y"
                      {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Args",NULL,3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node)); }
#line 1815 "./syntax.tab.c"
    break;

  case 65: /* Args: Exp  */
#line 257 "./syntax.y"
          {(yyval.type_node) = insertNode((yyloc).first_line,TYPE_NO,"Args",NULL,1,(yyvsp[0].type_node)); }
#line 1821 "./syntax.tab.c"
    break;


#line 1825 "./syntax.tab.c"

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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 260 "./syntax.y"


