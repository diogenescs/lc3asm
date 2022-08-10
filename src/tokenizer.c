#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"
#include "instruction.h"

static token_t *create_token(token_type_t t);

static char peek(source_t *src);
static char get_char(source_t *src);

static char* copy_string(source_t *src, size_t start, size_t len);

static int is_reg(char *s, int len);
static int is_hex_number(char *s);

static token_t *match_directive(source_t *src);
static token_t *match_identifier(source_t *src);
static token_t *match_string(source_t *src);
static token_t *match_decimal(source_t *src);

static token_t *create_token(token_type_t t) {
	token_t *tk = (token_t *) malloc(sizeof(token_t));
	if(!tk) {
		fprintf(stderr, "Unexpected eror while allocating memory");
		exit(EXIT_FAILURE);
	}
	tk->type = t;
	tk->value = 0;
	tk->start = 0;
	tk->len = 0;
	return tk;
}

static char peek(source_t *src) {
	if(src->cursor == src->buffer_size - 1) {
		return 0;
	}
	return src->buffer[src->cursor + 1];
}

static char get_char(source_t *src) {
	if(src->cursor == src->buffer_size - 1) {
		return 0;
	}
	char c = src->buffer[++src->cursor]; 
	if(c == '\n') ++src->line;
	return c;
}

source_t *create_source(const char *filename){
	
	FILE *fp = fopen(filename, "rb");
	
	if(!fp) {
		perror("Failed to open source file");
		exit(EXIT_FAILURE);
	}
	
	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char *buffer = (char *) malloc(sizeof(char) * file_size + 1);
	
	if(!buffer) {
		fprintf(stderr, "Could not allocate memory for buffer");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	size_t bytes_read = fread(buffer, 1, file_size, fp);
	
	if(bytes_read != file_size) {
		perror("Error reading source file");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	buffer[file_size] = 0;
	
	fclose(fp);
	
	source_t *source = (source_t*) malloc(sizeof(source_t));
	
	if(!source) {
		free(buffer);
		fprintf(stderr, "Unexpected eror while allocating memory");
		exit(EXIT_FAILURE);	
	}
	
	source->buffer = buffer;
	source->buffer_size = file_size;
	source->cursor = -1;
	source->line = 1;
	
	return source;
}

void free_source(source_t *t) {
	if(t) {
		free(t->buffer);
		free(t);
	}
}

token_t *get_next_token(source_t *source) {
    token_t *t = NULL;
	while(1) {
		char c = peek(source);
		switch(c) {
			case '.':
				return match_directive(source);
			case '\n':
				t = create_token(EOL);
				get_char(source);
				t->start = source->cursor;
				t->len = 1;
				return t;
			case ',':
				t = create_token(COMMA);
				get_char(source);
				t->start = source->cursor;
				t->len = 1;
				return t;				
			case ';':
				get_char(source);
				while(peek(source) != '\n') {
					get_char(source);
				}
				break;
			case ':':
				t = create_token(COLON);
				get_char(source);
				t->start = source->cursor;
				t->len = 1;
				return t;
			case '"':
				return match_string(source);
			case ' ':
				get_char(source);
				continue;
			case 0:
				get_char(source);
				return create_token(_EOF);
				return NULL;
			default: {
				if(isdigit(c)) {
					return match_decimal(source);
				} else if (isalpha(c) || c == '_') {
					return match_identifier(source);
				} else {
					get_char(source);
					token_t *t = create_token(INVALID);
					t->start = source->cursor;
					t->len = 1;
					return t;
				}
			}
		}
	}
}

static char* copy_string(source_t *src, size_t start, size_t len) {
	char *buffer = (char*) malloc(sizeof(char) * (len + 1));
	memset(buffer, '\0', sizeof(buffer));
	strncpy(buffer, &src->buffer[start], len);
	return buffer;
}


static int is_reg(char *lexema, int len) {
	return len == 2 && (lexema[0] == 'r' || lexema[0] == 'R') 
		&& (lexema[1] >= '0' && lexema[1] <= '7'); 
}


static int is_hex_number(char *s) {
	
	if(!s || s[0] != 'X') return 0;

	int i = 1;

	while (s[i]) {
		if(!isxdigit(s[i]))	
			return 0;
		i++;
	}

	return 1;
}

static token_t *match_directive(source_t *src) {
    token_t *t = create_token(MACRO);
    get_char(src);
	t->start = src->cursor;
	t->len = 1;
	while(isalpha(peek(src))) {
		++t->len;
		get_char(src);
	}
	char *buffer = copy_string(src, src->cursor-(t->len-1), t->len);
	t->value = encode_directive(buffer);
	free(buffer);
	return t;
}

static token_t *match_decimal(source_t *src){
    token_t *t = create_token(DEC_LITERAL);
    get_char(src);
	t->start = src->cursor;
	t->len = 1;
	while(isdigit(peek(src))) {
		++t->len;
		get_char(src);
	}
	char *buffer = copy_string(src, src->cursor-(t->len-1), t->len);
	t->value = strtol(buffer, NULL, 10);
	free(buffer);
	return t;	
}

static token_t *match_identifier(source_t *src) {

	token_t *t;

	size_t 	start = 0, len = 1;

	get_char(src);
	start = src->cursor;
	while(isalnum(peek(src)) || peek(src) == '_') {
		++len;
		get_char(src);
	}

	char *buffer = copy_string(src, src->cursor-(len - 1), len);
	
	if(is_reg(buffer, len)) {
		t = create_token(REGISTER);
	} else if(is_hex_number(buffer)) {
		t = create_token(HEX_LITERAL);
		t->value = strtol(buffer + 1, NULL, 16);
	} else {
		uint16_t instr_code = encode_instruction(buffer);
		if(instr_code != INVALID_INSTRUCTION) {
			t = create_token(INSTRUCTION);
			t->value = instr_code;
		} else {
			t = create_token(LABEL);
		}
	}
	free(buffer);
	t->start = start;
	t->len = len;
	return t;
}

static token_t *match_string(source_t *src) {

    token_t *t = create_token(STR_LITERAL);
    
    char c = 0;
    get_char(src);
	
	t->start = src->cursor;
	t->len = 1;
	
	while(peek(src) != '"' || c == '\\') {
		++t->len;
		c = get_char(src);
		if(c == '\n' || c == EOF) {
			fprintf(stderr, "Unterminated string Error: %d\n", src->line - 1);
			exit(EXIT_FAILURE);	
		}
	}
	get_char(src);
	t->len++;
	return t;

}

