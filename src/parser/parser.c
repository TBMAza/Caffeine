#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../symbol_table.h"
#include "../lexer/tokens.h"
#include "../lexer/automatonlib.h"

#include "parser.h"

ASLNode* parse_program(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs) {
    size_t asl_size = ASL_INIT_SIZE, asl_curs = 0;
    ASLNode* asl = (ASLNode*) malloc(sizeof(ASLNode) * asl_size);
    if(!asl) goto ASL_Allocation_Error;

    while(tokenlist[*tl_curs].name != EOTL && tokenlist[*tl_curs].name != CCRL) {
        switch(tokenlist[*tl_curs].name) {
            case EXIT:
                asl[asl_curs].instype = EXITS;
                asl[asl_curs].instruction = (ExitNode*) malloc(sizeof(ExitNode));
                if(!asl[asl_curs].instruction) goto ASL_Node_Allocation_Error;
                
                ++(*tl_curs);
                if(tokenlist[*tl_curs].name != NAT) {
                    printf("[PARSER] [EXTERNAL ERROR] Invalid token after 'exit' statement: %s; expected unsigned interger.\n\n\n", tokenlist[*tl_curs].value);
                    FREEHEAP2;
                    exit(255);
                }
                ((ExitNode*)asl[asl_curs].instruction)->exitcode = atoi(tokenlist[*tl_curs].value);

                ++(*tl_curs);
                if(tokenlist[*tl_curs].name != SEMI) {
                    printf("[PARSER] [EXTERNAL ERROR] Expected ';', found %s.\n\n\n", tokenlist[*tl_curs].value);
                    FREEHEAP2;
                    exit(255);
                }

                ++(*tl_curs);
            break;

            case DTYPE:
                asl[asl_curs].instype = DEFINES;
                asl[asl_curs].instruction = (DefinitionNode*) malloc(sizeof(DefinitionNode));
                if(!asl[asl_curs].instruction) goto ASL_Node_Allocation_Error;

                char* dtype = tokenlist[*tl_curs].value;
                
                ++(*tl_curs);
                if(tokenlist[*tl_curs].name != WORD) {
                    printf("[PARSER] [EXTERNAL ERROR] Invalid token in definition statement: %s; expected identifier.\n\n\n", tokenlist[*tl_curs].value);
                    FREEHEAP2;
                    exit(255);
                }
                char* name = tokenlist[*tl_curs].value;
                ((DefinitionNode*)(asl[asl_curs].instruction))->varname = name;

                ++(*tl_curs);
                if(tokenlist[*tl_curs].name != ASSN) {
                    printf("[PARSER] [EXTERNAL ERROR] Invalid token in definition statement: %s; expected '<-'.\n\n\n", tokenlist[*tl_curs].value);
                    FREEHEAP2;
                    exit(255);
                }

                ++(*tl_curs);
                char* value = NULL;
                if(tokenlist[*tl_curs].name == INPUT) ++(*tl_curs); // value remains null and code generator understands that it has to get value from user input
                else if(tokenlist[*tl_curs].name == STR) {
                    value = tokenlist[*tl_curs].value;
                    ++(*tl_curs);
                }
                else {
                    value = eval_expr(tokenlist, tl_curs, symtab, st_curs);
                    if(!value) {
                        printf("[PARSER] [EXTERNAL ERROR] Invalid expression in assignment.\n\n\n");
                        FREEHEAP2;
                        exit(255);
                    }
                }

                symtab_insert(symtab, st_size, st_curs, dtype, name, value);

                if(tokenlist[*tl_curs].name != SEMI) {
                    printf("[PARSER] [EXTERNAL ERROR] Expected ';', found %s.\n\n\n", tokenlist[*tl_curs].value);
                    FREEHEAP2;
                    exit(255);
                }

                ++(*tl_curs);
            break;

            case WORD:
                asl[asl_curs].instype = ASSIGNS;
                asl[asl_curs].instruction = (AssignmentNode*) malloc(sizeof(AssignmentNode));
                if(!asl[asl_curs].instruction) goto ASL_Node_Allocation_Error;

                char* name2 = tokenlist[*tl_curs].value;
                if(!in_symtab(symtab, *st_curs, name2)) goto Undefined_Variable_Error;
                ((AssignmentNode*)(asl[asl_curs].instruction))->varname = name2;

                char* dtype2 = type_in_symtab(symtab, *st_curs, name2);

                ++(*tl_curs);
                if(tokenlist[*tl_curs].name != ASSN) {
                    printf("[PARSER] [EXTERNAL ERROR] Invalid token in definition statement: %s; expected '<-'.\n\n\n", tokenlist[*tl_curs].value);
                    FREEHEAP2;
                    exit(255);
                }

                ++(*tl_curs);
                char* value2 = NULL;
                if(tokenlist[*tl_curs].name == INPUT) ++(*tl_curs); // value remains null and code generator understands that it has to get value from user input
                else if(tokenlist[*tl_curs].name == STR) {
                    value2 = tokenlist[*tl_curs].value;
                    ++(*tl_curs);
                }
                else {
                    value2 = eval_expr(tokenlist, tl_curs, symtab, st_curs);
                    if(!value) {
                        printf("[PARSER] [EXTERNAL ERROR] Invalid expression in assignment.\n\n\n");
                        FREEHEAP2;
                        exit(255);
                    }
                }

                symtab_insert(symtab, st_size, st_curs, dtype2, name2, value2);

                if(tokenlist[*tl_curs].name != SEMI) {
                    printf("[PARSER] [EXTERNAL ERROR] Expected ';', found %s.\n\n\n", tokenlist[*tl_curs].value);
                    FREEHEAP2;
                    exit(255);
                }

                ++(*tl_curs);
            break;

            case OUTPUT:
                asl[asl_curs].instype = OUTPUTS;
                asl[asl_curs].instruction = parse_output(tokenlist, tl_curs, symtab, st_curs);
            break;

            case LOOP:
                asl[asl_curs].instype = LOOPS;
                asl[asl_curs].instruction = parse_loop(tokenlist, tl_curs, symtab, st_size, st_curs);
            break;

            case IF:
                asl[asl_curs].instype = IFS;
                asl[asl_curs].instruction = parse_if(tokenlist, tl_curs, symtab, st_size, st_curs);
            break;

            case CASE:
                asl[asl_curs].instype = CASES;
                asl[asl_curs].instruction = parse_case(tokenlist, tl_curs, symtab, st_size, st_curs);
            break;

            default:
                printf("[PARSER] [EXTERNAL ERROR] Invalid instruction: %s...\n\n\n", tokenlist[*tl_curs].value);
                FREEHEAP2;
                exit(255);
            break;
        }

        ++asl_curs;
        if(asl_curs >= asl_size) {
            asl_size *= 2;
            ASLNode* resized_asl = (ASLNode*) realloc(asl, sizeof(ASLNode) * asl_size);
            if(!resized_asl) goto ASL_Reallocation_Error;

            asl = resized_asl;
        }
    }
    asl[asl_curs].instype = EOP;
    asl[asl_curs].instruction = NULL;
    return asl;









ASL_Allocation_Error:
    printf("[PARSER] [INTERNAL ERROR] Abstract syntax list allocation failed.\n\n\n");
    FREEHEAP2;
    exit(7);

ASL_Reallocation_Error:
    printf("[PARSER] [INTERNAL ERROR] Abstract syntax list reallocation failed.\n\n\n");
    FREEHEAP2;
    exit(8);

ASL_Node_Allocation_Error:
    printf("[PARSER] [INTERNAL ERROR] Abstract syntax list node allocation failed.\n\n\n");
    FREEHEAP2;
    exit(9);

Undefined_Variable_Error:
    printf("[PARSER] [INTERNAL ERROR] Undefined variable %s.\n\n\n", tokenlist[*tl_curs].value);
    FREEHEAP2;
    exit(10);
}

