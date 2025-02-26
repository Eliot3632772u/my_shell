# Parser Documentation

## Overview
The parser implements a recursive descent parser that converts the token stream from the lexer into an Abstract Syntax Tree (AST). It handles shell command structures including pipes, redirections, logical operators, and subshells.

## Core Functions

### parser.c

#### `parser(t_token *tokens, t_ast_cmd **tree)`
- **Purpose**: Main parser function that converts tokens into an AST
- **Parameters**:
  - `tokens`: Input token stream
  - `tree`: Pointer to store resulting AST
- **Returns**: bool (true if successful, false if parsing error)
- **Error Cases**:
  - Syntax errors
  - Empty input
  - Invalid token sequences
- **Implementation Details**:
  1. Initializes tree to NULL
  2. Early return for empty input
  3. For empty command (just newline), frees tokens and returns success
  4. Main parsing sequence:
     - Calls parse_cmd() to build command tree
     - Validates parsing completed at newline
     - Sets up heredoc handling
  5. Error handling:
     - Returns false and frees tokens on NULL tree
     - Returns false on syntax errors with error message
     - Special handling for exit status 2

Example parsing "ls -l | grep foo":
```
1. Start with token list: [WORD(ls), WORD(-l), PIPE(|), WORD(grep), WORD(foo), NEW_LINE]
2. parse_cmd creates initial pipe node
3. Left side: exec node for "ls -l"
4. Right side: exec node for "grep foo"
5. Final AST structure:
   PIPE
   ├── EXEC("ls -l")
   └── EXEC("grep foo")
```

### Recursive Descent Parser (rdp.c)

#### `parse_cmd(t_token **current)`
- **Purpose**: Top-level parser function for command sequences
- **Handles**: Logical operators (AND, OR)
- **Returns**: t_ast_cmd* (command node or NULL on error)
- **Implementation Details**:
  1. Parses initial pipeline using parse_pipe()
  2. Enters loop to handle chains of AND/OR operators:
     - Checks for && or || operator
     - Creates appropriate binary node
     - Parses next pipeline as right child
  3. Error handling with tree cleanup
  4. Maintains operator precedence

Example parsing "ls && echo hi || echo bye":
```
1. First parses "ls" pipeline
2. Encounters &&, creates AND node
3. Parses "echo hi" pipeline
4. Encounters ||, creates OR node with previous AND as left child
5. Final AST structure:
   OR
   ├── AND
   │   ├── EXEC("ls")
   │   └── EXEC("echo hi")
   └── EXEC("echo bye")
```

#### `parse_pipe(t_token **current)`
- **Purpose**: Parses pipe sequences
- **Handles**: Command pipelines (|)
- **Returns**: t_ast_cmd* (pipe node or NULL on error)
- **Implementation Details**:
  1. Parses initial redirection using parse_redir()
  2. Enters loop for pipeline sequence:
     - Checks for pipe operator
     - Creates pipe node with current command as left child
     - Parses next redirection as right child
  3. Handles memory cleanup on errors
  4. Maintains left-to-right evaluation order

Example parsing "cat file | grep foo | wc -l":
```
1. First parses "cat file" redirection
2. Encounters |, creates first pipe node
3. Parses "grep foo" redirection
4. Encounters |, creates second pipe node
5. Final AST structure:
   PIPE
   ├── PIPE
   │   ├── EXEC("cat file")
   │   └── EXEC("grep foo")
   └── EXEC("wc -l")
```

#### `parse_redir(t_token **current)`
- **Purpose**: Parses redirections and simple commands
- **Handles**: 
  - Input/Output redirections (<, >)
  - Here-documents (<<)
  - Append redirections (>>)
  - Word sequences
- **Returns**: t_ast_cmd* (redirection node or NULL on error)
- **Implementation Details**:
  1. Handles potential subshell by checking for parentheses
  2. Main parsing loop:
     - Processes words and redirections using parse_term()
     - Builds list of executable arguments
     - Creates redirection nodes with proper file descriptors
  3. Special handling:
     - Creates exec node for word sequences
     - Links redirections in proper order
     - Handles both file and heredoc redirections
  4. Memory management for all components

