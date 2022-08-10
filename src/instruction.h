#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#define BR_INSTR 			"BR"
#define ADD_INSTR 			"ADD"
#define LD_INSTR 			"LD"
#define ST_INSTR 			"ST"
#define JSRR_INSTR 			"JSRR"
#define AND_INSTR 			"AND"
#define LDR_INSTR 			"LDR"
#define STR_INSTR 			"STR"
#define RTI_INSTR 			"RTI"
#define NOT_INSTR 			"NOT"
#define LDI_INSTR 			"LDI"
#define STI_INSTR 			"STI"
#define RET_INSTR 			"RET"
#define LEAD_INSTR 			"LEAD"
#define TRAP_INSTR 			"TRAP" 
#define GETC_INSTR 			"GETC"
#define OUT_INSTR 			"OUT"
#define PUTS_INSTR 			"PUTS"
#define IN_INSTR 			"IN"
#define PUTSP_INSTR 		"PUTSP"
#define HALT_INSTR 			"HALT"

#define ORIG_DIRCT 			".ORIG"
#define	END_DIRCT			".END"
#define	FILL_DIRCT			".FILL"
#define	BLKW_DIRCT			".BLKW"
#define	STRINGZ_DIRCT		".STRINGZ"

#define INVALID_INSTRUCTION 0xFFFA
#define INVALID_DIRECTIVE 	0xFFFB

typedef enum {
	BR = 0,
	ADD,
	LD,
	ST,
	JSRR,
	AND,
	LDR,
	STR,
	RTI,
	NOT,
	LDI,
	STI,
	RET,
	RESERVED,
	LEAD,
	TRAP
} instruction_code_t;

typedef enum {
	GETC = 0x20,
	OUT,
	PUTS,
	IN,
	PUTSP,
	HALT
} trap_code_t;

typedef enum {
	ORIG = 0x100,
	END,
	FILL,
	BLKW,
	STRINGZ
} directive_t;

typedef struct {
	int line;
	uint16_t opcode;
	uint16_t dr;
	uint16_t sr1;
	uint16_t sr2;
	uint8_t imm;
	uint16_t offset;
	uint8_t trapvect;
	uint8_t mode;
	union {
		uint16_t num_value;
		char* str_value;
	} pseudo_op_arg;
} instruction_t;

uint16_t encode_instruction(const char*);
int16_t encode_directive(const char* directive_name);

#endif
