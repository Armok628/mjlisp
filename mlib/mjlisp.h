// Lisp high-level functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h" // Datatypes
#include "vars.h" // Variable tools
#include "cfunc.h" // Core functions

#ifndef MJLISP_H
#define MJLISP_H
#include "debug.h"
bool is_whitespace(char c)
{
	return c==' '||c=='\n'||c=='\t'||c=='\0';
}
datatype infer_type(char *input)
{
	//printf("INFER_TYPE\n");
	if (!strcasecmp("NIL",input))
		return VOID;
	if (!strcasecmp("CAR",input)
			||!strcasecmp("CDR",input)
			||!strcasecmp("CONS",input)
			||!strcasecmp("DISPLAY",input)
			||!strcasecmp("EQ",input)
			||!strcasecmp("ATOM",input)
			||!strcasecmp("DEFINE",input)
			||!strcasecmp("PROGN",input)
			||!strcasecmp("LAMBDA",input)
			||!strcasecmp("TERPRI",input)
			||!strcasecmp("IF",input)
			||!strcasecmp("EVAL",input)
			||!strcasecmp("READ",input)
			||!strcasecmp("EXIT",input)
			||!strcmp("+",input)
			||!strcmp("-",input)
			||!strcmp("*",input)
			||!strcmp("/",input)
			||!strcmp(">",input)
			||!strcmp("<",input)
			||!strcasecmp("AND",input)
			||!strcasecmp("OR",input)
			||!strcasecmp("RANDOM",input))
		return SPECIAL;
	if (*input=='(') {
		if (*(input+1)==')')
			return VOID;
		char *lambda=malloc(7);
		strncpy(lambda,input+1,6);
		if (!strcasecmp("LAMBDA",lambda))
			return FUNCTION;
		else
			free(lambda);
		for (;*input&&*input!=')';input++);
		if (*input)
			return CELL;
		return ERROR;
	}
	if (*input=='\\') {
		ASSERTM(strlen(input)==2,"\nFatal error: Escaped non-character\n\n");
		return CHAR;
	}
	if (*input=='\'') {
		if (*(input+1)=='(') {
			bool v=true;
			for (;*input&&*input!=')';input++)
				if (!is_whitespace(*input))
					v=false;
			if (v)
				return VOID;
			if (*input)
				return QUOTE;
			return ERROR;
		} else
			return SYMBOL;
	}
	int periods=0;
	if (*input=='-')
		input++;
	for (char *c=input;!is_whitespace(*c);c++) {
		if (*c!='.'&&*c<'0'||*c>'9')
			return VARIABLE;
		if (*c=='.')
			periods++;
	}
	switch (periods) {
		case 0: return INT;
		case 1: return FLOAT;
		default: return ERROR;
	}
}
var_t *reference(var_t *term,var_t **env)
{
	//printf("REFERENCE\n");
	//printf("ENV: "); debug_display(*env); terpri();
	if (!*env) {
		*env=&NIL;
		return &NIL;
	}
	if (term->type==VARIABLE)
		term->type=SYMBOL;
	for (var_t *e=*env;e->type==CELL&&cdr(e);e=cdr(e))
		if (eq(term,car(car(e)))==&T)
			return cdr(car(e));
	return &NIL;
}
var_t *eval(var_t *form,var_t **env);
var_t *subst(var_t *list,var_t **env);
var_t *copy(var_t *var)
{
	////printf("COPY\n");
	if (var->type==VOID||var->type==SPECIAL)
		return var;
	var_t *c=NEW(var_t);
	if (var->type==CELL||var->type==QUOTE||var->type==FUNCTION)
		c=cons(copy(car(var)),copy(cdr(var)));
	else if (var->type==SYMBOL||var->type==VARIABLE) {
		c->data.s=malloc(strlen(var->data.s));
		strcpy(c->data.s,var->data.s);
	} else
		c->data=var->data;
	c->type=var->type;
	return c;
}
void destroy(var_t *var)
{
	if (!var||var->type==VOID||var->type==SPECIAL||var->type==FUNCTION)
		return;
	/* Temporary fix */
	if (var->type==INT)
		return;
	/* Temporary fix */
	//printf("DESTROY "); debug_display(var); terpri();
	if (var->type==CELL||var->type==QUOTE||var->type==FUNCTION) {
		destroy(car(var));
		destroy(cdr(var));
		free(var->data.l);
	} else if (var->type==SYMBOL||var->type==VARIABLE)
		free(var->data.s);
	free(var);
}
var_t *read(char *str);
var_t *apply(var_t *function,var_t *args,var_t **env)
{
	//printf("APPLY "); debug_display(function); //printf(" TO "); debug_display(args); terpri();
	//printf("ENV: "); debug_display(*env); terpri();
	ASSERTM(function!=&NIL,"\nFatal error: NIL is not a function\n\n");
	ASSERTM(args->type==CELL||args->type==VOID,"\nFatal error: Arguments not formatted as list\n\n");
	if (function==&PROGN) {
		var_t *v=args;
		for (;v->type==CELL&&cdr(v)!=&NIL;v=cdr(v))
			eval(car(v),env);
		return eval(car(v),env);
	}
	if (function==&IF) {
		if (eval(car(args),env)!=&NIL)
			return eval(car(cdr(args)),env);
		return eval(car(cdr(cdr(args))),env);
	}
	if (function==&AND||function==&OR) {
		for (var_t *v=args;v->type==CELL&&cdr(v);v=cdr(v))
			if ((function==&AND)^(eval(car(v),env)!=&NIL))
				return function==&OR?&T:&NIL;
		return function==&AND?&T:&NIL;
	}
	args=subst(args,env);
	if (function==&CAR)
		return car(car(args));
	if (function==&CDR)
		return cdr(car(args));
	if (function==&CONS)
		return cons(car(args),car(cdr(args)));
	if (function==&DISPLAY)
		return display(car(args));
	if (function==&EQ)
		return eq(car(args),car(cdr(args)));
	if (function==&ATOM)
		return atom(car(args));
	if (function==&DEFINE) {
		ASSERTM(car(args)->type==SYMBOL,"\nFatal error: Non-symbols can not be defined\n\n");
		*env=cons(cons(car(args),car(cdr(args))),*env);
		car(args)->type=SYMBOL;
		//printf("NEW ENV: "); debug_display(*env); terpri();
		return car(args);
	}
	if (function==&TERPRI)
		return terpri();
	if (function==&EVAL) {
		car(args)->type=CELL;
		return eval(car(args),env);
	}
	if (function==&READ) {
		ASSERTM(car(args)->type==INT,"\nFatal error: Read requires size argument\n\n");
		char *s=malloc(car(args)->data.i+2);
		fgets(s,car(args)->data.i+1,stdin);
		var_t *v=read(s);
		free(s);
		return v;
	}
	if (function==&EXIT)
		exit(0);
	if (function==&ADD)
		return arith(car(args),car(cdr(args)),'+');
	if (function==&SUB)
		return arith(car(args),car(cdr(args)),'-');
	if (function==&MUL)
		return arith(car(args),car(cdr(args)),'*');
	if (function==&DIV)
		return arith(car(args),car(cdr(args)),'/');
	if (function==&GREATER)
		return car(args)->data.i>car(cdr(args))->data.i?&T:&NIL;
	if (function==&LESS)
		return car(args)->data.i<car(cdr(args))->data.i?&T:&NIL;
	if (function==&RANDOM) {
		ASSERTM(car(args)->type==INT,"\nFatal error: Non-integer argument to RANDOM\n\n");
		return new_ivar(rand()%car(args)->data.i);
	}
	ASSERTM(function->type==FUNCTION,"\nFatal error: Non-functions can not be applied\n\n");
	//printf("COPY "); debug_display(function); terpri();
	var_t *func=copy(function);
	var_t *lex=*env;
	for (var_t *v=car(func);v->type==CELL&&cdr(v);v=cdr(v)) {
		//printf("BINDING "); debug_display(car(v)); //printf(" TO "); debug_display(car(args)); terpri();
		lex=cons(cons(car(v),car(args)),lex);
		car(v)->type=SYMBOL;
		args=cdr(args);
	}
	ASSERTM(args->type==VOID,"\nFatal error: Excess arguments in function application\n\n");
	//printf("LEXICAL ENV: "); debug_display(lex); terpri();
	var_t *result=eval(car(cdr(func)),&lex);
	//printf("DESTROY "); debug_display(func); terpri();
	destroy(car(func));
	destroy(cdr(func));
	return result;
}
var_t *to_var(char *str)
{
	//printf("TO_VAR\n");
	if (!strcasecmp("CAR",str))
		return &CAR;
	if (!strcasecmp("CDR",str))
		return &CDR;
	if (!strcasecmp("CONS",str))
		return &CONS;
	if (!strcasecmp("DISPLAY",str))
		return &DISPLAY;
	if (!strcasecmp("EQ",str))
		return &EQ;
	if (!strcasecmp("ATOM",str))
		return &ATOM;
	if (!strcasecmp("DEFINE",str))
		return &DEFINE;
	if (!strcasecmp("PROGN",str))
		return &PROGN;
	if (!strcasecmp("LAMBDA",str))
		return &LAMBDA;
	if (!strcasecmp("TERPRI",str))
		return &TERPRI;
	if (!strcasecmp("IF",str))
		return &IF;
	if (!strcasecmp("EVAL",str))
		return &EVAL;
	if (!strcasecmp("READ",str))
		return &READ;
	if (!strcasecmp("EXIT",str))
		return &EXIT;
	if (!strcmp("+",str))
		return &ADD;
	if (!strcmp("-",str))
		return &SUB;
	if (!strcmp("*",str))
		return &MUL;
	if (!strcmp("/",str))
		return &DIV;
	if (!strcmp(">",str))
		return &GREATER;
	if (!strcmp("<",str))
		return &LESS;
	if (!strcasecmp("AND",str))
		return &AND;
	if (!strcasecmp("OR",str))
		return &OR;
	if (!strcasecmp("RANDOM",str))
		return &RANDOM;
	int i,q;
	float f;
	char *s;
	var_t *v;
	switch (infer_type(str)) {
		case VOID: return &NIL;
		case INT: sscanf(str,"%i",&i);
			  return new_ivar(i);
		case FLOAT: sscanf(str,"%f",&f);
			  return new_fvar(f);
		case CHAR: return new_cvar(*++str);
		case VARIABLE:
		case SYMBOL: q=*str=='\'';
			     s=malloc(strlen(str)-q);
			     strcpy(s,str+q);
			     var_t *sym=new_svar(s);
			     if (!q)
				     sym->type=VARIABLE;
  			     return sym;
		case QUOTE: v=read(str+1);
			    v->type=QUOTE;
			    return v;
		case CELL: return read(str);
		case FUNCTION: v=read(str);
			       v=cons(car(cdr(v)),cdr(cdr(v)));
			       v->type=FUNCTION;
			       return v;
	}
}
var_t *read(char *str)
{
	//printf("READ\n");
	datatype t=infer_type(str);
	if (t!=CELL&&t!=QUOTE&&t!=FUNCTION)
		return to_var(str);
	str+=*str=='\'';
	var_t *end=cons(NULL,&NIL),*start=end;
	int parens=0;
	char *token=malloc(strlen(str)),*marker=str;
	for (char *c=str;*c;c++) {
		if (parens==1&&*c=='.'&&*(c+1)==' ') {
			marker=c+2;
			for (;*c!=')';c++);
			strncpy(token,marker,c-marker);
			token[c-marker]='\0';
			end->data.l->cdr=to_var(token);
			//printf("|%s| ",token); debug_display(cdr(end)); terpri();
			break;
		}
		if (*c=='(')
			parens++;
		else if (*c==')') {
			parens--;
		}
		if (parens==1&&is_whitespace(*c)||parens==0) {
			if (is_whitespace(*(c-1))) {
				marker=c;
				continue;
			}
			strncpy(token,marker+1,c-marker-1);
			token[c-marker-1]='\0';
			if (car(start)) {
				end->data.l->cdr=cons(&NIL,&NIL);
				end=cdr(end);
				end->data.l->car=to_var(token);
			} else {
				start->data.l->car=to_var(token);
			}
			//printf("|%s| ",token); debug_display(car(end)); terpri();
			marker=c;
			if (parens==0)
				break;
		}
	}
	end=&NIL;
	free(token);
	start->type=t;
	//printf("READ: "); debug_display(start); terpri();
	return start;
}
var_t *subst(var_t *list,var_t **env)
{
	//printf("SUBST "); debug_display(list); terpri();
	//printf("ENV: "); debug_display(*env); terpri();
	if (list->type==VOID)
		return &NIL;
	if (list->type!=CELL&&list->type!=QUOTE) {
		if (list->type==VARIABLE)
			return reference(list,env);
		return list;
	}
	if (car(list)->type==CELL||car(list)->type==QUOTE)
		return cons(eval(car(list),env),subst(cdr(list),env));
	if (car(list)->type==VARIABLE)
		return cons(reference(car(list),env),subst(cdr(list),env));
	return cons(car(list),subst(cdr(list),env));
}
var_t *eval(var_t *form,var_t **env)
{
	//printf("EVAL "); debug_display(form); terpri();
	if (!*env)
		*env=&NIL;
	//printf("ENV: "); debug_display(*env); terpri();
	if (form->type==VARIABLE)
		return reference(form,env);
	if (form->type==QUOTE) {
		var_t *v=subst(form,env);
		v->type=QUOTE;
		return v;
	}
	if (form->type==CELL) {
		if (car(form)->type==VARIABLE)
			form->data.l->car=reference(car(form),env);
		if (car(form)->type==FUNCTION||car(form)->type==SPECIAL)
			return apply(car(form),cdr(form),env);
		else
			return subst(form,env);
	}
	return form;
}
#endif
