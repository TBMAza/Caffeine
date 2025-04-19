#ifndef PARSER
#define PARSER

#define ASL_INIT_SIZE 1
#define OUTPUT_ARGS_INIT_SIZE 1
#define EXPRESSION_INIT_LEN 16

#define FREEHEAP2 free(tokenlist);free(asl)

typedef enum {
    EXITS, DEFINES, ASSIGNS, INPUTS, OUTPUTS, LOOPS, IFS, CASES, EOP
} InstructionType;

typedef struct {
    InstructionType instype;
    void* instruction;
} ASLNode;

typedef struct {
    unsigned int exitcode;
} ExitNode;

typedef struct {
    char* outstr;
    char** args;
} OutputNode;

typedef struct {
    char* condition_expr;
    ASLNode* procedure;
} LoopNode;

typedef struct {
    char* varname;
} DefinitionNode;

typedef struct {
    char* varname;
} AssignmentNode;

typedef struct {
    char* condition_expr;
    ASLNode* mainbranch;
    ASLNode* elsebrach;
} IfNode;

typedef struct {
    char* condition_expr;
    ASLNode* mainbranch;
    ASLNode* elsebrach;
} CaseNode;

ASLNode* parse_program(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs);
OutputNode* parse_output(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_curs);
LoopNode* parse_loop(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs);
IfNode* parse_if(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs);
CaseNode* parse_case(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs);
char* eval_expr(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_curs);
char* concat_strings(char **strings, int count);

#endif