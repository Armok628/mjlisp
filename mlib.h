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
#define NEW(X) malloc(sizeof(X))
#ifndef MLISP_H
#define MLISP_H

// Type definitions
typedef enum {/**/ERROR,VARIABLE,SPECIAL,VOID,/*Pseudo-types*/
	INT,FLOAT,CHAR,
	SYMBOL,CELL,FUNCTION} datatype;
struct cell_t;
typedef struct {
	union {
		int i;
		float f;
		char c;
		char *s;
		struct cell_t *l;
	} data;
	datatype type;
} var_t;
typedef struct cell_t {
	var_t *car;
	var_t *cdr;
} cell_t;
// Special variable declarations
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
// Variable creation
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
// Cell creation and access
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
// Arithmetic
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
// Other
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
void print_type_of(char *input)
{
	datatype type=infer_type(input);
	switch (type) {
		case SPECIAL: printf("SPECIAL\n");
			      break;
		case VOID: printf("VOID\n");
			   break;
		case VARIABLE: printf("VARIABLE\n");
			       break;
		case INT: printf("INT\n");
			  break;
		case FLOAT: printf("FLOAT\n");
			    break;
		case CHAR: printf("CHAR\n");
			   break;
		case SYMBOL: printf("SYMBOL\n");
			     break;
		case CELL: printf("CELL\n");
			   break;
		case FUNCTION: printf("FUNCTION\n");
			       break;
		default: printf("%i",type);
	}
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
	int i;
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
		case SYMBOL: s=malloc(strlen(str));
			     strcpy(s,str);
			     return new_svar(s);
		case CELL: return eval(str);
	}
}
var_t *apply_function(var_t *function,var_t *args)
{
	assert(args->type==CELL||args->type==VOID);
	if (function==CAR) {
		assert(cdr(args)==NIL);
		return car(car(args));
	}
	if (function==CDR) {
		assert(cdr(args)==NIL);
		return cdr(car(args));
	}
	if (function==CONS) {
		assert(cdr(cdr(args))==NIL);
		return cons(car(args),car(cdr(args)));
	}
	if (function==ADD) {
		assert(cdr(cdr(args))==NIL);
		return add(car(args),car(cdr(args)));
	}
	assert(function->type==FUNCTION);
	// Nothing after this line has been tested.
	int argc=0;
	for (var_t *v=car(cdr(function));v->type==CELL&&cdr(v);v=cdr(v))
		argc++;
	var_t **argv=malloc(argc*sizeof(var_t *));
	int i=0;
	var_t *vn=car(cdr(function));
	for (var_t *va=args;va->type==CELL&&cdr(va);va=cdr(va)) {
		assert(i<=argc);
		argv[i]=cons(vn,va);
		i++;
		vn=cdr(vn);
	}
	assert(i==argc);
	// What to do next?
}
var_t *eval(char *str)
{
	var_t *list=NIL;
	int parens=0;
	char *start=str,*token=malloc(64);
	for (;*str;str++);
	str--;
	char *marker=str;
	for (char *c=str;c>=start;c--) {
		if (*c==')')
			parens++;
		else if (*c=='(') {
			parens--;
		}
		if (parens==1&&*c==' '||parens==0) {
			list=cons(NEW(var_t),list);
			marker--;
			strncpy(token,c+1,marker-c);
			token[marker-c]='\0';
			marker=c;
			//printf("|%s| ",token);
			//print_type_of(token);
			list->data.l->car=to_var(token);
			if (parens==0)
				break;
		}
	}
	free(token);
	//printf("%c\n",*start);
	if (*start=='\'')
		return list;
	else
		return apply_function(car(list),cdr(list));
}
#endif
