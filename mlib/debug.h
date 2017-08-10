#include "types.h"
#include "vars.h"
#include "cfunc.h"
#include "mjlisp.h"
#ifndef DEBUG_H
#define DEBUG_H
void print_type_of(char *input)
{
	datatype type=infer_type(input);
	switch (type) {
		case SPECIAL: printf("SPECIAL\n");
			      break;
		case VOID: printf("VOID\n");
			   break;
		case VARIABLE: printf("VARIABLE\n");
			       break;
		case INT: printf("INT\n");
			  break;
		case FLOAT: printf("FLOAT\n");
			    break;
		case CHAR: printf("CHAR\n");
			   break;
		case SYMBOL: printf("SYMBOL\n");
			     break;
		case CELL: printf("CELL\n");
			   break;
		case FUNCTION: printf("FUNCTION\n");
			       break;
		default: printf("%i",type);
	}
}
#endif
