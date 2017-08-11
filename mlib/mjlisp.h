// To-do:
// Finish adding core functions
// Implement a dictionary system (assoc list) and definitions
// Implement lexical bindings (Idea: cons assoc list to global environment)
// Implement lambda functions (using lexical bindings)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h" // Datatypes
#include "vars.h" // Variable tools
#include "cfunc.h" // Core functions

#ifndef MJLISP_H
#define MJLISP_H
datatype infer_type(char *input)
{
	if (!strcasecmp("NIL",input))
		return VOID;
	if (!strcasecmp("CAR",input)
			||!strcasecmp("CDR",input)
			||!strcasecmp("CONS",input)
			||!strcasecmp("DISPLAY",input)
			||!strcasecmp("EQ",input)
			||!strcasecmp("ATOM",input)
			||!strcasecmp("DEFINE",input)
			||!strcasecmp("LAMBDA",input)
			||!strcasecmp("ADD",input))
		return SPECIAL;
	if (*input=='(') {
		char *lambda=malloc(7);
		strncpy(lambda,input+1,6);
		if (!strcasecmp("LAMBDA",lambda))
			return FUNCTION;
		else
			free(lambda);
		for (char *c=input;*c;c++)
			if (*c==')')
				return CELL;
		return ERROR;
	}
	if (*input=='\\') {
		assert(strlen(input)==2);
		return CHAR;
	} else if (*input=='\'') {
		if (*(input+1)=='(') {
			if (*(input+2)==')')
				return VOID;
			for (;*input;input++);
			if (*(input-1)==')')
				return QUOTE;
			else
				return ERROR;
		} else
			return SYMBOL;
	}
	int periods=0;
	for (char *c=input;*c;c++) {
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
var_t *reference(var_t *term,var_t *env)
{
	if (!env) {
		env=NIL;
		return NIL;
	}
	if (term->type==VARIABLE)
		term->type=SYMBOL;
	for (;env->type==CELL&&cdr(env);env=cdr(env))
		if (eq(term,car(car(env)))==T)
			return cdr(car(env));
	return NIL;
}
var_t *apply_function(var_t *function,var_t *args)
{
	assert(args->type==CELL||args->type==VOID);
	if (function==CAR)
		return car(car(args));
	if (function==CDR)
		return cdr(car(args));
	if (function==CONS)
		return cons(car(args),car(cdr(args)));
	if (function==DISPLAY)
		return display(car(args));
	if (function==EQ)
		return eq(car(args),car(cdr(args)));
	if (function==ATOM)
		return atom(car(args));
	if (function==DEFINE) {
		ENV=cons(cons(car(args),car(cdr(args))),ENV);
		car(car(ENV))->type=VARIABLE;
		car(args)->type=SYMBOL;
		return car(args);
	}
	if (function==LAMBDA) {
		var_t *fun=cons(car(args),cdr(args));
		fun->type=FUNCTION;
		return fun;
	}
	if (function==ADD)
		return add(car(args),car(cdr(args)));
	assert(function->type==FUNCTION);
	return NULL; // To-do: Create lexical binding, then read
}
var_t *read(char *str);
var_t *to_var(char *str)
{
	if (!strcasecmp("CAR",str))
		return CAR;
	if (!strcasecmp("CDR",str))
		return CDR;
	if (!strcasecmp("CONS",str))
		return CONS;
	if (!strcasecmp("DISPLAY",str))
		return DISPLAY;
	if (!strcasecmp("EQ",str))
		return EQ;
	if (!strcasecmp("ATOM",str))
		return ATOM;
	if (!strcasecmp("DEFINE",str))
		return DEFINE;
	if (!strcasecmp("LAMBDA",str))
		return LAMBDA;
	if (!strcasecmp("ADD",str))
		return ADD;
	int i,q;
	float f;
	char *s;
	var_t *v;
	switch (infer_type(str)) {
		case VOID: return NIL;
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
	}
}
var_t *read(char *str)
{
	datatype t=infer_type(str);
	if (t!=CELL&&t!=QUOTE)
		return to_var(str);
	var_t *list=NIL;
	int parens=0;
	char *start=str,*token=malloc(strlen(str));
	for (;*str;str++);
	str--;
	char *marker=str;
	for (char *c=str;c>=start;c--) {
		if (list==NIL&&parens==1&&*c==' '&&*(c-1)=='.') {
			marker--;
			strncpy(token,c+1,marker-c);
			token[marker-c]='\0';
			list=to_var(token);
			c-=2;
			marker=c;
			continue;
		}
		if (*c==')')
			parens++;
		else if (*c=='(') {
			parens--;
		}
		if (parens==1&&*c==' '||parens==0) {
			list=cons(malloc(sizeof(var_t)),list);
			marker--;
			strncpy(token,c+1,marker-c);
			token[marker-c]='\0';
			list->data.l->car=to_var(token);
			marker=c;
			if (parens==0)
				break;
		}
	}
	free(token);
	list->type=t;
	return list;
}
var_t *eval(var_t *form,var_t *env);
var_t *eval_all(var_t *list,var_t *env)
{
	if (list->type!=CELL||list->type!=QUOTE||list->type!=VOID)
		return list;
	if (!env)
		env=NIL;
	if (list->type==VOID)
		return NIL;
	return cons(eval(car(list),env),eval_all(cdr(list),env));
}
var_t *eval(var_t *form,var_t *env)
{
	if (!env)
		env=NIL;
	if (form->type==QUOTE) {
		var_t *v=eval_all(form,env);
		v->type=QUOTE;
		return v;
	}
	if (form->type==CELL)
		if (car(form)->type==FUNCTION||car(form)->type==SPECIAL)
			return apply_function(car(form),eval_all(cdr(form),env));
		else
			return eval_all(form,env);
	return form;
}
#endif
