# Minishell Documentation

A custom implementation of a shell, similar to bash. This shell supports command execution, pipes, redirections, environment variables, and built-in commands.

## Requirements

- GCC compiler
- GNU Make
- readline library
- POSIX-compliant operating system (Linux/Unix)

## Dependencies

The project uses the following external libraries:
- `readline`: For command line editing and history
- `libft`: Custom C library (included in project)

## Installation

```bash
# Install readline library on Ubuntu/Debian
sudo apt-get install libreadline-dev

# Install readline library on macOS
brew install readline

# Clone the repository
git clone https://github.com/YoTi1412/minishell.git

# Change to project directory
cd minishell

# Compile the project
make

# Run the shell
./minishell
```

## Usage

The shell provides an interactive prompt where you can enter commands:

```bash
$ ./minishell
minishell$ ls -l
minishell$ echo Hello > file.txt
minishell$ cat < file.txt
minishell$ command1 | command2
minishell$ (cd dir && pwd)
```

Key features:
- Command execution with arguments
- Input/output redirections (>, >>, <)
- Here-documents (<<)
- Pipes (|)
- Environment variables ($PATH, etc.)
- Wildcards (*)
- Exit status ($?)
- Logical operators (&& and ||)
- Subshells with parentheses

## Table of Contents
1. [Overview](#overview)
   - [Execution Flow](#execution-flow)
2. [Data Structures](#data-structures)
   - [Token Structure](#token-structure)
   - [AST Node Types](#ast-node-types)
   - [Environment Variable Structure](#environment-variable-structure)
3. [Components](#components)
   - [Lexer](#1-lexer-srclexer)
   - [Parser](#2-parser-srcparser)
   - [Expander](#3-expander-srcexpander)
   - [Executor](#4-executor-srcexecution)
4. [Built-in Commands](#built-in-commands)
5. [Error Handling](#error-handling)
   - [Syntax Errors](#syntax-errors)
   - [Runtime Errors](#runtime-errors)
   - [Here-Document Handling](#here-document-handling)
6. [Special Features](#special-features)
   - [Here-Document Support](#here-document-support)
   - [Wildcard Expansion](#wildcard-expansion)
   - [Subshell Support](#subshell-support)
7. [Memory Management](#memory-management)
8. [Signal Handling](#signal-handling)
9. [Environment Variables](#environment-variables)

## Overview

The shell implements a complete command-line interface that follows standard Unix shell conventions. It provides an interactive environment for executing commands, managing processes, and handling shell operations.

### Execution Flow

The shell follows this execution flow:
1. Read input from user
2. Lexical analysis (tokenization)
3. Parsing (creating AST)
4. Expansion (variables, wildcards)
5. Execution

## Data Structures

### Token Structure
```c
typedef struct s_token {
    t_token_type type;     // Type of token
    char *value;           // Token content
    int len;              // Length of token
    bool to_expand;       // Whether token needs expansion
    struct s_token *next; // Next token in list
} t_token;
```

### AST Node Types
```c
typedef struct s_ast_cmd {
    t_node_type type;     // Type of AST node
} t_ast_cmd;

typedef struct s_ast_binary {
    t_node_type type;     // Node type (P_AND, P_OR, P_PIPE)
    t_ast_cmd *left;      // Left subtree
    t_ast_cmd *right;     // Right subtree
} t_ast_binary;

typedef struct s_ast_exec {
    t_node_type type;     // Node type (P_EXEC)
    t_token *argv_tok;    // Command arguments
} t_ast_exec;
```

### Environment Variable Structure
```c
typedef struct s_env {
    bool equal_sign;      // Has assignment
    char *key;           // Variable name
    char *value;         // Variable value
    struct s_env *prev;  // Previous entry
    struct s_env *next;  // Next entry
} t_env;
```

## Components

### 1. Lexer (`src/lexer/`)

The lexer breaks down the input command line into tokens, transforming raw input into a sequence of meaningful tokens. It handles:

- **Token Types**:
  - `WORD`: Regular command words
  - `STR`: Single-quoted strings
  - `DQSTR`: Double-quoted strings
  - `OUTPUT`, `APPEND`: Output redirections (> and >>)
  - `INPUT`: Input redirection (<)
  - `HEREDOC`: Here-document (<<)
  - `PIPE`: Command pipes (|)
  - `OR`, `AND`: Logical operators (|| and &&)
  - `LPREN`, `RPREN`: Parentheses
  - `WHITE_SPACE`, `NEW_LINE`: Spacing tokens

**Key Functions**:
- `lexer(char *command_line, t_token **tokens)`: Main tokenization function that processes the input command line and generates a linked list of tokens
- `check_token(t_token *this_tok, char *cmd_line, int index_space[2], t_token **tokens)`: Validates token syntax and adds it to the token list
- `get_token(char *where, t_token *tok)`: Extracts token value from the command line
- `token_type(char *str)`: Determines token type based on character patterns
- `token_len(t_token_type type, char *str)`: Calculates token length based on type
- `check_expanding(char *str, t_token_type type)`: Determines if a token needs variable expansion
- `valid_env_char(char c)`: Validates environment variable name characters

### 2. Parser (`src/parser/`)

Converts tokens into an Abstract Syntax Tree (AST) for execution. The parser implements a recursive descent algorithm to build a tree structure representing command relationships.

**Node Types**:
- `P_AND`, `P_OR`: Logical operators
- `P_PIPE`: Pipe operator
- `P_EXEC`: Command execution
- `P_REDIR`: Redirections
- `P_SUBSH`: Subshell commands

**Key Functions**:
- `parser(t_token *tokens, t_ast_cmd **tree)`: Main parsing function that builds the AST from token list
- `parse_cmd(t_token **current)`: Parses command sequences and builds command nodes
- `parse_pipe(t_token **current)`: Creates pipe nodes for command pipelines
- `parse_redir(t_token **current)`: Builds redirection nodes for I/O operations
- `parse_paren(t_token **current)`: Handles subshell expressions in parentheses
- `parse_term(t_token **current, t_ast_cmd *sub_sh, t_token **exe_lst, t_ast_redir **redir_lst)`: Parses individual command terms

### 3. Expander (`src/expander/`)

Handles dynamic expansion of various shell elements. The expander processes:
- Environment variables
- Wildcards
- Tilde (~)
- Special parameters ($?, $$)

**Key Functions**:
- `expand_args(t_token *tok_lst)`: Expands all arguments in a command
- `expand_wild_cards(t_str *argv_lst)`: Expands wildcard patterns to matching files
- `expand_env(char *to_expand, bool in_quote, bool ignore_env)`: Expands environment variables
- `expand_tilde(char *str)`: Expands tilde to home directory path
- `expand(t_token *tok, bool ignore_env)`: Main expansion function for tokens
- `handle_dollar_quoted(char *str)`: Processes quoted dollar signs
- `handle_env_var(t_env_var *env)`: Processes environment variable expansions

### 4. Executor (`src/execution/`)

Executes the parsed AST commands. The executor handles process creation, file descriptors, and command execution:
- Command execution
- Pipes
- Redirections
- Built-in commands
- Subshells

**Key Functions**:
- `executor(t_ast_cmd *tree, bool forked)`: Main execution function that traverses AST
- `execute_cmd(char **cmd, t_env *env)`: Executes external commands with PATH lookup
- `exec_pipe(t_ast_binary *tree, bool forked)`: Handles pipe execution by creating child processes
- `exec_redir(t_ast_redir *tree, bool forked)`: Manages file descriptor redirections
- `exec_subsh(t_ast_subsh *tree, bool forked)`: Executes commands in subshells
- `check_if_executable(char **cmd, char *exec, char **paths, t_env *env)`: Validates command executability
- `find_path(t_env *envp)`: Searches for command in PATH directories

## Built-in Commands

The shell implements several built-in commands that are handled internally without creating new processes:

### cd (Change Directory)
- Changes current directory
- Supports relative and absolute paths
- Updates PWD and OLDPWD environment variables
```bash
cd [directory]
```

### echo (Output Text)
- Displays text
- Supports -n flag to omit trailing newline
```bash
echo [-n] [text...]
```

### env (Environment Variables)
- Displays environment variables
```bash
env
```

### exit
- Exits the shell with optional status code
```bash
exit [status]
```

### export (Set Environment)
- Sets environment variables
- Displays all exported variables when no arguments
```bash
export [name[=value]...]
```

### pwd (Print Working Directory)
- Prints current working directory
```bash
pwd
```

### unset (Remove Variables)
- Removes environment variables
```bash
unset [name...]
```

## Error Handling

### Syntax Errors
- Unclosed quotes detection
- Invalid command syntax checking
- Missing command arguments validation
- Redirection syntax verification

**Key Functions**:
- `tok_error(char err)`: Handles token-related errors
- `unclosed_error(char err)`: Reports unclosed quotes or parentheses
- `syntax_error(char *err)`: Displays syntax error messages
- `print_err(char *preced, int msg_code)`: Formats and prints error messages

### Runtime Errors
- Command not found handling
- Permission denied cases
- File operation errors
- Pipeline failures
- Exit status management through `exit_status()` function

### Here-Document Handling
- Proper EOF detection
- Signal interruption handling
- Temporary file management
- Variable expansion control

## Special Features

### Here-Document Support
- Supports both expandable and literal here-documents
- Handles tab stripping with `HEREDOC_TAB` token
- Manages temporary files securely
- Supports nested here-documents

### Wildcard Expansion
- Implements glob pattern matching
- Handles `*` expansion for files
- Sorts matches alphabetically
- Preserves quotes in patterns

### Subshell Support
- Executes commands in isolated environment
- Manages nested subshells
- Handles environment inheritance
- Proper exit status propagation

## Memory Management

The shell implements careful memory management to prevent leaks:

### Allocation Functions
- `ft_malloc()`: Wrapper for memory allocation
- `ft_calloc()`: Allocates and zeroes memory
- `clean()`: Frees allocated memory

### Memory Cleanup
- AST nodes are freed recursively after execution
- Token lists are freed after parsing
- Environment variables are cleaned up on shell exit
- File descriptors are properly closed after redirection

## Signal Handling

The shell implements custom signal handling to provide a smooth user experience. It handles:
- `SIGINT` (Ctrl+C): Interrupts current command
- `SIGQUIT` (Ctrl+\): Ignored in interactive mode
- Custom handling for here-documents

**Key Functions**:
- `sigint_handler(int sig)`: Handles Ctrl+C interrupts by resetting the prompt
- `heredoc_sigint_handler(int sig)`: Special handling for here-document interrupts
- `handle_default_sig_handlers(int action)`: Sets up or resets signal handlers
- `install_default_sig_handlers()`: Initializes default signal handling

## Environment Variables

The shell maintains environment variables in a doubly-linked list structure for efficient access and modification:
- Supports variable expansion in commands
- Handles special variables (PATH, HOME, PWD, etc.)

**Key Functions**:
- `get_env_value(char *var)`: Retrieves the value of an environment variable
- `set_env_value(char *key, char *value, int equal_sign)`: Sets or updates environment variable
- `create_env(char **envp)`: Initializes environment from parent shell
- `handle_shlvl(void)`: Manages shell nesting level
- `search_in_env(t_env *env, char *key)`: Searches for variable in environment
- `edit_env(t_env *node, char *value, int equal_sign)`: Modifies existing environment entry

## Resources & References

### Books
- "The Linux Programming Interface" by Michael Kerrisk
  - Essential reference for system calls and POSIX programming
  - Particularly useful for process management and signals

- "Computer Systems: A Programmer's Perspective" by Bryant and O'Hallaron
  - Deep understanding of process creation and management
  - Memory management and system-level programming

- "Crafting a Compiler" by Charles N. Fischer
  - Insights into lexical analysis and parsing
  - Helpful for implementing the command parser

### Documentation
- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/)
  - Complete reference for bash features and behavior
  - Shell expansion rules and command execution

### Additional Resources
- [Advanced Linux Programming](https://mentorembedded.github.io/advancedlinuxprogramming/)
  - Process management and IPC
  - File operations and signal handling

- [The Open Group Base Specifications (POSIX)](https://pubs.opengroup.org/onlinepubs/9699919799/)
  - Standard shell command language specification
  - System interface definitions

- [Bash Reference Manual - Shell Syntax](https://www.gnu.org/software/bash/manual/html_node/Shell-Syntax.html)
  - Detailed explanation of shell syntax
  - Token recognition and parsing rules

---

This shell implementation provides a robust command-line interface with support for most common shell operations, making it a functional alternative to basic shell usage.
