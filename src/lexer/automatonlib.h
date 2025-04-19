#ifndef AUTOMATONLIB
#define AUTOMATONLIB

#define NUMBER(x) ((x) >= '0' && (x) <= '9')
#define NONZERO_NUMBER(x) ((x) >= '1' && (x) <= '9')
#define ALPHA(x) ((x) >= 'a' && (x) <= 'z' || (x) >= 'A' && (x) <= 'Z')

typedef enum __State__ {
	A, B, C, D, E, F, INITIAL, FINAL, ERR
} State;

#endif
