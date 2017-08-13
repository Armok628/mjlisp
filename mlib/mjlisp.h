// Lisp high-level functions

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
			||!strcasecmp("PROGN",input)
			||!strcasecmp("LAMBDA",input)
			||!strcasecmp("ADD",input))
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
var_t *reference(var_t *term,var_t **env)
{
	if (!*env) {
		*env=NIL;
		return NIL;
	}
	if (term->type==VARIABLE)
		term->type=SYMBOL;
	for (;(*env)->type==CELL&&cdr(*env);*env=cdr(*env))
		if (eq(term,car(car(*env)))==T)
			return cdr(car(*env));
	return NIL;
}
var_t *eval(var_t *form,var_t **env);
var_t *subst(var_t *list,var_t **env);
#include "debug.h" //////////////////////////////
var_t *copy(var_t *var)
{
	if (var->type==VOID||var->type==SPECIAL)
		return var;
	var_t *c=NEW(var_t);
	if (var->type==CELL||var->type==QUOTE||var->type==FUNCTION) {
		c=cons(copy(car(var)),copy(cdr(var)));
		c->type=var->type;
		return c;
	}
	c->type=var->type;
	if (var->type==SYMBOL||var->type==VARIABLE) {
		c->data.s=malloc(strlen(var->data.s));
		strcpy(c->data.s,var->data.s);
	} else
		c->data=var->data;
	return c;
}
void destroy(var_t *var)
{
	if (!var)
		return;
	if (var->type==VOID||var->type==SPECIAL)
		return;
	if (var->type==CELL||var->type==QUOTE||var->type==FUNCTION) {
		destroy(car(var));
		destroy(cdr(var));
		free(var->data.l);
	} else if (var->type==SYMBOL||var->type==VARIABLE)
		free(var->data.s);
	free(var);
}
var_t *apply(var_t *function,var_t *args,var_t **env)
{
	printf("APPLY "); debug_display(function); printf(" TO "); debug_display(args); terpri();
	printf("ENVIRONMENT: "); debug_display(ENV); terpri();
	static var_t *func;
	assert(args->type==CELL||args->type==VOID);
	// To-do: Convert to switch case
	if (function==PROGN) {
		var_t *v=args;
		for (;v->type==CELL&&cdr(v)!=NIL;v=cdr(v))
			eval(subst(car(v),env),env);
		return eval(subst(car(v),env),env);
	} else
		args=subst(args,env);
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
		assert(car(args)->type==SYMBOL);
		*env=cons(cons(car(args),car(cdr(args))),*env);
		car(args)->type=SYMBOL;
		printf("NEW ENV: "); debug_display(ENV); terpri();
		return car(args);
	}
	if (function==ADD)
		return add(car(args),car(cdr(args)));
	///////////////////////////
	assert(function->type==FUNCTION);
	destroy(func);
	func=copy(function);
	var_t *lex=*env;
	printf("ARGS: "); debug_display(args); terpri();
	for (var_t *v=car(func);v->type==CELL&&cdr(v);v=cdr(v)) {
		printf("BINDING "); debug_display(car(v)); printf(" TO "); debug_display(car(args)); terpri();
		lex=cons(cons(car(v),car(args)),lex);
		car(v)->type=SYMBOL;
		args=cdr(args);
	}
	assert(args->type==VOID);
	printf("LEXICAL ENV: "); debug_display(lex); terpri();
	return eval(car(cdr(func)),&lex);
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
	if (!strcasecmp("PROGN",str))
		return PROGN;
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
		case FUNCTION: v=read(str);
			       v=cons(car(cdr(v)),cdr(cdr(v)));
			       v->type=FUNCTION;
			       return v;
	}
}
var_t *read(char *str)
{
	// to-do: Change to left to right
	
	datatype t=infer_type(str);
	if (t!=CELL&&t!=QUOTE&&t!=FUNCTION)
		return to_var(str);
	var_t *end=cons(NULL,NIL),*start=end;
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
		if (parens==1&&*c==' '||parens==0) {
			strncpy(token,marker+1,c-marker-1);
			token[c-marker-1]='\0';
			if (car(start)) {
				end->data.l->cdr=cons(NIL,NIL);
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
	end=NIL;
	free(token);
	start->type=t;
	debug_display(start);
	return start;
}
var_t *subst(var_t *list,var_t **env)
{
	printf("SUBST "); debug_display(list); terpri();
	if (list->type==VOID)
		return NIL;
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
	printf("EVAL "); debug_display(form); terpri();
	if (!*env)
		*env=NIL;
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
		//form->data.l->cdr=subst(cdr(form),env);
		if (car(form)->type==FUNCTION||car(form)->type==SPECIAL)
			return apply(car(form),cdr(form),env);
		else
			return subst(form,env);
	}
	return form;
}
#endif
