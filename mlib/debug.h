#include "types.h"
#include "vars.h"
#include "cfunc.h"
#ifndef DEBUG_H
#define DEBUG_H
void print_type(datatype type)
{
	switch (type) {
		case SPECIAL: printf("SPECIAL");
			      break;
		case VOID: printf("VOID");
			   break;
		case VARIABLE: printf("VARIABLE");
			       break;
		case INT: printf("INT");
			  break;
		case FLOAT: printf("FLOAT");
			    break;
		case CHAR: printf("CHAR");
			   break;
		case SYMBOL: printf("SYMBOL");
			     break;
		case CELL: printf("CELL");
			   break;
		case QUOTE: printf("QUOTE");
			    break;
		case FUNCTION: printf("FUNCTION");
			       break;
		default: printf("%i",type);
	}
}
var_t *debug_display(var_t *var)
{
	switch (var->type) {
		case VARIABLE:
		case SPECIAL: printf("%s",var->data.s);
			      putchar('['); print_type(var->type); putchar(']');
			      return NIL;
		case VOID: printf("NIL");
     			   putchar('['); print_type(var->type); putchar(']');
			   return NIL;
		case INT: printf("%i",var->data.i);
    			  putchar('['); print_type(var->type); putchar(']');
			  return NIL;
		case FLOAT: printf("%f",var->data.f);
      			    putchar('['); print_type(var->type); putchar(']');
			    return NIL;
		case CHAR: printf("\\%c",var->data.c);
     			   putchar('['); print_type(var->type); putchar(']');
			   return NIL;
		case SYMBOL: printf("%s",var->data.s);
       			     putchar('['); print_type(var->type); putchar(']');
			     return NIL;
		case FUNCTION: printf("#FUNCTION-%x",var);
	 		       putchar('['); print_type(var->type); putchar(']');
			       return NIL;
		case QUOTE:
		case CELL: break;
		default: printf("ERROR");
			 return NULL;
	}
	datatype type=var->type;
	putchar('(');
	for (;var->type==CELL||var->type==QUOTE&&var->data.l->cdr;var=var->data.l->cdr) {
		debug_display(var->data.l->car);
		putchar(' ');
	}
	if (var&&var->type!=VOID) {
		printf(". ");
		debug_display(var);
		putchar(' ');
	}
	printf("\e[D)");
	putchar('['); print_type(type); putchar(']');
	return NIL;
}
#endif
