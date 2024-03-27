%{
#include <stdio.h>
#include <stdarg.h>
#include "syntax.tab.h"
#define DEBUG_MODE 0 // 控制是否输出log
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
"//".*\n {log_msg("//note\n"); yycolumn = 1;}
"/*"(.|\n)*"*/" {log_msg("/*note\n"); /*wrong*/}
"struct" { log_msg("STRUCT\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"STRUCT",NULL,0); return STRUCT;}
"return" { log_msg("RETURN\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"RETURN",NULL,0);return RETURN;}
"if" { log_msg("IF\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"IF",NULL,0);return IF;}
"else" { log_msg("ELSE\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"ELSE",NULL,0);return ELSE;}
"while" { log_msg("WHILE\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"WHILE",NULL,0);return WHILE;}
"int" { log_msg("type: %s\n", yytext); yylval.type_node = insertNode(yylineno,TYPE_TYPE,"TYPE","INT",0); return TYPE;}
"float" { log_msg("type: %s\n", yytext); yylval.type_node = insertNode(yylineno,TYPE_TYPE,"TYPE","FLOAT",0); return TYPE;}
0|([1-9]{digit}*) { log_msg("Integer value: %s\n", yytext); yylval.type_node = insertNode(yylineno,TYPE_INT,"INT",yytext,0); return INT;}
[0][0-7]+ { log_msg("8_Integer value: %s\n", yytext); yylval.type_node = insertNode(yylineno,TYPE_INT,"INT",NULL,0); return INT;}
[0][xX]([0-9A-Fa-f]*) { log_msg("16_Integer value: %s\n", yytext); yylval.type_node = insertNode(yylineno,TYPE_INT,"INT",NULL,0); return INT;}
{letter}({digit}|{letter})* { log_msg("ID value: %s\n", yytext); yylval.type_node = insertNode(yylineno,TYPE_ID,"ID",yytext,0); return ID;}
{digit}+"."{digit}+|{digit}*"."{digit}+[eE][+-]?{digit}+|{digit}+"."{digit}*[eE][+-]?{digit}+ { log_msg("Float value: %s\n", yytext); yylval.type_node = insertNode(yylineno,TYPE_FLOAT,"ID",yytext,0); return FLOAT;}
">"|"<"|">="|"<="|"=="|"!" {log_msg("RELOP\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"RELOP",NULL,0); return RELOP;}
"+" { log_msg("PLUS\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"PLUS",NULL,0);return PLUS;}
"-" { log_msg("MINUS\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"MINUS",NULL,0);return MINUS;}
"*" { log_msg("STAR\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"STAR",NULL,0);return STAR;}
"/" { log_msg("DIV\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"DIV",NULL,0);return DIV;}
"&&" { log_msg("AND\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"AND",NULL,0);return AND;}
"||" { log_msg("OR\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"OR",NULL,0);return OR;}
"(" { log_msg("LP\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"LP",NULL,0); return LP;}
")" { log_msg("RP\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"RP",NULL,0); return RP;}
"[" { log_msg("LB\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"LB",NULL,0); return LB;}
"]" { log_msg("RB\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"RB",NULL,0); return RB;}
"{" { log_msg("LC\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"LC",NULL,0); return LC;}
"}" { log_msg("RC\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"RC",NULL,0); return RC;}
";" { log_msg("SEMI\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"SEMI",NULL,0); return SEMI;}
"," { log_msg("COMMA\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"COMMA",NULL,0); return COMMA;}
"=" { log_msg("ASSIGNOP\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"ASSIGNOP",NULL,0); return ASSIGNOP;}
"." { log_msg("DOT\n"); yylval.type_node = insertNode(yylineno,TYPE_TOKEN,"DOT",NULL,0); return DOT;}
\n { log_msg("\\n\n"); yycolumn = 1;}
" " {}
"\t" {}
"\r" {}
. { fprintf(stderr, "Error type A at line %d: Unexpected character '%s'.\n", yylineno, yytext); }
%%

