# Caffeine Language Specification

**Version 1.0**

---

## 1. Lexical Structure

### 1.1 Character Set

- ASCII-only encoding.  
- Case-sensitive (`loop` ≠ `LOOP`).

### 1.2 Tokens

| Category    | Examples                                    | Notes                                                |
| ----------- | ------------------------------------------- | ---------------------------------------------------- |
| Identifiers | `x`, `_count`, `tempVar`                    | Start with `_` or letter                             |
| Literals    | `42`, `-3`, `0.1`, `` `hello` ``            | Strings use backticks (``)                           |
| Operators   | `+`, `-`, `*`, `/`, `%`, `and`, `or`, `not` | Space-separated (see [1.3](#13-whitespace--comments) |
| Delimiters  | `;`, `,`, `{`, `}`, `(`, `)`                |                                                      |

### 1.3 Whitespace & Comments

- **Significant:** Spaces **required** around operators (e.g., `a + b`, **not** `a+b`).
- **Insignificant:** Elsewhere (e.g., `Int x<-input;` is invalid; use `Int x <- input;`).
- **Comments:** Text after `;` until end-of-line.

```cfn
Int x <- input;  ; This is a comment  
```

### 1.4 Escape Sequences & Format Specifiers

- Follows C conventions (e.g., `\n`, `\t`, `%d`, `%f`).

```cfn
output `Line1\nLine2`;  
output `Value: %d`, x;  
```

---

## 2. Syntax & Grammar

### 2.1 Program Structure

```bnf
Program ::= Statement*
```

- Statements execute linearly from top to bottom.

### 2.2 Statements

| Statement | Syntax                                                                     | Example                 |
| --------- | -------------------------------------------------------------------------- | ----------------------- |
| Variable  | `DTYPE WORD ASSN (Expression INPUT)`                                       | `Int x <- 5;`           |
| Input     | `INPUT WORD SEMI`                                                          | `Str name <- input;`    |
| Output    | `OUTPUT STR [COMM WORD*] SEMI`                                             | `output Result: %d, x;` |
| Loop      | `LOOP Expression COMM OCRL Program CCRL`                                   | `loop x > 0, { ... }`   |
| If        | `IF Expression COMM OCRL Program CCRL [ELSE OCRL Program CCRL]`            | `if x = 1, { ... }`     |
| Case      | `CASE Expression COMM OCRL Program CCRL [ELSECASE COMM OCRL Program CCRL]` | `case x, { ... }`       |
| Exit      | `EXIT NAT SEMI`                                                            | `exit 0;`               |

---

## 3. Type System

### 3.1 Data Types

| Type | Description    | Examples       | Coercion Rules     |
| ---- | -------------- | -------------- | ------------------ |
| Int  | Integer        | `42`, `-3`     | → Flt in mixed ops |
| Flt  | Floating-point | `3.14`, `-0.1` | —                  |
| Str  | String         | `` `hi` ``     | None               |

- **Implicit Conversions:** Only between Int and Flt (e.g., `Int 5 + Flt 0.1 → Flt 5.1`)

### 3.2 Variables

- **Scope:** Global, visible from point of declaration onward.

```cfn
output `x=`, x;  ; Error if x not yet declared  
Int x <- 1;      ; x visible hereafter  
```

- **Initialization:** Required (via `<-`).

---

## 4. Semantics

### 4.1 Expressions

- **Precedence:** Follows C rules (e.g., `*` before `+`).
- **Short-Circuit:** `and` / `or` behave like C’s `&&` / `||`.

### 4.2 Control Flow

| Construct | Behavior                         | C Equivalent                                    |
| --------- | -------------------------------- | ----------------------------------------------- |
| If-Else   | else binds to nearest if         | `if (...) { if (...) {...} else {...} }`        |
| Case      | Sequential checks (like else if) | `if (...) {...} else if (...) {...} else {...}` |
| Loop      | No break/continue                | `while (...) {...}`                             |

---

## 5. I/O & Execution

### 5.1 Input

- Always assigns to a variable:

```cfn
Int x <- input;  ; Valid  
input;           ; Invalid  
```

### 5.2 Output

- Requires string literal + optional format specifiers:

```cfn
output `Sum: %d`, (a + b);  ; Valid  
output a;                   ; Invalid (no string)  
```

### 5.3 Termination

- `exit NAT;` immediately halts with status code.

---

## 6. Error Handling

- **Compile-Time:** Syntax errors (e.g., missing `;`, type mismatches).
- **Runtime:** Handled by C (e.g., division by zero, invalid input).

---

## 7. Memory Model

- **Static Allocation Only:** No heap or dynamic structures.
- **Strings:** Fixed-size (determined at compile time).

---

## 8. Examples

### 8.1 Valid Program

```cfn
Int x <- input;  
if x > 0, {  
    output `Positive: %d`, x;  
}  
else, {  
    output `Non-positive`;  
}  
```

### 8.2 Invalid Program

```cfn
output x;           ; Error: Missing string  
Int y <- `hello`;   ; Error: Type mismatch  
```

---

## Appendix

- **A:** Full grammar (see [caffeine_syntax.txt](caffeine_syntax.txt).  
- **B:** Compiler (see [src/](src/)). 
