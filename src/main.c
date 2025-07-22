#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "symbol_table.h"

#include "lexer/automatonlib.h"
#include "lexer/tokens.h"
#include "lexer/lexer.h"

#include "parser/parser.h"

#include "codegen/codegen.h"

int main(int argc, char** argv) {
	// check if FILENAME was provided
	if(argc < 3) {
		printf("[COMPILER] [EXTERNAL ERROR] Usage: ./coffeemaker <cfn file path> <generated executable name>\n\n\n");
		exit(255);
	}
	// check if extension is correct
	int filename_len = strlen(FILENAME);
	if(filename_len < MIN_FILENAME_LEN) {
		printf("[COMPILER] [EXTERNAL ERROR] Not a .cfn file.\n\n\n");
		exit(255);
	}
	char extension[] = FILENAME_EXT;
	if(strcmp(extension, ".cfn")) {
		printf("[COMPILER] [EXTERNAL ERROR] Not a .cfn file.\n\n\n");
		exit(255);
	}
	// open file, exit if cannot
	FILE* program = fopen(FILENAME, "r");
	if(!program) {
		printf("[COMPILER] [ERROR] Could not open file.\n\n\n");
		exit(1);
	}

	size_t st_size = SYMTAB_INIT_SIZE, st_curs = 0;
    SymbolTableRecord** symbol_table = (SymbolTableRecord**) malloc(st_size * sizeof(SymbolTableRecord*));
    if(!symbol_table) {
        printf("[COMPILER] [INTERNAL ERROR] Symbol table allocation failed.\n\n\n");
        goto End_Of_Program;
    }

    printf("[COMPILER] [PROGRESS] Compilation started.\n");
	
	printf("[COMPILER] [PROGRESS] Tokenizing...\n");
		Token* tokenlist = lexer(program); size_t tl_curs = 0;
	printf("[COMPILER] [PROGRESS] Tokenizing done.\n");
	
	printf("[COMPILER] [PROGRESS] Parsing...\n");
		ASLNode* abstract_syntax_list = parse_program(tokenlist, &tl_curs, symbol_table, &st_size, &st_curs);
	printf("[COMPILER] [PROGRESS] Parsing done.\n");

	printf("[COMPILER] [PROGRESS] Generating executable...\n");
		codegen(abstract_syntax_list, symbol_table, st_curs, EXENAME);
	printf("[COMPILER] [PROGRESS] Executable generated.\n");
	
	printf("[COMPILER] [SUCCESS] Compilation successful.\n\n\n");

	End_Of_Program:
		fclose(program);
		free(symbol_table);
		return 0;
}
