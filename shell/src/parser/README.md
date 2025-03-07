# Parser Component Documentation

## Overview

The parser component implements a recursive descent parser that transforms a stream of tokens into an Abstract Syntax Tree (AST). It handles shell grammar including:
- Command execution
- Pipelines
- Redirections
- Subshells
- Logical operators (AND, OR)

## Grammar Rules

```ebnf
command     → pipeline (("&&" | "||") pipeline)*
pipeline    → redirection ("|" redirection)*
redirection → term*
term        → WORD | STRING | DQSTRING | redirection_op WORD
redirection_op → ">" | ">>" | "<" | "<<" | "<<-"
```

## Components

### 1. Parser Entry Point

```c
bool parser(t_token *tokens, t_ast_cmd **tree)
```

Main parsing function that:
- Validates token stream
- Initiates recursive descent parsing
- Handles heredoc processing
- Returns AST or error status

### 2. AST Node Types

```c
typedef enum {
    P_PIPE,    // Pipeline
    P_AND,     // Logical AND
    P_OR,      // Logical OR
    P_EXEC,    // Command execution
    P_REDIR,   // Redirection
    P_SUBSH    // Subshell
} t_node_type;
```

### 3. Node Structures

#### Binary Node (Pipe, AND, OR)
```c
typedef struct s_ast_binary {
    t_node_type type;    // P_PIPE, P_AND, or P_OR
    t_ast_cmd *left;     // Left command
    t_ast_cmd *right;    // Right command
} t_ast_binary;
```

#### Execution Node
```c
typedef struct s_ast_exec {
    t_node_type type;    // P_EXEC
    t_token *argv_tok;   // Command arguments
} t_ast_exec;
```

#### Redirection Node
```c
typedef struct s_ast_redir {
    t_node_type type;       // P_REDIR
    t_token_type direction; // INPUT, OUTPUT, APPEND, etc.
    t_token *file_tok;      // Redirection file/target
    int mode;               // File open mode
    int fd;                 // File descriptor
    t_ast_cmd *cmd;        // Command to redirect
} t_ast_redir;
```

## Parsing Process

### 1. Command Parsing
```c
t_ast_cmd *parse_cmd(t_token **current)
```
- Handles top-level command parsing
- Processes logical operators (&&, ||)
- Returns a binary tree of commands

### 2. Pipeline Parsing
```c
t_ast_cmd *parse_pipe(t_token **current)
```
- Processes pipe operations
- Connects commands with P_PIPE nodes
- Ensures proper command chaining

### 3. Redirection Parsing
```c
t_ast_cmd *parse_redir(t_token **current)
```
- Handles input/output redirections
- Processes heredoc syntax
- Manages file descriptor assignments

### 4. Term Parsing
```c
bool parse_term(t_token **current, t_ast_cmd *sub_sh,
    t_token **exe_lst, t_ast_redir **redir_lst)
```
- Processes individual command terms
- Handles word tokens and redirections
- Builds command argument lists

## Examples

### Simple Command
Input: `ls -l`
```
AST Structure:
P_EXEC
└── argv_tok: ["ls", "-l"]
```

### Pipeline
Input: `echo hello | grep e`
```
AST Structure:
P_PIPE
├── left: P_EXEC ["echo", "hello"]
└── right: P_EXEC ["grep", "e"]
```

### Complex Command
Input: `(ls -l) > output.txt && cat input.txt`
```
AST Structure:
P_AND
├── left: P_REDIR
│   ├── direction: OUTPUT
│   ├── file: "output.txt"
│   └── cmd: P_SUBSH
│       └── cmd: P_EXEC ["ls", "-l"]
└── right: P_EXEC ["cat", "input.txt"]
```

## Error Handling

1. **Syntax Errors**
   - Unclosed parentheses
   - Missing redirection targets
   - Invalid operator combinations

2. **Resource Limits**
   - Maximum heredoc count (16)
   - Memory allocation failures
   - File descriptor limits

3. **Recovery Strategies**
   - Cleanup on parse failure
   - Resource deallocation
   - Error reporting

## Memory Management

### Node Creation
```c
t_ast_cmd *binary_node(t_node_type type, t_ast_cmd *left, t_ast_cmd *right);
t_ast_cmd *exec_node(t_token *argv_tok);
t_ast_cmd *redir_node(t_token_type direction, t_token *file_tok,
    int mode_fd[2], t_ast_cmd *cmd);
t_ast_cmd *subsh_node(t_ast_cmd *cmd);
```

### Cleanup Functions
```c
void free_ast(t_ast_cmd *cmd);
void free_redir(t_ast_cmd *sub_sh, t_ast_redir *redir_lst, t_token *exe_lst);
```

## Usage Guidelines

1. **Token Stream Requirements**
   - Must be NULL-terminated
   - Must end with NEW_LINE token
   - Tokens must be properly linked

2. **Error Handling**
   - Check return values
   - Handle memory allocation failures
   - Process syntax errors gracefully

3. **AST Navigation**
   - Use type checking for node traversal
   - Handle all node types
   - Follow proper cleanup procedures
