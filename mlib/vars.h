// Lisp variable tools

#include <stdlib.h>
#include "types.h" // Datatypes

#define NEW(x) malloc(sizeof(x))
#ifndef VARS_H
#define VARS_H
var_t NIL_VAR={.type=VOID,.data.l=NULL};
var_t *NIL=&NIL_VAR;
var_t T_SYMBOL={.type=SYMBOL,.data.s="T"};
var_t *T=&T_SYMBOL;
var_t CAR_FUN={.type=SPECIAL,.data.s="CAR"};
var_t *CAR=&CAR_FUN;
var_t CDR_FUN={.type=SPECIAL,.data.s="CDR"};
var_t *CDR=&CDR_FUN;
var_t CONS_FUN={.type=SPECIAL,.data.s="CONS"};
var_t *CONS=&CONS_FUN;
var_t DISPLAY_FUN={.type=SPECIAL,.data.s="DISPLAY"};
var_t *DISPLAY=&DISPLAY_FUN;
var_t EQ_FUN={.type=SPECIAL,.data.s="EQ"};
var_t *EQ=&EQ_FUN;
var_t ATOM_FUN={.type=SPECIAL,.data.s="ATOM"};
var_t *ATOM=&ATOM_FUN;
var_t DEFINE_FUN={.type=SPECIAL,.data.s="DEFINE"};
var_t *DEFINE=&DEFINE_FUN;
var_t LAMBDA_FUN={.type=SPECIAL,.data.s="LAMBDA"};
var_t *LAMBDA=&LAMBDA_FUN;
var_t PROGN_FUN={.type=SPECIAL,.data.s="PROGN"};
var_t *PROGN=&PROGN_FUN;
var_t TERPRI_FUN={.type=SPECIAL,.data.s="TERPRI"};
var_t *TERPRI=&TERPRI_FUN;
var_t IF_FUN={.type=SPECIAL,.data.s="IF"};
var_t *IF=&IF_FUN;
var_t EVAL_FUN={.type=SPECIAL,.data.s="EVAL"};
var_t *EVAL=&EVAL_FUN;
var_t ADD_FUN={.type=SPECIAL,.data.s="+"};
var_t *ADD=&ADD_FUN;
var_t SUB_FUN={.type=SPECIAL,.data.s="-"};
var_t *SUB=&SUB_FUN;
var_t MUL_FUN={.type=SPECIAL,.data.s="*"};
var_t *MUL=&MUL_FUN;
var_t DIV_FUN={.type=SPECIAL,.data.s="/"};
var_t *DIV=&DIV_FUN;
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
