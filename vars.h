// Lisp variable tools

#include <stdlib.h>
#include "types.h" // Datatypes

#define NEW(x) malloc(sizeof(x))
#ifndef VARS_H
#define VARS_H
#define NEW_SPECIAL_OP(name,str) var_t name={.type=SPECIAL,.data.s=str}
#define NEW_SPECIAL_FORM(name) NEW_SPECIAL_OP(name,#name)
var_t NIL={.type=VOID,.data.l=NULL};
var_t T={.type=SYMBOL,.data.s="T"};
var_t SELF={.type=VARIABLE,.data.s="#"};
NEW_SPECIAL_FORM(CAR);
NEW_SPECIAL_FORM(CDR);
NEW_SPECIAL_FORM(CONS);
NEW_SPECIAL_FORM(DISPLAY);
NEW_SPECIAL_FORM(EQ);
NEW_SPECIAL_FORM(ATOM);
NEW_SPECIAL_FORM(DEFINE);
NEW_SPECIAL_FORM(LAMBDA);
NEW_SPECIAL_FORM(PROGN);
NEW_SPECIAL_FORM(TERPRI);
NEW_SPECIAL_FORM(IF);
NEW_SPECIAL_FORM(EVAL);
NEW_SPECIAL_FORM(READ);
NEW_SPECIAL_FORM(EXIT);
NEW_SPECIAL_OP(ADD,"+");
NEW_SPECIAL_OP(SUB,"-");
NEW_SPECIAL_OP(MUL,"*");
NEW_SPECIAL_OP(DIV,"/");
NEW_SPECIAL_OP(MOD,"%");
NEW_SPECIAL_OP(GREATER,">");
NEW_SPECIAL_OP(LESS,"<");
NEW_SPECIAL_OP(EXPT,"^");
NEW_SPECIAL_FORM(AND);
NEW_SPECIAL_FORM(OR);
NEW_SPECIAL_FORM(RANDOM);
var_t *ENV;
var_t *new_ivar(long i)
{
	var_t *v=NEW(var_t);
	v->type=INT;
	v->data.i=i;
	return v;
}
var_t *new_fvar(double f)
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
