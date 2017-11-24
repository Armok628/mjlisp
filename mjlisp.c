// Lisp high-level functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h" // Datatypes
#include "vars.h" // Variable tools
#include "cfunc.c" // Core functions

#ifndef MJLISP_H
#define MJLISP_H
#include "debug.h"
// Used to retrieve lval:
#define CAR(x) (x->data.l->car)
#define CDR(x) (x->data.l->cdr)
bool is_whitespace(char c)
{
	return c==' '||c=='\n'||c=='\t'||c=='\0';
}
datatype infer_type(char *input) // Decides type value for given input
{
	//printf("INFER_TYPE\n");
#define INPUT_MATCH(x) !strcasecmp(#x,input)
#define OP_MATCH(x) (input[0]==x&&input[1]=='\0')
	// Check for special forms
	if (!strcasecmp("NIL",input))
		return VOID;
	if (!strcasecmp("CAR",input)
			||INPUT_MATCH(CDR)
			||INPUT_MATCH(CONS)
			||INPUT_MATCH(DISPLAY)
			||INPUT_MATCH(EQ)
			||INPUT_MATCH(ATOM)
			||INPUT_MATCH(DEFINE)
			||INPUT_MATCH(PROGN)
			||INPUT_MATCH(LAMBDA)
			||INPUT_MATCH(TERPRI)
			||INPUT_MATCH(IF)
			||INPUT_MATCH(EVAL)
			||INPUT_MATCH(READ)
			||INPUT_MATCH(EXIT)
			||INPUT_MATCH(AND)
			||INPUT_MATCH(OR)
			||INPUT_MATCH(RANDOM)
			||INPUT_MATCH(LOAD)
			||INPUT_MATCH(SET)
			||OP_MATCH('+')
			||OP_MATCH('-')
			||OP_MATCH('*')
			||OP_MATCH('/')
			||OP_MATCH('%')
			||OP_MATCH('>')
			||OP_MATCH('<')
			||OP_MATCH('^'))
		return SPECIAL;
	// Check for anything represented by a list
	if (*input=='(') {
		// Check for nil representation by ()
		if (*(input+1)==')')
			return VOID;
		// Check for function definitions
		char *lambda=calloc(7,1);
		strncpy(lambda,input+1,6);
		if (!strcasecmp("LAMBDA",lambda))
			return FUNCTION; // This tells to_var to behave differently
		else
			free(lambda);
		// Go to the end of the input
		for (;*input&&*input!=')';input++);
		if (*input)
			return CELL;
		return ERROR; // (If not properly terminated)
	}
	// Check for characters
	if (*input=='\\') {
		ASSERTM(strlen(input)<3,"infer_type(): Escaped non-character");
		return CHAR;
	}
	// Check for "quotations"
	if (*input=='\'') {
		// Check for quoted lists
		if (*(input+1)=='(') {
			bool v=true;
			for (;*input&&*input!=')';input++)
				if (!is_whitespace(*input))
					v=false;
			if (v) // Check for nil representation by '()
				return VOID;
			if (*input)
				return QUOTE;
			return ERROR;
		} else // If it wasn't a list, it's a symbol
			return SYMBOL;
	}
	// Check for numbers
	int periods=0;
	if (*input=='-') // Skip the negative sign if there is one.
		input++; // This is dealt with by to_var()
	for (char *c=input;!is_whitespace(*c);c++) {
		// Check for allowed characters
		if (*c!='.'&&*c<'0'||*c>'9')
			return VARIABLE; // This lets variables begin with a number
		// Count the periods
		if (*c=='.')
			periods++;
	}
	// Act with respect to the number of periods
	switch (periods) {
		case 0: return INT;
		case 1: return FLOAT;
		default: return ERROR; // (Numbers should never have more than one period)
	}
}
var_t *reference(var_t *term,var_t **env) // (cdr (assoc term env))
{
	//printf("REFERENCE\n");
	//printf("ENV: "); debug_display(*env); terpri();
	if (!*env) {
		*env=&NIL;
		return &NIL;
	}
	if (term->type==VARIABLE) // (Realistically, term should never be a symbol)
		term->type=SYMBOL; // Definitions are made by symbol, not variable, and types must match.
	for (var_t *e=*env;e->type==CELL&&cdr(e);e=cdr(e)) // assoc
		if (eq(term,car(car(e)))==&T)
			return cdr(car(e)); // cdr
	return &NIL;
}
var_t *eval(var_t *form,var_t **env);
var_t *subst(var_t *list,var_t **env);
var_t *copy(var_t *var) // Recursively copy a variable and its contents (if a list)
{
	////printf("COPY\n");
	if (var->type==VOID||var->type==SPECIAL)
		return var;
	var_t *c=NEW(var_t);
	if (var->type==CELL||var->type==QUOTE||var->type==FUNCTION)
		c=cons(copy(car(var)),copy(cdr(var)));
	else if (var->type==SYMBOL||var->type==VARIABLE) {
		c->data.s=calloc(strlen(var->data.s)+1,1);
		strcpy(c->data.s,var->data.s);
	} else
		c->data=var->data;
	c->type=var->type;
	return c;
}
void destroy(var_t *var) // Recursively free a variable's memory
{
	if (!var||var->type==VOID||var->type==SPECIAL||var->type==FUNCTION)
		return;
	/* Temporary fix */
	if (var->type==INT||var->type==CHAR)
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
var_t *load(char *filename,var_t **env);
var_t *apply(var_t *function,var_t *args,var_t **env) // Apply a function to arguments in an environment
{
	//printf("APPLY "); debug_display(function); //printf(" TO "); debug_display(args); terpri();
	//printf("ENV: "); debug_display(*env); terpri();
	ASSERTM(function!=&NIL,"apply(): NIL is not a function");
	ASSERTM(args->type==CELL||args->type==QUOTE||args->type==VOID
			,"apply(): Arguments not formatted as list");
	// Special forms are defined here (typically using functions in cfunc.c)
	if (function==&PROGN) {
		var_t *v=args;
		for (;v->type==CELL&&cdr(v)!=&NIL;v=cdr(v))
			eval(car(v),env);
		// Returns evaluation of last item in args
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
		ASSERTM(car(args)->type==SYMBOL,"DEFINE: Non-symbols can not be defined");
		*env=cons(cons(car(args),car(cdr(args))),*env);
		car(args)->type=SYMBOL;
		//printf("NEW ENV: "); debug_display(*env); terpri();
		return car(args);
	}
	if (function==&TERPRI)
		return terpri();
	if (function==&EVAL) {
		if (car(args)->type==QUOTE)
			car(args)->type=CELL;
		return eval(car(args),env);
	}
	if (function==&READ) {
		ASSERTM(car(args)->type==INT,"READ: Size argument required");
		char *s=calloc(car(args)->data.i+2,1);
		fgets(s,car(args)->data.i+1,stdin);
		var_t *v=read(s);
		free(s);
		return v;
	}
	if (function==&LOAD) {
		ASSERTM(car(args)->type==SYMBOL,"LOAD: Non-symbol argument");
		return load(car(args)->data.s,env);
	}
	if (function==&SET) {
		ASSERTM(car(args)!=&NIL,"SET: Undefined argument");
		CAR(args)->type=car(cdr(args))->type;
		CAR(args)->data=car(cdr(args))->data;
		return car(args);
	}
	if (function==&EXIT)
		exit(0);
#define ARITH_OP(n,c) if (function==&n) return arith(car(args),car(cdr(args)),c)
	ARITH_OP(ADD,'+');
	ARITH_OP(SUB,'-');
	ARITH_OP(MUL,'*');
	ARITH_OP(DIV,'/');
	ARITH_OP(MOD,'%');
	ARITH_OP(EXPT,'^');
	if (function==&GREATER)
		return car(args)->data.i>car(cdr(args))->data.i?&T:&NIL;
	if (function==&LESS)
		return car(args)->data.i<car(cdr(args))->data.i?&T:&NIL;
	if (function==&RANDOM) {
		ASSERTM(car(args)->type==INT,"RANDOM: Non-integer argument");
		return new_ivar(rand()%car(args)->data.i);
	}
	ASSERTM(function->type==FUNCTION,"apply(): Non-functions can not be applied");
	//printf("COPY "); debug_display(function); terpri();
	var_t *func=copy(function);
	var_t *lex=*env;
	for (var_t *v=car(func);v->type==CELL&&cdr(v);v=cdr(v)) { // Bind arguments
		//printf("BINDING "); debug_display(car(v)); //printf(" TO "); debug_display(car(args)); terpri();
		lex=cons(cons(car(v),car(args)),lex);
		car(v)->type=SYMBOL;
		args=cdr(args);
	}
	ASSERTM(args->type==VOID,"apply(): Excess arguments in function application");
	lex=cons(cons(&SELF,function),lex); // Bind function to local variable for anonymous recursion
	//printf("LEXICAL ENV: "); debug_display(lex); terpri();
	var_t *result=eval(car(cdr(func)),&lex);
	//printf("DESTROY "); debug_display(func); terpri();
	destroy(car(func));
	destroy(cdr(func));
	return result;
}
var_t *to_var(char *str) // Converts input string (see read) into a real variable
{
	//printf("TO_VAR\n");
#define SPECIAL_FORM(s) if (!strcasecmp(#s,str)) return &s
#define OPERATOR(s,c) if (str[0]==c&&str[1]=='\0') return &s
	SPECIAL_FORM(CAR);
	SPECIAL_FORM(CDR);
	SPECIAL_FORM(CONS);
	SPECIAL_FORM(DISPLAY);
	SPECIAL_FORM(EQ);
	SPECIAL_FORM(ATOM);
	SPECIAL_FORM(DEFINE);
	SPECIAL_FORM(PROGN);
	SPECIAL_FORM(LAMBDA);
	SPECIAL_FORM(TERPRI);
	SPECIAL_FORM(IF);
	SPECIAL_FORM(EVAL);
	SPECIAL_FORM(READ);
	SPECIAL_FORM(EXIT);
	SPECIAL_FORM(AND);
	SPECIAL_FORM(OR);
	SPECIAL_FORM(RANDOM);
	SPECIAL_FORM(LOAD);
	SPECIAL_FORM(SET);
	OPERATOR(ADD,'+');
	OPERATOR(SUB,'-');
	OPERATOR(MUL,'*');
	OPERATOR(DIV,'/');
	OPERATOR(GREATER,'>');
	OPERATOR(LESS,'<');
	OPERATOR(EXPT,'^');
	OPERATOR(MOD,'%');
	long i;
	int q;
	double f;
	char *s;
	var_t *v;
	switch (infer_type(str)) {
		case VOID: return &NIL;
		case INT: sscanf(str,"%li",&i);
			  return new_ivar(i);
		case FLOAT: sscanf(str,"%lf",&f);
			  return new_fvar(f);
		case CHAR: return new_cvar(str[1]);
		case VARIABLE:
		case SYMBOL: q=*str=='\'';
			     s=calloc(strlen(str)-q+1,1);
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
var_t *read(char *str) // Tokenizes input into nested lists. Mutual recursion with to_var
{
	//printf("READ\n");
	datatype t=infer_type(str);
	if (t!=CELL&&t!=QUOTE&&t!=FUNCTION)
		return to_var(str);
	str+=*str=='\'';
	var_t *end=cons(NULL,&NIL),*start=end;
	int parens=0;
	char *token=calloc(strlen(str)+1,1),*marker=str;
	for (char *c=str;*c;c++) {
		// Character escape
		if (parens==1&&*c=='\\') {
			strncpy(token,c,2);
			token[2]='\0';
			c+=2;
			marker=c;
			goto ADD_TOKEN;
		}
		// Dotted list interpretation
		if (parens==1&&*c=='.'&&*(c+1)==' ') {
			marker=c+2;
			for (;*c!=')';c++); // Skip to the end of the form
			strncpy(token,marker,c-marker);
			token[c-marker]='\0';
			CDR(end)=to_var(token);
			//printf("|%s| ",token); debug_display(cdr(end)); terpri();
			break;
		}
		// Ignore things between parentheses until later
		if (*c=='(')
			parens++;
		else if (*c==')') {
			parens--;
		}
		// General tokens/lists
		if (parens==1&&is_whitespace(*c)||parens==0) {
			if (is_whitespace(*(c-1))) {
				marker=c;
				continue;
			}
			strncpy(token,marker+1,c-marker-1);
			token[c-marker-1]='\0';
			marker=c;
ADD_TOKEN:		if (car(start)) {
				CDR(end)=cons(&NIL,&NIL);
				end=cdr(end);
				CAR(end)=to_var(token);
			} else {
				CAR(start)=to_var(token);
			}
			//printf("|%s| ",token); debug_display(car(end)); terpri();
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
var_t *subst(var_t *list,var_t **env) // Replaces items in list with evaluations (via eval) or bindings (via reference)
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
var_t *eval(var_t *form,var_t **env) // Evaluates a form. Mutual recursion with subst
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
			CAR(form)=reference(car(form),env);
		if (car(form)->type==FUNCTION||car(form)->type==SPECIAL)
			return apply(car(form),cdr(form),env);
		else
			return subst(form,env);
	}
	return form;
}
var_t *load(char *filename,var_t **env)
{
	FILE *input_file=fopen(filename,"r");
	if (!input_file) {
		printf("Error: File %s can not be opened.\n",filename);
		exit(1);
	}
	fseek(input_file,0,SEEK_END);
	long length=ftell(input_file);
	rewind(input_file);
	char *input=calloc(length+1,1);
	fread(input,1,length,input_file);
	var_t *result=eval(read(input),env);
	free(input);
	return result;
}
#endif
