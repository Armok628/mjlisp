#include <stdio.h>
#include <stdlib.h>
#include "mlib/mjlisp.h"
int main(int argc,char **argv)
{
	char *input=malloc(256),*c;
	for (;;) {
		fgets(input,255,stdin);
		for (c=input;*c;c++);
		*(c-1)='\0';
		display(eval(read(input),&ENV));
		putchar('\n');
	}
	return 0;
}
