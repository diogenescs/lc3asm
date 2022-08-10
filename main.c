#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "src/tokenizer.h"
#include "src/instruction.h"

int main (int argc, char **argv) {
	
	if(argc != 2) {
		printf("lasm (file) ... \n");
		return 1;
	}
	
	printf("Reading file %s\n", argv[1]);

	printf("*******************************\n");

	source_t *source = create_source(argv[1]);

	printf("Source:\n\n%s\n\n", source->buffer);

	printf("*******************************\n");
	
	if(!source){
		printf("Invalid file\n");
		return 1;
	}


	while (1) {
		token_t *t = get_next_token(source);
		if(t->type == _EOF) {
			printf("End of file\n");
			free(t);
			break;
		}
		printf("(%lu:%lu) - Type = %d, Value = %d \n", t->start, t->len, t->type, t->value);
		free(t);
	}

	free(source);
	

}
