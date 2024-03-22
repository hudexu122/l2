%{
#include <stdio.h>
#include <stdarg.h>
#include "syntax.tab.h"
#define DEBUG_MODE 0  // 控制是否输出log
int yycolumn = 1; 
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;

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
%option yylineno
%%
"struct" { log_msg("STRUCT\n"); return STRUCT;}
"return" { log_msg("RETURN\n"); return RETURN;}
"if" { log_msg("IF\n"); return IF;}
"else" { log_msg("ELSE\n"); return ELSE;}
"while" { log_msg("WHILE\n"); return WHILE;}
"int"|"float"  { log_msg("type: %s\n", yytext); return TYPE;}
0|([1-9]{digit}*) { log_msg("Integer value: %s\n", yytext); return INT;}
[0][0-7]+ { log_msg("Oct_Integer value: %s\n", yytext); return INT;}
[0][xX]([0-9A-Fa-f]*) { log_msg("16_Integer value: %s\n", yytext); return INT;}
{letter}({digit}|{letter})* { log_msg("ID value: %s\n", yytext); return ID;}
{digit}+"."{digit}+ { log_msg("Float value: %s\n", yytext); return FLOAT;}
"+" { log_msg("PLUS\n"); return PLUS;}
"-" { log_msg("MINUS\n"); return MINUS;}
"*" { log_msg("STAR\n"); return STAR;}
"/" { log_msg("DIV\n"); return DIV;}
"(" { log_msg("LP\n"); return LP;}
")" { log_msg("RP\n"); return RP;}
"[" { log_msg("LB\n"); return LB;}
"]" { log_msg("RB\n"); return RB;}
"{" { log_msg("LC\n"); return LC;}
"}" { log_msg("RC\n"); return RC;}
";" { log_msg("SEMI\n"); return SEMI;}
"=" { log_msg("ASSIGNOP\n"); return ASSIGNOP;}
"," { log_msg("COMMA\n"); return COMMA;}
.$ { log_msg("endline\n"); }
\n { log_msg("\\n\n"); yycolumn = 1;}
" " {}
"\t" {}
"\r" {}
. { fprintf(stderr, "Error type A at line %d: Unexpected character '%s'.\n", yylineno, yytext); }
%%

