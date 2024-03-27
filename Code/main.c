#include <stdio.h>
#include <stdbool.h>
/*extern FILE* yyin;

int main(int argc,char** argv){
	if (argc > 1){
		if(!(yyin = fopen(argv[1],"r"))){
			perror(argv[1]);
			return 1;
		}
	}
	while(yylex() != 0);
	return 0;
}*/

extern bool has_error;
//extern TreeNode* root = NULL; //根节点

int main(int argc, char** argv) {
    if (argc <= 1)
        return 1;
    
    FILE* f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    
    yyrestart(f);
    yyparse();
    
    //if(!has_error) printTree(root,0);

    return 0;
}
