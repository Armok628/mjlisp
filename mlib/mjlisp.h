// To-do:
// Split this file into multiple files
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
void print_var(var_t *v)
{
	switch (v->type) {
		case SPECIAL: printf("%s",v->data.s);
			      return;
		case VOID: printf("NIL");
			   return;
		case INT: printf("%i",v->data.i);
			  return;
		case FLOAT: printf("%f",v->data.f);
			    return;
		case CHAR: printf("\\%c",v->data.c);
			   return;
		case SYMBOL: printf("%s",v->data.s);
			     return;
		case CELL: break;
		case FUNCTION: printf("#FUNCTION%i",v);
			       return;
	}
	putchar('(');
	for (;v->type==CELL&&cdr(v);v=cdr(v)) {
		print_var(car(v));
		putchar(' ');
	}
	if (v&&v->type!=VOID) {
		printf(". ");
		print_var(v);
		putchar(' ');
	}
	printf("\e[D)");
}
datatype infer_type(char *input)
{
	if (!strcasecmp("NIL",input))
		return VOID;
	if (!strcasecmp("CAR",input)
			||!strcasecmp("CDR",input)
			||!strcasecmp("CONS",input)
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
				return CELL;
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
var_t *apply_function(var_t *function,var_t *args)
{
	assert(args->type==CELL||args->type==VOID);
	if (function==CAR)
		return car(car(args));
	if (function==CDR)
		return cdr(car(args));
	if (function==CONS)
		return cons(car(args),car(cdr(args)));
	if (function==ADD)
		return add(car(args),car(cdr(args)));
	assert(function->type==FUNCTION);
	return NULL; // To-do: Create lexical binding, then eval
}
var_t *eval(char *str);
var_t *to_var(char *str)
{
	if (!strcasecmp("CAR",str))
		return CAR;
	if (!strcasecmp("CDR",str))
		return CDR;
	if (!strcasecmp("CONS",str))
		return CONS;
	if (!strcasecmp("LAMBDA",str))
		return LAMBDA;
	if (!strcasecmp("ADD",str))
		return ADD;
	int i,q;
	float f;
	char *s;
	switch (infer_type(str)) {
		//case VARIABLE: /*To-do*/ break;
		case VOID: return NIL;
		case INT: sscanf(str,"%i",&i);
			  return new_ivar(i);
		case FLOAT: sscanf(str,"%f",&f);
			  return new_fvar(f);
		case CHAR: return new_cvar(*++str);
		/**/ case VARIABLE: /**/
		case SYMBOL: q=*str=='\'';
			     s=malloc(strlen(str)-q);
			     strcpy(s,str+q);
			     return new_svar(s);
		case CELL: return eval(str);
	}
}
var_t *eval(char *str)
{
	// To-do: Refactor into multiple functions
	// read-from-string and eval
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
	if (*start=='\'')
		return list;
	else
		return apply_function(car(list),cdr(list));
}
#endif
