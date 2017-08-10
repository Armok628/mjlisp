// Lisp core functions

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
