#ifndef TOKENS
#define TOKENS

#define TOKENSCOUNT 5
#define INIT_TOKENLIST_SIZE 8

typedef enum {
	WORD,
	NAT,
	NINT,
	NREAL,
	PREAL,
	STR,
	IF,
	ELSE,
	CASE,
	ELSECASE,
	LOOP,
	EXIT,
	RETURN,
	INPUT,
	OUTPUT,
	DTYPE,
	ORND,
	CRND,
	OCRL,
	CCRL,
	SEMI,
	COMM,
	ASSN,
	SUM,
	SUB,
	MUL,
	DIV,
	MOD,
	AND,
	OR,
	NOT,
	NEQ,
	EQ,
	LT,
	LEQ,
	GT,
	GEQ,
	EOTL,
	UNRECOGNIZED
} TokenName;

typedef struct {
	char* value;
	TokenName name;
} Token;

TokenName recognize_token(char* target);

#endif
