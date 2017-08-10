// Lisp variable tools

#include <stdlib.h>
#include "types.h" // Datatypes

#define NEW(x) malloc(sizeof(x))
#ifndef VARS_H
#define VARS_H
var_t NIL_VAR={.type=VOID,.data.l=NULL};
var_t *NIL=&NIL_VAR;
var_t CAR_FUN={.type=SPECIAL,.data.s="CAR"};
var_t *CAR=&CAR_FUN;
var_t CDR_FUN={.type=SPECIAL,.data.s="CDR"};
var_t *CDR=&CDR_FUN;
var_t CONS_FUN={.type=SPECIAL,.data.s="CONS"};
var_t *CONS=&CONS_FUN;
var_t LAMBDA_FUN={.type=SPECIAL,.data.s="LAMBDA"};
var_t *LAMBDA=&LAMBDA_FUN;
var_t ADD_FUN={.type=SPECIAL,.data.s="ADD"};
var_t *ADD=&ADD_FUN;
var_t *new_ivar(int i)
{
	var_t *v=NEW(var_t);
	v->type=INT;
	v->data.i=i;
	return v;
}
var_t *new_fvar(float f)
{
	var_t *v=NEW(var_t);
	v->type=FLOAT;
	v->data.f=f;
	return v;
}
var_t *new_cvar(char c)
{
	var_t *v=NEW(var_t);
	v->type=CHAR;
	v->data.c=c;
	return v;
}
var_t *new_svar(char *s)
{
	var_t *v=NEW(var_t);
	v->type=SYMBOL;
	v->data.s=s;
	return v;
}
#endif