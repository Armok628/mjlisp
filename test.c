#include <stdio.h>
#include <stdlib.h>
#include "mlib/mjlisp.h"
#include "mlib/debug.h"
void dbg_print_type(char *input);
int main(int argc,char **argv)
{
	char *input=malloc(256);
	fgets(input,255,stdin);
	char *c=input; for (;*c;c++); *(c-1)='\0';
	//print_type_of(input);
	display(eval(input));
	putchar('\n');
	return 0;
}
