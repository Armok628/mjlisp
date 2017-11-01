// Lisp datatypes

#ifndef TYPES_H
#define TYPES_H
typedef enum {false,true} bool;
typedef enum {/**/ERROR,SPECIAL,VOID,/*Pseudo-types*/
	INT,FLOAT,CHAR,
	VARIABLE,SYMBOL,
	CELL,QUOTE,
	FUNCTION} datatype;
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
#endif
