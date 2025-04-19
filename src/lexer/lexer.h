#ifndef LEXER
#define LEXER

#define EMPTY_CHAR(x) ((x) == ' ' || (x) == '\n' || (x) == '\t')
#define CONCATENABLE_CHAR(x) ((x) == '(' || (x) == ')' || (x) == '{' || (x) == '}' || (x) == ',' || (x) == ';' || (x) == '`')

#define INIT_BUFFER_SIZE 128

#define FREEHEAP free(tokenlist); free(buffer)

typedef State (*Automaton)(char, Bool);

Token* lexer(FILE* program);

#endif
