#include <stdio.h>
#include <stdlib.h>
#include "mlib/mjlisp.h"
#include "mlib/debug.h"
int main(int argc,char **argv)
{
	char *input=malloc(256),*c;
	fgets(input,255,stdin); for (c=input;*c;c++); *(c-1)='\0';
	eval(input);
	fgets(input,255,stdin); for (c=input;*c;c++); *(c-1)='\0';
	display(eval(input));
	putchar('\n');
	return 0;
}
