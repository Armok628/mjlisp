// Lisp core functions

#include "types.h" // Datatypes
#include "vars.h" // Variable tools

#ifndef CFUNC_H
#define CFUNC_H
var_t *cons(var_t *var1,var_t *var2)
{
	var_t *c=NEW(var_t);
	c->type=CELL;
	c->data.l=NEW(cell_t);
	c->data.l->car=var1;
	c->data.l->cdr=var2?var2:NIL;
	return c;
}
var_t *car(var_t *var)
{
	assert(var->type==CELL);
	return var->data.l->car;
}
var_t *cdr(var_t *var)
{
	assert(var->type==CELL);
	return var->data.l->cdr;
}
var_t *display(var_t *v)
{
	switch (v->type) {
		case SPECIAL: printf("%s",v->data.s);
			      return NIL;
		case VOID: printf("NIL");
			   return NIL;
		case INT: printf("%i",v->data.i);
			  return NIL;
		case FLOAT: printf("%f",v->data.f);
			    return NIL;
		case CHAR: printf("\\%c",v->data.c);
			   return NIL;
		case SYMBOL: printf("%s",v->data.s);
			     return NIL;
		case CELL: break;
		case FUNCTION: printf("#FUNCTION%i",v);
			       return NIL;
	}
	putchar('(');
	for (;v->type==CELL&&cdr(v);v=cdr(v)) {
		display(car(v));
		putchar(' ');
	}
	if (v&&v->type!=VOID) {
		printf(". ");
		display(v);
		putchar(' ');
	}
	printf("\e[D)");
	return NIL;
}
var_t *eq(var_t *v1,var_t *v2)
{
	if (v1->type!=v2->type)
		return NIL;
	if (v1->type==SYMBOL)
		return v1==v2||!strcmp(v1->data.s,v2->data.s)?T:NIL;
	if (v1->type==VOID&&v2->type==VOID)
		return T;
	if (v1->type!=CELL)
		return v1->data.i==v2->data.i?T:NIL;
	return NIL;
}
var_t *add(var_t *v1,var_t *v2)
{
	assert(v1->type==INT||v1->type==FLOAT);
	assert(v2->type==INT||v2->type==FLOAT);
	var_t *result=NEW(var_t);
	result->type=INT;
	if (v1->type==FLOAT||v2->type==FLOAT) {
		result->type=FLOAT;
		result->data.f=(v1->type==INT?v1->data.i:v1->data.f)
			+(v2->type==INT?v2->data.i:v2->data.f);
	} else 
		result->data.i=(v1->type==INT?v1->data.i:v1->data.f)
			+(v2->type==INT?v2->data.i:v2->data.f);
}
#endif
