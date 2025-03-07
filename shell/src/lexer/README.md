# Lexer Component Documentation

## Overview

The lexer is responsible for breaking down the input command line into a sequence of tokens. Each token represents a meaningful unit in the shell syntax (words, operators, quotes, etc.).

## Token Types

```c
typedef enum {
    WHITE_SPACE,    // Spaces, tabs, etc.
    PIPE,           // |
    OR,            // ||
    STR,           // Single-quoted string
    DQSTR,         // Double-quoted string
    INPUT,         // <
    APPEND,        // >>
    HEREDOC,       // <<
    HEREDOC_TAB,   // <<-
    OUTPUT,        // >
    WORD,          // Regular words/commands
    AND,           // &&
    LPREN,         // (
    RPREN,         // )
    NEW_LINE       // End of input
} t_token_type;
```

## Data Structures

### Token Structure
```c
typedef struct s_token {
    t_token_type     type;           // Type of token
    char            *value;          // Actual content
    int             len;            // Length of token
    bool            to_expand;      // Needs expansion?
    struct s_token  *nospace_next;  // Connected tokens without space
    struct s_token  *next;         // Next token in sequence
} t_token;
```

## Workflow

1. **Initialization**: The lexer starts by creating an empty token list.

2. **Token Recognition**:
   - The lexer reads the input character by character
   - For each position, it determines the type of token starting at that position
   - It calculates the length of the token based on its type
   - The token value is extracted from the input string

3. **Token Processing**:
   ```c
   while (command_line[index]) {
       space = tok.type == WHITE_SPACE;
       get_token(command_line + index, &tok);
       if (!check_token(&tok, command_line, (int [2]){index, space}, tokens))
           return false;
       index += tok.len;
   }
   ```

4. **Token Linking**:
   - Tokens are linked in a main list using the `next` pointer
   - Adjacent tokens without spaces are linked using `nospace_next` pointer
   - This creates a two-dimensional token structure

## Key Functions

### get_token
```c
void get_token(char *where, t_token *tok)
```
Determines the token type and length at the current position.

### token_type
```c
t_token_type token_type(char *str)
```
Identifies the type of token based on the starting character(s).

### token_len
```c
int token_len(t_token_type type, char *str)
```
Calculates the length of the token based on its type and content.

### check_expanding
```c
bool check_expanding(char *str, t_token_type type)
```
Determines if a token needs expansion (variables, wildcards).

## Special Cases

1. **Quoted Strings**:
   - Single quotes (`'`): No expansion
   - Double quotes (`"`): Variable expansion allowed
   - Unclosed quotes are treated as errors

2. **Heredoc Variants**:
   - `<<`: Regular heredoc
   - `<<-`: Heredoc with tab stripping

3. **Connected Tokens**:
   - Tokens without intervening spaces are connected via `nospace_next`
   - Example: `echo"hello"` becomes `echo -> "hello"`

## Error Handling

1. **Syntax Errors**:
   - Invalid operators (single `&`)
   - Unclosed quotes
   - Missing command separators

2. **Memory Management**:
   - Proper allocation/deallocation of token structures
   - Cleanup on error conditions

## Examples

### Basic Command
Input: `echo hello world`
```
Token List:
[WORD, "echo"] -> [WHITE_SPACE] -> [WORD, "hello"] -> [WHITE_SPACE] -> [WORD, "world"] -> [NEW_LINE]
```

### Quoted String
Input: `echo "Hello 'World'"`
```
Token List:
[WORD, "echo"] -> [WHITE_SPACE] -> [DQSTR, "Hello 'World'"] -> [NEW_LINE]
```

### Operators
Input: `ls | grep file > output.txt`
```
Token List:
[WORD, "ls"] -> [WHITE_SPACE] -> [PIPE] -> [WHITE_SPACE] -> [WORD, "grep"] -> 
[WHITE_SPACE] -> [WORD, "file"] -> [WHITE_SPACE] -> [OUTPUT] -> [WHITE_SPACE] -> 
[WORD, "output.txt"] -> [NEW_LINE]
```

### Connected Tokens
Input: `echo"hello"world`
```
Token List:
[WORD, "echo"] -> [DQSTR, "hello"] -> [WORD, "world"] -> [NEW_LINE]
nospace_next:    echo -> "hello" -> world
```

## Usage Guidelines

1. Always initialize the token list to NULL before starting lexical analysis
2. Handle memory cleanup using `free_tok_lst`
3. Check return values for error conditions
4. Process tokens sequentially for parsing
