# Caffeine Compiler Documentation

## Overview

Caffeine is a simple programming language designed as a learning project to understand compiler construction and advanced C programming concepts. The language features basic programming constructs like variables, conditionals, loops, and I/O operations, but intentionally omits more complex features like functions to keep the focus on core compiler concepts.

---

## Project Structure

The compiler is divided into three main components:

- **Lexer**: Tokenizes the input source code  
- **Parser**: Builds an abstract syntax tree from the tokens  
- **Code Generator**: Produces executable output  

The project directory structure is organized as follows:

```
src/
├── Makefile                # Build configuration
├── codegen/                # Code generation components
├── lexer/                  # Lexical analysis components
├── parser/                 # Syntax analysis components
├── symbol_table.c          # Symbol table implementation
├── globals.h               # Global definitions
├── main.c                  # Main program
└── conditionals.c          # Conditional compilation support
```

---

## Language Features

### Data Types

Caffeine supports three basic data types:

- `Int`: Integer numbers (positive and negative)  
- `Flt`: Floating-point numbers  
- `Str`: Strings (enclosed in backticks \`)

---

### Variables

Variables must be declared with their type before use:

```cfn
Int count <- 0;
Flt pi <- 3.14;
Str greeting <- `Hello`;
```

---

### Input/Output

- `input`: Reads user input  
- `output`: Prints to standard output  

Example:

```cfn
output `Enter your name: `;
Str name <- input;
output `Hello %s`, name;
```

---

### Control Flow

#### Conditionals

```cfn
if condition_1, {
    ; code block
}
if condition_2, {
    ; code block
}

...

if condition_n, {
    ; code block
}
else, {
    ; alternative block
}
```

#### Case Statements

```cfn
case condition_1, {
    ; code block
}
case condition_2, {
    ; code block
}

...

case condition_n, {
    ; code block
}
elsecase, {
    ; default block
}
```

#### Loops

```cfn
loop condition, {
    ; repeated code
}
```

---

### Operators

- **Arithmetic**: `+`, `-`, `*`, `/`, `%`  
- **Comparison**: `>`, `<`, `>=`, `<=`, `=`, `!=`  
- **Logical**: `and`, `or`, `not`  

---

## Building the Compiler

Navigate to the `src` directory:

```bash
cd src/
```

Build the compiler:

```bash
make
```

This will generate the compiler executable named `coffeemaker`.

---

## Compiling Caffeine Programs

To compile a Caffeine program:

```bash
./coffeemaker <program.cfn> <output_executable>
```

Then run the compiled program:

```bash
./<output_executable>
```

---

## Examples

### Simple Arithmetic

```cfn
Flt a <- 0.1;
Flt b <- 0.2;
Flt c <- a + b;
output `The sum is %lf`, c;
```

### Conditional Logic

```cfn
Int age <- input;
if age >= 18, {
    output `Adult`;
}
else, {
    output `Minor`;
}
```

### Loop Example

```cfn
Int i <- 0;
loop i < 5, {
    output `Count: `, i;
    i <- i + 1;
}
```

---

## Limitations

As a learning project, Caffeine has several intentional limitations:

- No function support  (may be added later)
- No arrays or other complex data structures 
- Basic type system with very little type checking 
- Simple error handling  

---

## Contributing

Contributions are welcome! Since this is primarily a learning project, focus should be on:

- Improving code clarity  
- Adding educational comments  
- Enhancing error messages  
- Documenting implementation decisions  

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
