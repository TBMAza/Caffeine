#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../globals.h"
#include "../symbol_table.h"
#include "../lexer/tokens.h"
#include "../parser/parser.h"

#include "codegen.h"

Bool case_state = False;

void codegen(ASLNode* abstract_syntax_list, SymbolTableRecord** symtab, size_t st_curs, char* filename) {
    char exe_name[strlen(filename + 1)]; strcpy(exe_name, filename);
    char* objfile_name = strcat(filename, ".c");

    FILE* objfile = fopen(objfile_name, "a");
    if(!objfile) {
        printf("[CODE GENERATOR] [INTERNAL ERROR] Failed to create object file.\n\n\n");
        exit(255);
    }

    fprintf(objfile,
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "int main() {\n"
    );

    for(size_t asl_curs = 0; abstract_syntax_list[asl_curs].instype != EOP; ++asl_curs) {
        switch(abstract_syntax_list[asl_curs].instype) {
            case EXITS:
                case_state = False;
                generate_exit_ins((ExitNode*) abstract_syntax_list[asl_curs].instruction, objfile);
            break;
            
            case DEFINES:
                case_state = False;
                generate_definition_ins((DefinitionNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case ASSIGNS:
                case_state = False;
                generate_assignment_ins((AssignmentNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case OUTPUTS:
                case_state = False;
                generate_output_ins((OutputNode*) abstract_syntax_list[asl_curs].instruction, objfile);
            break;

            case LOOPS:
                case_state = False;
                generate_loop_ins((LoopNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case IFS:
                case_state = False;
                generate_if_ins((IfNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case CASES:
                generate_case_ins((CaseNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            default:
                printf("[PARSER] [WTF] It should be literally impossible to reach this part of the code :/\n\n\n");
                exit(69);
            break;
        }
    }

    fprintf(objfile,
        "return 0;\n"
        "}\n"
    );

    fclose(objfile);

    char command[strlen(objfile_name) + strlen(exe_name) + 16];
    snprintf(command, sizeof(command), "gcc %s -o %s", objfile_name, exe_name);
    if(system(command) != 0) {
        printf("[CODE GENERATOR] [EXTERNAL ERROR] Code generation failed.\n\n\n");
        exit(255);
    }
    remove(objfile_name);
}

void codegen_2(ASLNode* abstract_syntax_list, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile) {
    for(size_t asl_curs = 0; abstract_syntax_list[asl_curs].instype != EOP; ++asl_curs) {
        switch(abstract_syntax_list[asl_curs].instype) {
            case EXITS:
                case_state = False;
                generate_exit_ins((ExitNode*) abstract_syntax_list[asl_curs].instruction, objfile);
            break;
            
            case DEFINES:
                case_state = False;
                generate_definition_ins((DefinitionNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case ASSIGNS:
                case_state = False;
                generate_assignment_ins((AssignmentNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case OUTPUTS:
                case_state = False;
                generate_output_ins((OutputNode*) abstract_syntax_list[asl_curs].instruction, objfile);
            break;

            case LOOPS:
                case_state = False;
                generate_loop_ins((LoopNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case IFS:
                case_state = False;
                generate_if_ins((IfNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            case CASES:
                generate_case_ins((CaseNode*) abstract_syntax_list[asl_curs].instruction, symtab, st_curs, objfile);
            break;

            default:
                printf("[PARSER] [WTF] It should be literally impossible to reach this part of the code :/\n\n\n");
                exit(69);
            break;
        }
    }

    fprintf(objfile,
        "}\n"
    );
}

void generate_exit_ins(ExitNode* exitnode, FILE* objfile) {
    fprintf(objfile,
        "exit(%d);\n", exitnode->exitcode
    );
}

void generate_definition_ins(DefinitionNode* definition_node, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile) {
    char* dtype = type_in_symtab(symtab, st_curs, definition_node->varname);
    char* value = value_in_symtab(symtab, st_curs, definition_node->varname);

    if(value) {
        if(!strcmp(dtype, "Int")) {
            fprintf(objfile,
                "int %s = %s;\n", definition_node->varname, value
            );
        }
        else if(!strcmp(dtype, "Flt")) {
            fprintf(objfile,
                "double %s = %s;\n", definition_node->varname, value
            );
        }
        else if(!strcmp(dtype, "Str")) {
            fprintf(objfile,
                "char* %s = \"%s\";\n", definition_node->varname, value
            );
        }
    }
    else {
        if(!strcmp(dtype, "Int")) {
            fprintf(objfile,
                "int %s; scanf(\"%%d\", &%s);\n", definition_node->varname, definition_node->varname
            );
        }
        else if(!strcmp(dtype, "Flt")) {
            fprintf(objfile,
                "double %s; scanf(\"%%lf\", &%s);\n", definition_node->varname, definition_node->varname
            );
        }
        else if(!strcmp(dtype, "Str")) {
            fprintf(objfile,
                "char* %s; scanf(\"%%s\", %s);\n", definition_node->varname, definition_node->varname
            );
        }
    }
    symtab_remove(symtab, st_curs, definition_node->varname);
}

void generate_assignment_ins(AssignmentNode* assignment_node, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile) {
    char* dtype = type_in_symtab(symtab, st_curs, assignment_node->varname);
    char* value = value_in_symtab(symtab, st_curs, assignment_node->varname);

    if(value) {
        fprintf(objfile,
            "%s = %s;\n", assignment_node->varname, value
        );
    }
    else {
        if(!strcmp(dtype, "Int")) {
            fprintf(objfile,
                "%s; scanf(\"%%d\", &%s);\n", assignment_node->varname, assignment_node->varname
            );
        }
        else if(!strcmp(dtype, "Flt")) {
            fprintf(objfile,
                "%s; scanf(\"%%lf\", &%s);\n", assignment_node->varname, assignment_node->varname
            );
        }
        else if(!strcmp(dtype, "Str")) {
            fprintf(objfile,
                "%s; scanf(\"%%s\", %s);\n", assignment_node->varname, assignment_node->varname
            );
        }
    }
    symtab_remove(symtab, st_curs, assignment_node->varname);
}

void generate_output_ins(OutputNode* output_node, FILE* objfile) {
    if(output_node->args) {
        size_t counter = 0;
        for(int i = 0; (output_node->args)[i] != NULL; ++i) ++counter;
        char* args_str = args_list_to_str(output_node->args, counter);
        
        fprintf(objfile,
            "printf(\"%s\", %s);\n", output_node->outstr, args_str
        );
    }
    else {
        fprintf(objfile,
            "printf(\"%s\");\n", output_node->outstr
        );
    }
}

void generate_loop_ins(LoopNode* loopnode, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile) {
    fprintf(objfile,
        "while(%s) {\n", loopnode->condition_expr
    );
    codegen_2(loopnode->procedure, symtab, st_curs, objfile);
}

void generate_if_ins(IfNode* ifnode, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile) {
    fprintf(objfile,
        "if(%s) {\n", ifnode->condition_expr
    );
    codegen_2(ifnode->mainbranch, symtab, st_curs, objfile);
    if(ifnode->elsebrach) {
        fprintf(objfile,
            "else {\n"
        );
        codegen_2(ifnode->elsebrach, symtab, st_curs, objfile);
    }
}

void generate_case_ins(CaseNode* casenode, SymbolTableRecord** symtab, size_t st_curs, FILE* objfile) {
    if(case_state) {
        fprintf(objfile,
            "else if(%s) {\n", casenode->condition_expr
        );
    }
    else {
        fprintf(objfile,
            "if(%s) {\n", casenode->condition_expr
        );
        case_state = True;
    }
    
    if(case_state) {
        codegen_2(casenode->mainbranch, symtab, st_curs, objfile);
        case_state = True;
    }
    else codegen_2(casenode->mainbranch, symtab, st_curs, objfile);
    
    if(casenode->elsebrach) {
        fprintf(objfile,
            "else {\n"
        );
        codegen_2(casenode->elsebrach, symtab, st_curs, objfile);
    }
}

char* args_list_to_str(char **strings, size_t count) {
    if (count == 0) return strdup("");

    size_t total_length = 0;

    for (int i = 0; i < count; i++) {
        total_length += strlen(strings[i]);
        total_length += 4;
    }

    total_length += (count - 1);
    total_length += 1;

    char *result = malloc(total_length);
    if (!result) return NULL;

    result[0] = '\0';

    for (int i = 0; i < count; i++) {
        strcat(result, strings[i]);
        if (i < count - 1) {
            strcat(result, ",");
        }
    }

    return result;
}
