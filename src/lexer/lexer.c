#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../globals.h"

#include "tokens.h"

#include "automatonlib.h"
#include "automata.h"

#include "lexer.h"

Token* lexer(FILE* program) {
	TokenName token_names[] = {WORD, NAT, PREAL, NINT, NREAL};
	State state[] = {INITIAL, INITIAL, INITIAL, INITIAL, INITIAL};
	Automaton automaton[] = {word_automaton, nat_automaton, preal_automaton, nint_automaton, nreal_automaton};
	
	size_t buffer_size = INIT_BUFFER_SIZE, buffer_cursor = 0;
	char* buffer = (char*) malloc(sizeof(char) * buffer_size); buffer[0] = '\0';
	if(!buffer) goto Buffer_Allocation_Error;
	
	size_t tokenlist_size = INIT_TOKENLIST_SIZE, tokenlist_cursor = 0;
	Token* tokenlist = (Token*) malloc(sizeof(Token) * tokenlist_size);
	if(!tokenlist) goto Token_List_Allocation_Error;
	
	char ch;
	while((ch = fgetc(program)) != EOF) {
		if(ch == ';') {
			tokenlist[tokenlist_cursor].name = SEMI;
			tokenlist[tokenlist_cursor].value = ";";
			++tokenlist_cursor;
			// resize token list (array) if needed
			if(tokenlist_cursor >= tokenlist_size) {
				tokenlist_size *= 2;
				Token* resized_tokenlist = (Token*) realloc(tokenlist, sizeof(Token) * tokenlist_size);
				if(!resized_tokenlist) goto Token_List_Reallocation_Error;

				tokenlist = resized_tokenlist;
			}

			while((ch = fgetc(program)) != EOF && ch != '\n'); // when a semicolon is detected, go to next line
		}

		else if(ch == '`') {
			while((ch = fgetc(program)) != EOF && ch != '`') {
				buffer[buffer_cursor] = ch;
				++buffer_cursor;
				// resize buffer if needed
				if(buffer_cursor >= buffer_size) {
					buffer_size *= 2;
					char* resized_buffer = (char*) realloc(buffer, sizeof(char) * buffer_size);
					if(!resized_buffer) goto Buffer_Reallocation_Error;
					
					buffer = resized_buffer;
				}
			}
			buffer[buffer_cursor] = '\0';

			if(ch == '`') {
				tokenlist[tokenlist_cursor].name = STR;
				tokenlist[tokenlist_cursor].value = (char*) malloc(strlen(buffer) + 1);
				if(!tokenlist[tokenlist_cursor].value) goto Token_Value_Allocation_Error;
				strcpy(tokenlist[tokenlist_cursor].value, buffer);
				++tokenlist_cursor;
				// resize token list (array) if needed
				if(tokenlist_cursor >= tokenlist_size) {
					tokenlist_size *= 2;
					Token* resized_tokenlist = (Token*) realloc(tokenlist, sizeof(Token) * tokenlist_size);
					if(!resized_tokenlist) goto Token_List_Reallocation_Error;

					tokenlist = resized_tokenlist;
				}

				// shrink back buffer to initial size before reusing it
				buffer_size = INIT_BUFFER_SIZE;
				char* resized_buffer = (char*) realloc(buffer, sizeof(char) * buffer_size);
				if(!resized_buffer) goto Buffer_Reallocation_Error;

				buffer_cursor = 0;
				buffer[0] = '\0';
			}
			else goto Unclosed_String_Error;
		}
		
		else if(!EMPTY_CHAR(ch)) {
			do {
				buffer[buffer_cursor] = ch;
				for(int j = 0; j < TOKENSCOUNT; ++j) state[j] = automaton[j](ch, False);
				++buffer_cursor;
				// resize buffer if needed
				if(buffer_cursor >= buffer_size) {
					buffer_size *= 2;
					char* resized_buffer = (char*) realloc(buffer, sizeof(char) * buffer_size);
					if(!resized_buffer) goto Buffer_Reallocation_Error;

					buffer = resized_buffer;
				}

			} while((ch = fgetc(program)) != EOF && !EMPTY_CHAR(ch) && !CONCATENABLE_CHAR(ch));

			buffer[buffer_cursor] = '\0';

			Bool state_detected = False;
			for(int j = 0; j < TOKENSCOUNT; ++j) {
				if(state[j] == FINAL) {
					if(!state_detected) {
						tokenlist[tokenlist_cursor].name = token_names[j];
						tokenlist[tokenlist_cursor].value = (char*) malloc(strlen(buffer) + 1);
						if(!tokenlist[tokenlist_cursor].value) goto Token_Value_Allocation_Error;
						strcpy(tokenlist[tokenlist_cursor].value, buffer);

						if(j == 0) { // i.e. a WORD token type was detected
							TokenName tokenname = recognize_token(buffer);
							if(tokenname != UNRECOGNIZED) {
								tokenlist[tokenlist_cursor].name = tokenname;
								tokenlist[tokenlist_cursor].value = (char*) malloc(strlen(buffer) + 1);
								if(!tokenlist[tokenlist_cursor].value) goto Token_Value_Allocation_Error;
								strcpy(tokenlist[tokenlist_cursor].value, buffer);
							}
						}
						
						++tokenlist_cursor;
						// resize token list (array) if needed
						if(tokenlist_cursor >= tokenlist_size) {
							tokenlist_size *= 2;
							Token* resized_tokenlist = (Token*) realloc(tokenlist, sizeof(Token) * tokenlist_size);
							if(!resized_tokenlist) goto Token_List_Reallocation_Error;

							tokenlist = resized_tokenlist;
						}
						
						state_detected = True;
					}
					else goto Unrecognized_Token_Error;
				}
			}
			if(!state_detected) {
				TokenName tokenname = recognize_token(buffer);
				if(tokenname == UNRECOGNIZED) goto Unrecognized_Token_Error;

				tokenlist[tokenlist_cursor].name = tokenname;
				tokenlist[tokenlist_cursor].value = (char*) malloc(strlen(buffer) + 1);
				if(!tokenlist[tokenlist_cursor].value) goto Token_Value_Allocation_Error;
				strcpy(tokenlist[tokenlist_cursor].value, buffer);
				++tokenlist_cursor;
				// resize token list (array) if needed
				if(tokenlist_cursor >= tokenlist_size) {
					tokenlist_size *= 2;
					Token* resized_tokenlist = (Token*) realloc(tokenlist, sizeof(Token) * tokenlist_size);
					if(!resized_tokenlist) goto Token_List_Reallocation_Error;

					tokenlist = resized_tokenlist;
				}
			}

			if(CONCATENABLE_CHAR(ch)) {
				char strch[] = {ch, '\0'};
				TokenName tokenname = recognize_token(strch);
				tokenlist[tokenlist_cursor].name = tokenname;
				tokenlist[tokenlist_cursor].value = (char*) malloc(strlen(strch) + 1);
				if(!tokenlist[tokenlist_cursor].value) goto Token_Value_Allocation_Error;
				strcpy(tokenlist[tokenlist_cursor].value, strch);
				++tokenlist_cursor;
				// resize token list (array) if needed
				if(tokenlist_cursor >= tokenlist_size) {
					tokenlist_size *= 2;
					Token* resized_tokenlist = (Token*) realloc(tokenlist, sizeof(Token) * tokenlist_size);
					if(!resized_tokenlist) goto Token_List_Reallocation_Error;

					tokenlist = resized_tokenlist;
				}

				if(ch == ';') while((ch = fgetc(program)) != EOF && ch != '\n');
			}

			// shrink back buffer to initial size before reusing it
			buffer_size = INIT_BUFFER_SIZE;
			char* resized_buffer = (char*) realloc(buffer, sizeof(char) * buffer_size);
			if(!resized_buffer) goto Buffer_Reallocation_Error;

			buffer_cursor = 0;
			buffer[0] = '\0';
			
			for(int j = 0; j < TOKENSCOUNT; ++j) automaton[j]('r', True);
		}
	}

	free(buffer);

	tokenlist[tokenlist_cursor].name = EOTL;
	tokenlist[tokenlist_cursor].value = "End of token list";

	return tokenlist;








Buffer_Allocation_Error:
	printf("[LEXER] [INTERNAL ERROR] Buffer allocation failed.\n\n\n");
	exit(1);

Buffer_Reallocation_Error:
	FREEHEAP;
	printf("[LEXER] [INTERNAL ERROR] Buffer reallocation failed.\n\n\n");
	exit(2);

Token_List_Allocation_Error:
	printf("[LEXER] [INTERNAL ERROR] Token list allocation failed.\n\n\n");
	exit(3);

Token_List_Reallocation_Error:
	FREEHEAP;
	printf("[LEXER] [INTERNAL ERROR] Token list reallocation failed.\n\n\n");
	exit(4);

Token_Value_Allocation_Error:
	FREEHEAP;
	printf("[LEXER] [INTERNAL ERROR] Token value allocation failed.\n\n\n");
	exit(4);

Unclosed_String_Error:
	FREEHEAP;
	printf("[LEXER] [EXTERNAL ERROR] Unclosed string.\n\n\n");
	exit(5);

Unrecognized_Token_Error:
	FREEHEAP;
	printf("[LEXER] [EXTERNAL ERROR] Unrecognized token: %s.\n\n\n", buffer);
	exit(6);
}