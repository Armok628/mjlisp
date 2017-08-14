// Lisp variable tools

#include <stdlib.h>
#include "types.h" // Datatypes

#define NEW(x) malloc(sizeof(x))
#ifndef VARS_H
#define VARS_H
var_t NIL={.type=VOID,.data.l=NULL};
var_t T={.type=SYMBOL,.data.s="T"};
var_t CAR={.type=SPECIAL,.data.s="CAR"};
var_t CDR={.type=SPECIAL,.data.s="CDR"};
var_t CONS={.type=SPECIAL,.data.s="CONS"};
var_t DISPLAY={.type=SPECIAL,.data.s="DISPLAY"};
var_t EQ={.type=SPECIAL,.data.s="EQ"};
var_t ATOM={.type=SPECIAL,.data.s="ATOM"};
var_t DEFINE={.type=SPECIAL,.data.s="DEFINE"};
var_t LAMBDA={.type=SPECIAL,.data.s="LAMBDA"};
var_t PROGN={.type=SPECIAL,.data.s="PROGN"};
var_t TERPRI={.type=SPECIAL,.data.s="TERPRI"};
var_t IF={.type=SPECIAL,.data.s="IF"};
var_t EVAL={.type=SPECIAL,.data.s="EVAL"};
var_t ADD={.type=SPECIAL,.data.s="+"};
var_t SUB={.type=SPECIAL,.data.s="-"};
var_t MUL={.type=SPECIAL,.data.s="*"};
var_t DIV={.type=SPECIAL,.data.s="/"};
var_t *ENV;
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
var_t *new_lvar(var_t *l)
{
	var_t *v=NEW(var_t);
	v->type=CELL;
	v->data.l=l->data.l;
	return v;
}
#endif