OutputNode* parse_output(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_curs) {
    OutputNode* output_node = (OutputNode*) malloc(sizeof(OutputNode));

    ++(*tl_curs);
    if(tokenlist[*tl_curs].name != STR) {
        printf("[PARSER] [EXTERNAL ERROR] Expected string after output statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
        free(output_node);
        exit(255);
    }
    output_node->outstr = tokenlist[*tl_curs].value;

    ++(*tl_curs);
    if(tokenlist[*tl_curs].name == SEMI) output_node->args = NULL;
    else if(tokenlist[*tl_curs].name == COMM) {
        size_t args = OUTPUT_ARGS_INIT_SIZE, args_curs = 0;
        output_node->args = (char**) malloc(sizeof(char*) * args);
        if(!output_node->args) {
            printf("[PARSER] [INTERNAL ERROR] Output identifiers list allocation failed.\n\n\n");
            free(output_node->args);
            free(output_node);
            exit(255);
        }
        ++(*tl_curs);
        while(tokenlist[*tl_curs].name != SEMI) {
            if(tokenlist[*tl_curs].name != WORD) {
                printf("[PARSER] [EXTERNAL ERROR] Expected identifier, found %s.\n\n\n", tokenlist[*tl_curs].value);
                free(output_node->args);
                free(output_node);
                exit(255);
            }
            output_node->args[args_curs] = tokenlist[*tl_curs].value;
            ++args_curs;
            if(args_curs >= args) {
                args *= 2;
                char** ext_args = (char**) realloc(output_node->args, sizeof(char*) * args);
                if(!ext_args) {
                    printf("[PARSER] [INTERNAL ERROR] Output identifiers list reallocation failed.\n\n\n");
                    free(output_node->args);
                    free(output_node);
                    exit(255);
                }
                output_node->args = ext_args;
            }
            ++(*tl_curs);
        }
        output_node->args[args_curs] = NULL;
    }
    else {
        printf("[PARSER] [EXTERNAL ERROR] Invalid token %s. Expected ';' or list of identifiers.\n\n\n", tokenlist[*tl_curs].value);
        free(output_node);
        exit(255);
    }

    ++(*tl_curs);
    return output_node;
}

LoopNode* parse_loop(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs) {
    LoopNode* loop_node = (LoopNode*) malloc(sizeof(LoopNode));
    if(!loop_node) {
        printf("[PARSER] [INTERNAL ERROR] Abstract syntax list node allocation failed.\n\n\n");
        exit(9);
    }
    
    ++(*tl_curs);
    loop_node->condition_expr = eval_expr(tokenlist, tl_curs, symtab, st_curs);
    if(!loop_node->condition_expr) {
        printf("[PARSER] [EXTERNAL ERROR] Invalid expression in loop condition.\n\n\n");
        free(loop_node);
        exit(255);
    }

    ++(*tl_curs);
    if(tokenlist[*tl_curs].name != OCRL) {
        printf("[PARSER] [EXTERNAL ERROR] Expected '{' after loop statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
        free(loop_node);
        exit(255);
    }
    
    ++(*tl_curs);
    loop_node->procedure = parse_program(tokenlist, tl_curs, symtab, st_size, st_curs);

    ++(*tl_curs);
    return loop_node;
}

IfNode* parse_if(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs) {
    IfNode* ifnode = (IfNode*) malloc(sizeof(IfNode));
    if(!ifnode) {
        printf("[PARSER] [INTERNAL ERROR] Abstract syntax list node allocation failed.\n\n\n");
        exit(9);
    }
    
    ++(*tl_curs);
    ifnode->condition_expr = eval_expr(tokenlist, tl_curs, symtab, st_curs);
    if(!ifnode->condition_expr) {
        printf("[PARSER] [EXTERNAL ERROR] Invalid expression in if condition.\n\n\n");
        free(ifnode);
        exit(255);
    }

    ++(*tl_curs);
    if(tokenlist[*tl_curs].name != OCRL) {
        printf("[PARSER] [EXTERNAL ERROR] Expected '{' after if statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
        free(ifnode);
        exit(255);
    }
    
    ++(*tl_curs);
    ifnode->mainbranch = parse_program(tokenlist, tl_curs, symtab, st_size, st_curs);

    ++(*tl_curs);
    if(tokenlist[*tl_curs].name == ELSE) {
        ++(*tl_curs);
        if(tokenlist[*tl_curs].name != COMM) {
            printf("[PARSER] [EXTERNAL ERROR] Expected ',' after else statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
            free(ifnode);
            exit(255);
        }
        ++(*tl_curs);
        if(tokenlist[*tl_curs].name != OCRL) {
            printf("[PARSER] [EXTERNAL ERROR] Expected '{' after else statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
            free(ifnode);
            exit(255);
        }

        ++(*tl_curs);
        ifnode->elsebrach = parse_program(tokenlist, tl_curs, symtab, st_size, st_curs);
    }
    else ifnode->elsebrach = NULL;

    return ifnode;
}

CaseNode* parse_case(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs) {
    CaseNode* case_node = (CaseNode*) malloc(sizeof(CaseNode));
    if(!case_node) {
        printf("[PARSER] [INTERNAL ERROR] Abstract syntax list node allocation failed.\n\n\n");
        exit(9);
    }
    
    ++(*tl_curs);
    case_node->condition_expr = eval_expr(tokenlist, tl_curs, symtab, st_curs);
    if(!case_node->condition_expr) {
        printf("[PARSER] [EXTERNAL ERROR] Invalid expression in case condition.\n\n\n");
        free(case_node);
        exit(255);
    }

    ++(*tl_curs);
    if(tokenlist[*tl_curs].name != OCRL) {
        printf("[PARSER] [EXTERNAL ERROR] Expected '{' after case statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
        free(case_node);
        exit(255);
    }
    
    ++(*tl_curs);
    case_node->mainbranch = parse_program(tokenlist, tl_curs, symtab, st_size, st_curs);

    ++(*tl_curs);
    if(tokenlist[*tl_curs].name == ELSECASE) {
        ++(*tl_curs);
        if(tokenlist[*tl_curs].name != COMM) {
            printf("[PARSER] [EXTERNAL ERROR] Expected ',' after elsecase statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
            free(case_node);
            exit(255);
        }
        ++(*tl_curs);
        if(tokenlist[*tl_curs].name != OCRL) {
            printf("[PARSER] [EXTERNAL ERROR] Expected '{' after elsecase statement, found %s.\n\n\n", tokenlist[*tl_curs].value);
            free(case_node);
            exit(255);
        }

        ++(*tl_curs);
        case_node->elsebrach = parse_program(tokenlist, tl_curs, symtab, st_size, st_curs);

        ++(*tl_curs);
    }
    else case_node->elsebrach = NULL;

    return case_node;
}

char* eval_expr(Token* tokenlist, size_t* tl_curs, SymbolTableRecord** symtab, size_t* st_curs) {
    size_t expr_len = EXPRESSION_INIT_LEN, expr_curs = 0;
    char** expression = (char**) malloc(sizeof(char*) * expr_len);
    if(!expression) {
        printf("[PARSER] [INTERNAL ERROR] Expression buffer allocation failed.\n\n\n");
        exit(255);
    }
    int pseudostack = 0;
    State state = A;
    while(tokenlist[*tl_curs].name != SEMI && tokenlist[*tl_curs].name != COMM && tokenlist[*tl_curs].name != EOTL) {
        switch(state) {
            case A:
                switch(tokenlist[*tl_curs].name) {
                    case NAT:
                    case NINT:
                    case PREAL:
                    case NREAL:
                        expression[expr_curs] = tokenlist[*tl_curs].value;
                        state = B;
                    break;
                    
                    case WORD:
                        if(!in_symtab(symtab, *st_curs, tokenlist[*tl_curs].value)) {
                            printf("[PARSER] [EXTERNAL ERROR] Undefined identifier in expression: %s.\n\n\n", tokenlist[*tl_curs].value);
                            free(expression);
                            exit(255);
                        }
                        expression[expr_curs] = tokenlist[*tl_curs].value;
                        state = B;
                    break;
                    
                    case ORND:
                        ++pseudostack;
                        expression[expr_curs] = tokenlist[*tl_curs].value;
                    break;
                    
                    default:
                        printf("[PARSER] [EXTERNAL ERROR] Invalid term in expression: %s.\n\n\n", tokenlist[*tl_curs].value);
                        free(expression);
                        exit(255);
                    break;
                }
            break;
    
            case B:
                switch (tokenlist[*tl_curs].name)
                {
                    case EQ:
                        expression[expr_curs] = "==";
                        state = A;
                    break;

                    case SUM:
                    case SUB:
                    case MUL:
                    case DIV:
                    case MOD:
                    case NEQ:
                    case LT:
                    case LEQ:
                    case GT:
                    case GEQ:
                        expression[expr_curs] = tokenlist[*tl_curs].value;
                        state = A;
                    break;

                    case AND:
                        expression[expr_curs] = "&&";
                        state = A;
                    break;

                    case OR:
                        expression[expr_curs] = "||";
                        state = A;
                    break;

                    case NOT:
                        expression[expr_curs] = "!";
                        state = A;
                    break;
                    
                    case CRND:
                        if(!pseudostack) {
                            printf("[PARSER] [EXTERNAL ERROR] Unbalanced parentheses in expression.\n\n\n");
                            free(expression);
                            exit(255);
                        }
                        --pseudostack;
                        expression[expr_curs] = tokenlist[*tl_curs].value;
                    break;

                    default:
                        printf("[PARSER] [EXTERNAL ERROR] Invalid term in expression: %s.\n\n\n", tokenlist[*tl_curs].value);
                        free(expression);
                        exit(255);
                    break;
                }
            break;
    
            default:
                printf("[PARSER] [WTF] It should be literally impossible to reach this part of the code :/\n\n\n");
                exit(69);
            break;
        }
        
        ++expr_curs;
        if(expr_curs >= expr_len) {
            expr_len *= 2;
            char** ext_expression = (char**) realloc(expression, sizeof(char*) * expr_len);
            if(!ext_expression) {
                printf("[PARSER] [INTERNAL ERROR] Expression buffer reallocation failed.\n\n\n");
                free(expression);
                exit(255);
            }
            expression = ext_expression;
        }
        
        ++(*tl_curs);
    }
    return (state == B && !pseudostack) ? concat_strings(expression, expr_curs) : NULL;
}

char* concat_strings(char **strings, int count) {
    size_t total_length = 0;

    for (int i = 0; i < count; i++) {
        total_length += strlen(strings[i]);
    }

    char *result = malloc(total_length + 1);
    if (!result) return NULL;

    result[0] = '\0';

    for (int i = 0; i < count; i++) {
        strcat(result, strings[i]);
    }

    return result;
}
