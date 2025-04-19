#include <string.h>
#include "../globals.h"
#include "automatonlib.h"

State nat_automaton(char target, Bool reset) {
	static State state = A;

	if(reset) {
		state = A;
		return state;
	}
	
	switch(state) {
		case A:
			if(target == '0') state = B;
			else if(NONZERO_NUMBER(target)) state = C;
			else state = ERR;
		break;
		case B:
			state = ERR;
		break;
		case C:
			if(NUMBER(target)); // state remains C
			else state = ERR;
		break;
		default:
			return False;
		break;
	}

	return (state == B || state == C) ? FINAL : state;
}

State nint_automaton(char target, Bool reset) {
	static State state = A;

	if(reset) {
		state = A;
		return state;
	}

	switch(state) {
		case A:
			if(target == '-') state = B;
			else state = ERR;
		break;
		case B:
			if(NONZERO_NUMBER(target)) state = C;
			else state = ERR;
		break;
		case C:
			if(NUMBER(target)); // state remains C
			else state = ERR;
		break;
		default:
			return False;
		break;
	}

	return (state == C) ? FINAL : state;
}

State nreal_automaton(char target, Bool reset) {
	static State state = A;

	if(reset) {
		state = A;
		return state;
	}

	switch(state) {
		case A:
			if(target == '-') state = B;
			else state = ERR;
		break;
		case B:
			if(target == '0') state = C;
			else if(NONZERO_NUMBER(target)) state = D;
			else state = ERR;
		break;
		case C:
			if(target == '.') state = E;
			else state = ERR;
		break;
		case D:
			if(target == '.') state = E;
			else if(NUMBER(target)); // state remains D
			else state = ERR;
		break;
		case E:
			if(target == '0'); // state remains E
			else if(NONZERO_NUMBER(target)) state = F;
			else state = ERR;
		break;
		case F:
			if(target == '0') state = E;
			else if(NONZERO_NUMBER(target)); // state remains F
			else state = ERR;
		break;
		default:
			return False;
		break;
	}

	return (state == F) ? FINAL : state;
}

State preal_automaton(char target, Bool reset) {
	static State state = A;

	if(reset) {
		state = A;
		return state;
	}

	switch(state) {
		case A:
			if(target == '0') state = B;
			else if(NONZERO_NUMBER(target)) state = C;
			else state = ERR;
		break;
		case B:
			if(target == '.') state = D;
			else state = ERR;
		break;
		case C:
			if(target == '.') state = D;
			else if(NUMBER(target)); // state remains C
			else state = ERR;
		break;
		case D:
			if(target == '0'); // state remains D
			else if(NONZERO_NUMBER(target)) state = E;
			else state = ERR;
		break;
		case E:
			if(target == '0') state = D;
			else if(NONZERO_NUMBER(target)); // state remains E
			else state = ERR;
		break;
		default:
			return False;
		break;
	}

	return (state == E) ? FINAL : state;
}

State word_automaton(char target, Bool reset) {
	static State state = A;
	
	if(reset) {
		state = A;
		return state;
	}
	
	switch(state) {
		case A:
			if(ALPHA(target) || target == '_') state = B;
			else if(NUMBER(target)) state = C;
			else state = ERR;
		break;
		case B:
			if(ALPHA(target) || target == '_'); // state remains B
			else if(NUMBER(target)) state = D;
			else state = ERR;
		break;
		case C:
			if(ALPHA(target) || target == '_') state = B;
			else if(NUMBER(target)); // state remains C
			else state = ERR;
		break;
		case D:
			if(ALPHA(target) || target == '_') state = B;
			else if(NUMBER(target)); // state remains D
			else state = ERR;
		break;
		default:
			return False;
		break;
	}
	
	return (state == B || state == D) ? FINAL : state;
}
