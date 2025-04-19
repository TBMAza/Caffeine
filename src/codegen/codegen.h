#ifndef CODEGEN
#define CODEGEN

void codegen(ASLNode* abstract_syntax_list, SymbolTableRecord** symtab, size_t st_curs, char* filename);
void generate_exit_ins(ExitNode* exitnode, FILE* objfile);
void generate_definition_ins(DefinitionNode* definition_node, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile);
void generate_assignment_ins(AssignmentNode* definition_node, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile);
void generate_output_ins(OutputNode* output_node, FILE* objfile);
void generate_loop_ins(LoopNode* loopnode, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile);
void generate_if_ins(IfNode* ifnode, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile);
void generate_case_ins(CaseNode* casenode, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile);
char* args_list_to_str(char **strings, size_t count);

#endif