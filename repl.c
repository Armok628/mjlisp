#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mjlisp.c"
int main(int argc,char **argv)
{
	srand(time(NULL));
	if (argc==2) {
		load(argv[1],&ENV);
		return 0;
	}
	char *input=malloc(256),*c;
	for (;;) {
		fgets(input,255,stdin);
		display(eval(read(input),&ENV));
		putchar('\n');
		putchar('\n');
	}
	return 0;
}
