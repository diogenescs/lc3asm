#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"

uint16_t encode_instruction(const char* instr_name) {
	if(!strcmp(instr_name, BR_INSTR)) {
		return BR;
	} else if(!strcmp(instr_name, ADD_INSTR)) {
		return ADD;
	} else if(!strcmp(instr_name, LD_INSTR)) {
		return LD;
	} else if(!strcmp(instr_name, ST_INSTR)) {
		return ST;
	} else if(!strcmp(instr_name, AND_INSTR)) {
		return AND;
	} else if(!strcmp(instr_name, LDR_INSTR)) {
		return LDR;
	} else if(!strcmp(instr_name, STR_INSTR)) {
		return STR;
	} else if(!strcmp(instr_name, RTI_INSTR)) {
		return RTI;
	} else if(!strcmp(instr_name, NOT_INSTR)) {
		return NOT;
	} else if(!strcmp(instr_name, LDI_INSTR)) {
		return LDI;
	} else if(!strcmp(instr_name, STI_INSTR)) {
		return STI;
	} else if(!strcmp(instr_name, RET_INSTR)) {
		return RET;
	} else if(!strcmp(instr_name, LEAD_INSTR)) {
		return LEAD;
	} else if(!strcmp(instr_name, TRAP_INSTR)) {
		return TRAP;
	} else if(!strcmp(instr_name, GETC_INSTR)) {
		return GETC;
	} else if(!strcmp(instr_name, OUT_INSTR)) {
		return OUT;
	} else if(!strcmp(instr_name, PUTS_INSTR)) {
		return PUTS;
	} else if(!strcmp(instr_name, IN_INSTR)) {
		return IN;
	} else if(!strcmp(instr_name, PUTSP_INSTR)) {
		return PUTSP;
	} else if(!strcmp(instr_name, HALT_INSTR)) {
		return HALT;
	} else {
		return INVALID_INSTRUCTION;
	}
}

int16_t encode_directive(const char* directive_name) {
	if(!strcmp(directive_name, ORIG_DIRCT)) {
		return ORIG;
	} else if(!strcmp(directive_name, END_DIRCT)) {
		return END;
	} else if(!strcmp(directive_name, FILL_DIRCT)) {
		return FILL;
	} else if(!strcmp(directive_name, STRINGZ_DIRCT)) {
		return STRINGZ;
	} else {
		return INVALID_DIRECTIVE;
	}	
}