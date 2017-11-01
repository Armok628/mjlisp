#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mjlisp.c"
int main(int argc,char **argv)
{
	srand(time(NULL));
	if (argc==2) {
		FILE *input_file=fopen(argv[1],"r");
		if (!input_file) {
			printf("Error: File %s can not be opened.\n",argv[1]);
			return 1;
		}
		fseek(input_file,0,SEEK_END);
		long length=ftell(input_file);
		rewind(input_file);
		char *input=malloc(length+1);
		fread(input,1,length,input_file);
		eval(read(input),&ENV);
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