Example parsing "echo hello > output.txt":
```
1. Parse "echo hello" as executable
2. Encounter >, create redirection node
3. Parse "output.txt" as file name
4. Final AST structure:
   REDIR(>)
   ├── FILE("output.txt")
   └── EXEC("echo hello")
```

#### `parse_paren(t_token **current)`
- **Purpose**: Parses subshell expressions
- **Handles**: Parenthesized command groups
- **Returns**: t_ast_cmd* (subshell node or NULL on error)

#### `parse_term(t_token **current, t_ast_cmd *sub_sh, t_token **exe_lst, t_ast_redir **redir_lst)`
- **Purpose**: Parses individual terms (words or redirections)
- **Parameters**:
  - `current`: Current token pointer
  - `sub_sh`: Subshell command if present
  - `exe_lst`: List of executable arguments
  - `redir_lst`: List of redirections
- **Returns**: bool (true if successful)

## AST Node Creation (ast.c)

#### `binary_node(t_node_type type, t_ast_cmd *left, t_ast_cmd *right)`
- **Purpose**: Creates a binary operator node (AND, OR, PIPE)
- **Returns**: t_ast_cmd* (binary node)

#### `exec_node(t_token *argv_tok)`
- **Purpose**: Creates a simple command execution node
- **Returns**: t_ast_cmd* (execution node)

#### `redir_node(t_token_type direction, t_token *file_tok, int mode_fd[2], t_ast_cmd *cmd)`
- **Purpose**: Creates a redirection node
- **Parameters**:
  - `direction`: Type of redirection
  - `file_tok`: File token
  - `mode_fd`: File mode and descriptor
  - `cmd`: Command to redirect
- **Returns**: t_ast_cmd* (redirection node)

#### `subsh_node(t_ast_cmd *cmd)`
- **Purpose**: Creates a subshell node
- **Returns**: t_ast_cmd* (subshell node)

## Helper Functions

### ast_helpers.c

#### `advance(t_token **current)`
- **Purpose**: Moves to next token in stream

#### `match(t_token *tok, t_token_type types[], int size)`
- **Purpose**: Checks if current token matches any of given types
- **Returns**: bool

#### `clone_tok(t_token *tok)`
- **Purpose**: Creates deep copy of token
- **Returns**: t_token* (cloned token)

#### `tok_to_redir(t_token *redir_ptr)`
- **Purpose**: Converts redirection token to AST node
- **Returns**: t_ast_redir* (redirection node)

### ast_helpers_2.c

#### `count_heredocs_in_tokens(t_token *tokens)`
- **Purpose**: Counts number of here-documents in token stream
- **Returns**: int (count of here-docs)

#### `free_ast(t_ast_cmd *tree)`
- **Purpose**: Recursively frees AST nodes
- **Handles**: All node types (binary, exec, redir, subshell)

#### `redir_file(t_token **current)`
- **Purpose**: Parses redirection file specifications
- **Returns**: t_ast_cmd* (redirection node)

#### `add_redir_node(t_ast_redir **lst, t_ast_cmd *node)`
- **Purpose**: Adds redirection node to list
- **Returns**: bool (true if successful)

#### `free_redir(t_ast_cmd *sub_sh, t_ast_redir *redir_lst, t_token *exe_lst)`
- **Purpose**: Frees all resources in redirection parsing

## AST Node Types
- `P_EXEC`: Simple command execution
- `P_REDIR`: Input/Output redirection
- `P_PIPE`: Pipeline
- `P_AND`: Logical AND
- `P_OR`: Logical OR
- `P_SUBSH`: Subshell execution

## Error Handling
- Syntax errors trigger error messages
- Memory allocation failures are handled
- Here-document limit (16) is enforced
- Invalid token sequences are detected
- Unclosed parentheses are caught
