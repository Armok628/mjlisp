#include <stdio.h>
#include <stdlib.h>
#include "mlib.h"
void dbg_print_type(char *input);
int main(int argc,char **argv)
{
	char *input=malloc(64);
	fgets(input,63,stdin);
	char *c=input; for (;*c;c++); *(c-1)='\0';
	var_t *l=interpret_list(input);
	print_var(apply_function(car(l),cdr(l)));
	putchar('\n');
	return 0;
}
