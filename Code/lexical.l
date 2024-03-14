%{
#include <stdio.h>
#include <stdarg.h>
#include "syntax.tab.h"
#define DEBUG_MODE 1  // 控制是否输出log

void log_msg(const char* format, ...) {
    #if DEBUG_MODE
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    #endif
}
%}

digit [0-9]
letter [_a-zA-Z]
%%
"INT"|"FLOAT"  { log_msg("type: %s\n", yytext); return TYPE;}
{digit}+ { log_msg("Integer value: %s\n", yytext); return INT;}
{letter}({digit}|{letter})* { log_msg("ID value: %s\n", yytext); }
{digit}+"."{digit}+ { log_msg("Float value: %s\n", yytext);}
"+" { log_msg("PLUS\n");}
"-" { log_msg("MINUS\n");}
"*" { log_msg("STAR\n");}
"/" { log_msg("DIV\n");}
"(" { log_msg("LP\n");}
")" { log_msg("RP\n");}
"[" { log_msg("LB\n");}
"]" { log_msg("RB\n");}
"{" { log_msg("LC\n");}
"}" { log_msg("RC\n");}
";" { log_msg("SEMI\n");}
"=" { log_msg("ASSIGNOP\n");}
"," { log_msg("COMMA\n");}
.$ { log_msg("endline\n");}
\n { log_msg("\\n\n");}
" " {}
"\t" {}
. { log_msg("\033[1;31mError: Unexpected character '%s'\033[0m\n", yytext); }
%%

