// Lisp core functions

#include <math.h>
#include "types.h" // Datatypes
#include "vars.h" // Variable tools

#ifndef CFUNC_H
#define CFUNC_H
#define ASSERTM(x,m) if (!(x)) {printf("\nFatal error: " m "\n\n"); exit(1);}
var_t *cons(var_t *var1,var_t *var2)
{
	var_t *c=NEW(var_t);
	c->type=CELL;
	c->data.l=NEW(cell_t);
	c->data.l->car=var1;
	c->data.l->cdr=var2?var2:&NIL;
	return c;
}
var_t *car(var_t *var)
{
	ASSERTM(var->type==CELL||var->type==QUOTE
			||var->type==VOID||var->type==FUNCTION
			,"car(): Invalid argument datatype");
	if (var==&NIL)
		return &NIL;
	return var->data.l->car;
}
var_t *cdr(var_t *var)
{
	ASSERTM(var->type==CELL||var->type==QUOTE
			||var->type==VOID||var->type==FUNCTION
			,"cdr(): Invalid argument datatype");
	if (var==&NIL)
		return &NIL;
	return var->data.l->cdr;
}
var_t *display(var_t *var)
{
	switch (var->type) {
		case VARIABLE:
		case SPECIAL: printf("%s",var->data.s);
			      return &NIL;
		case VOID: printf("NIL");
			   return &NIL;
		case INT: printf("%li",var->data.i);
			  return &NIL;
		case FLOAT: printf("%lf",var->data.f);
			    return &NIL;
		case CHAR: printf("%c",var->data.c);
			   return &NIL;
		case SYMBOL: printf("%s",var->data.s);
			     return &NIL;
		case FUNCTION: printf("#FUNCTION-%x",var);
			       return &NIL;
		case QUOTE:
		case CELL: break;
		default: printf("ERROR");
			 return NULL;
	}
	putchar('(');
	for (;var->type==CELL||var->type==QUOTE&&cdr(var);var=cdr(var)) {
		display(car(var));
		putchar(' ');
	}
	if (var&&var->type!=VOID) {
		printf(". ");
		display(var);
		putchar(' ');
	}
	printf("\e[D)");
	return &NIL;
}
var_t *eq(var_t *v1,var_t *v2)
{
	if (v1->type!=v2->type)
		return &NIL;
	if (v1->type==SYMBOL||v1->type==VARIABLE)
		return v1==v2||!strcmp(v1->data.s,v2->data.s)?&T:&NIL;
	if (v1->type==VOID&&v2->type==VOID)
		return &T;
	if (v1->type!=CELL&&v1->type!=QUOTE)
		return v1->data.i==v2->data.i?&T:&NIL;
	return &NIL;
}
var_t *atom(var_t *var)
{
	return var->type==CELL||var->type==QUOTE?&NIL:&T;
}
var_t *terpri()
{
	putchar('\n');
	return &NIL;
}
// Arithmetic
var_t *arith(var_t *v1,var_t *v2,char op)
{
	ASSERTM(v1->type==INT||v1->type==FLOAT,"arith(): First argument not a number");
	ASSERTM(v2->type==INT||v2->type==FLOAT,"arith(): Second argument not a number");
	var_t *result=NEW(var_t);
	result->type=INT;
	if (v1->type==FLOAT||v2->type==FLOAT
			||(op=='/'&&v1->data.i%v2->data.i!=0))
		goto FLOAT;
#define INT_OP_CASE(a,o,b) result->data.i=a->data.i o b->data.i; break
	switch (op) {
		case '+': INT_OP_CASE(v1,+,v2);
		case '-': INT_OP_CASE(v1,-,v2);
		case '*': INT_OP_CASE(v1,*,v2);
		case '/': INT_OP_CASE(v1,/,v2);
		case '%': INT_OP_CASE(v1,%,v2);
		case '^': result->data.i=pow(v1->data.i,v2->data.i);
	}
	goto RETURN;
FLOAT:  result->type=FLOAT;
#define NUMBER_VALUE(x) (x->type==INT?x->data.i:x->data.f)
#define FLOAT_OP_CASE(a,o,b) result->data.f=NUMBER_VALUE(a) o NUMBER_VALUE(b); break
	switch (op) {
		// Need to cope with the possibility that either variable is a float
		case '+': FLOAT_OP_CASE(v1,+,v2);
		case '-': FLOAT_OP_CASE(v1,-,v2);
		case '*': FLOAT_OP_CASE(v1,*,v2);
		case '/': FLOAT_OP_CASE(v1,/,v2);
		case '%': result->data.f=fmod(NUMBER_VALUE(v1),NUMBER_VALUE(v2));
			  break;
		case '^': result->data.f=pow(NUMBER_VALUE(v1),NUMBER_VALUE(v2));
	}
RETURN:	return result;
}
#endif
