#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#define SYMTAB_INIT_SIZE 16

typedef char Bool;

typedef struct {
    char* name;
    char* value;
    char* dtype;
} SymbolTableRecord;

void symtab_insert(SymbolTableRecord** symtab, size_t* st_size, size_t* st_curs, char* dtype, char* name, char* value);
void symtab_remove(SymbolTableRecord** symtab, size_t st_curs, char* name);
Bool in_symtab(SymbolTableRecord** symtab, size_t st_curs, char* name);
char* type_in_symtab(SymbolTableRecord** symtab, size_t st_curs, char* name);
char* value_in_symtab(SymbolTableRecord** symtab, size_t st_curs, char* name);

#endif