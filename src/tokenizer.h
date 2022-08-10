#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define MAX_LABEL_SIZE  0xF

typedef enum {
	INSTRUCTION 	= (1 << 0),
	REGISTER 		= (1 << 1),
	STR_LITERAL 	= (1 << 2),
	DEC_LITERAL 	= (1 << 3),
	HEX_LITERAL 	= (1 << 4),
	LABEL 			= (1 << 5),
	MACRO 			= (1 << 6),
	COMMA 			= (1 << 7),
	COLON 			= (1 << 8),
	EOL 			= (1 << 9),
	_EOF 			= (1 << 10),
	INVALID 		= (1 << 11)
} token_type_t;

typedef struct {
	token_type_t type;
	int value;
	size_t line;
	size_t start;
	size_t len;
} token_t; 


typedef struct {
	int line;
	char *buffer;
	size_t buffer_size;
	int cursor;
} source_t;


token_t *get_next_token(source_t *src);

typedef int(*predicate_fn_t)(char, char);

source_t *create_source(const char *filename);
void free_source(source_t *t);

#endif
