#include "types.h"
#include "vars.h"
#include "cfunc.c"
#ifndef DEBUG_H
#define DEBUG_H
static char *type_strings[] = {
	"ERROR", "SPECIAL", "VOID",
	"INT", "FLOAT", "CHAR",
	"VARIABLE", "SYMBOL",
	"CELL", "QUOTE",
	"FUNCTION"
};
var_t *debug_display(var_t *var)
{
	if (var->type!=CELL&&var->type!=QUOTE) {
		display(var);
		printf("[%s]",type_strings[var->type]);
		return &NIL;
	}
	datatype type=var->type;
	putchar('(');
	for (;var->type==CELL||var->type==QUOTE&&var->data.l->cdr;var=var->data.l->cdr) {
		debug_display(var->data.l->car);
		putchar(' ');
	}
	if (var&&var->type!=VOID) {
		printf(". ");
		debug_display(var);
		putchar(' ');
	}
	printf("\e[D)[%s]",type_strings[type]);
	return &NIL;
}
#endif
