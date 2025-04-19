parsertest: main.o symbol_table.o codegen.o parser.o lexer.o automata.o tokens.o
	@gcc -o coffeemaker main.o symbol_table.o codegen.o parser.o lexer.o automata.o tokens.o

main.o: main.c
	@gcc main.c -o main.o -c

symbol_table.o: symbol_table.c
	@gcc symbol_table.c -o symbol_table.o -c

codegen.o: codegen/codegen.c
	@gcc codegen/codegen.c -o codegen.o -c

parser.o: parser/parser.c
	@gcc Parser/parser.c -o parser.o -c

lexer.o: lexer/lexer.c
	@gcc lexer/lexer.c -o lexer.o -c

automata.o lexer/automata.c:
	@gcc lexer/automata.c -o automata.o -c

tokens.o: lexer/tokens.c
	@gcc lexer/tokens.c -o tokens.o -c

clean:
	@rm main.o symbol_table.o codegen.o parser.o lexer.o automata.o tokens.o
