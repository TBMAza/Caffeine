#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "lexer/tokens.h"
#include "symbol_table.h"

void symtab_insert(SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs, char* dtype, char* name, char* value) {
    symtab[*st_curs] = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
    if(!symtab[*st_curs]) {
        printf("[COMPILER] [INTERNAL ERROR] Symbol table record allocation failed.\n\n\n");
        exit(2);
    }

    symtab[*st_curs]->name = (char*) malloc(strlen(name) + 1);
    symtab[*st_curs]->dtype = (char*) malloc(strlen(dtype) + 1);
    if(value) symtab[*st_curs]->value = (char*) malloc(strlen(value) + 1);
    else symtab[*st_curs]->value = NULL;

    if(!symtab[*st_curs]->name || !symtab[*st_curs]->dtype || (value && !symtab[*st_curs]->value)) {
        printf("[COMPILER] [INTERNAL ERROR] Symbol table data allocation failed.\n\n\n");
        exit(255);
    }

    strcpy(symtab[*st_curs]->name, name);
    strcpy(symtab[*st_curs]->dtype, dtype);
    if(value) strcpy(symtab[*st_curs]->value, value);

    ++(*st_curs);
    if(*st_curs >= *st_size) {
        (*st_size) = (*st_size) * 2;
        SymbolTableRecord** new_symtab = (SymbolTableRecord**)realloc(symtab, *st_size);
        if(!new_symtab) {
            printf("[COMPILER] [INTERNAL ERROR] Symbol table reallocation failed.\n\n\n");
            exit(2);
        }
        symtab = new_symtab;
    }
}

void symtab_remove(SymbolTableRecord** symtab, size_t st_curs, char* name) {
    size_t i = 0;
    while(i < st_curs) {
        if(symtab[i] && !strcmp(symtab[i]->name, name)) break;
        ++i;
    }
    free(symtab[i]->dtype); symtab[i]->dtype = NULL;
    free(symtab[i]->name); symtab[i]->name = NULL;
    free(symtab[i]->value); symtab[i]->value = NULL;
    free(symtab[i]); symtab[i] = NULL;
}

Bool in_symtab(SymbolTableRecord** symtab, size_t st_curs, char* name) {
    size_t i = 0;
    while(i < st_curs) {
        if(symtab[i] && !strcmp(symtab[i]->name, name)) return True;
        ++i;
    }
    
    return False;
}

char* type_in_symtab(SymbolTableRecord** symtab, size_t st_curs, char* name) {
    size_t i = 0;
    while(i < st_curs) {
        if(symtab[i] && !strcmp(symtab[i]->name, name)) return symtab[i]->dtype;
        ++i;
    }
    
    return NULL;
}

char* value_in_symtab(SymbolTableRecord** symtab, size_t st_curs, char* name) {
    size_t i = 0;
    while(i < st_curs) {
        if(symtab[i] && !strcmp(symtab[i]->name, name)) return symtab[i]->value;
        ++i;
    }
    
    return NULL;
}