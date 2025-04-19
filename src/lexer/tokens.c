#include <string.h>
#include "tokens.h"

TokenName recognize_token(char* target) {
	if(!strcmp(target, "if")) return IF;
	if(!strcmp(target, "else")) return ELSE;
	if(!strcmp(target, "case")) return CASE;
	if(!strcmp(target, "elsecase")) return ELSECASE;
	if(!strcmp(target, "loop")) return LOOP;
	if(!strcmp(target, "exit")) return EXIT;
	if(!strcmp(target, "return")) return RETURN;

	if(!strcmp(target, "input")) return INPUT;
	if(!strcmp(target, "output")) return OUTPUT;

	if(!strcmp(target, "Int")  ||
	   !strcmp(target, "Flt")  ||
	   !strcmp(target, "Str")
	) return DTYPE;

	if(!strcmp(target, "(")) return ORND;
	if(!strcmp(target, ")")) return CRND;
	if(!strcmp(target, "{")) return OCRL;
	if(!strcmp(target, "}")) return CCRL;

	if(!strcmp(target, ";")) return SEMI;
	if(!strcmp(target, ",")) return COMM;

	if(!strcmp(target, "<-")) return ASSN;

	if(!strcmp(target, "!=")) return NEQ;
	if(!strcmp(target, "=")) return EQ;
	if(!strcmp(target, "<")) return LT;
	if(!strcmp(target, "<=")) return LEQ;
	if(!strcmp(target, ">")) return GT;
	if(!strcmp(target, ">=")) return GEQ;

	if(!strcmp(target, "+")) return SUM;
	if(!strcmp(target, "-")) return SUB;
	if(!strcmp(target, "*")) return MUL;
	if(!strcmp(target, "/")) return DIV;
	if(!strcmp(target, "%")) return MOD;
	if(!strcmp(target, "and")) return AND;
	if(!strcmp(target, "or")) return OR;
	if(!strcmp(target, "not")) return NOT;

	return UNRECOGNIZED;
}
