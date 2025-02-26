# Lexer Documentation

## Overview
The lexer is responsible for tokenizing input command lines in the Minishell project. It breaks down the input into meaningful tokens that can be processed by the parser.

## Core Functions

### lexer.c

#### `get_token(char *where, t_token *tok)`
- **Purpose**: Determines the type and length of the next token in the input string
- **Parameters**:
  - `where`: Pointer to the current position in the command line
  - `tok`: Token structure to store the token information
- **Returns**: void
- **Implementation Details**:
  1. First calls `token_type()` to identify what kind of token starts at the current position
  2. Then calls `token_len()` to determine how long this token is
  3. Updates the token structure with both pieces of information
  4. Handles special cases like double characters (||, &&, >>, <<)

For example, if `where` points to "echo hello":
  - First call identifies 'e' as start of a WORD token
  - Then measures length until next special character or space (4 in this case)
  - Token becomes {type: WORD, len: 4}

#### `check_token(t_token *tok, char *cmd_line, int index_space[2], t_token **tokens)`
- **Purpose**: Validates and processes a token
- **Parameters**:
  - `tok`: Current token being processed
  - `cmd_line`: Original command line string
  - `index_space`: Array containing [current_index, is_space_before]
  - `tokens`: Pointer to token list
- **Returns**: bool (true if token is valid, false otherwise)
- **Error Cases**:
  - Invalid token type (type < 0)
  - Unclosed quotes (length 1 for quote tokens)
  - Memory allocation failure
- **Implementation Details**:
  1. First validates token type isn't negative (invalid token)
  2. Skips further processing for whitespace tokens
  3. Checks for unclosed quotes by examining token length
  4. Creates substring of the token value using index_space[0] and tok->len
  5. Adds token to the list using add_token() with spacing info
  6. Handles memory management for token values

For example with `echo "hello`:
  - Token type DQSTR is valid
  - Length 1 indicates unclosed quote
  - Returns false and sets error

#### `lexer(char *command_line, t_token **tokens)`
- **Purpose**: Main lexer function that tokenizes the entire command line
- **Parameters**:
  - `command_line`: Input string to tokenize
  - `tokens`: Pointer to store the resulting token list
- **Returns**: bool (true if successful, false if errors occurred)
- **Implementation Details**:
  1. Initializes an empty token list and tracking variables:
     - index: current position in command line
     - space: tracks if previous token was whitespace
     - tok: current token being processed
  2. Main processing loop:
     - Updates space flag based on previous token
     - Calls get_token() to identify next token
     - Calls check_token() to validate and process token
     - Updates index by token length to move forward
  3. Error handling:
     - Returns false if command_line is NULL
     - Returns false and frees tokens if check_token fails
  4. Adds final NEW_LINE token before returning
  
For example with `echo "hello world"`:
```
1. Start: index=0, space=true
2. First token:
   - get_token identifies WORD "echo"
   - check_token validates and adds to list
   - index += 4
3. Space token:
   - get_token identifies WHITE_SPACE
   - check_token processes (space=true)
   - index += 1
4. Quote token:
   - get_token identifies DQSTR
   - check_token validates quotes and content
   - index += 13
5. Add NEW_LINE token
6. Return token list
```

## Helper Functions

### lexer_helpers.c

#### `new_token(t_token_type type, char *value, int len)`
- **Purpose**: Creates a new token structure
- **Parameters**:
  - `type`: Type of token
  - `value`: String value of token
  - `len`: Length of token
- **Returns**: t_token* (new token or NULL if allocation fails)

#### `token_type(char *str)`
- **Purpose**: Determines the type of token based on first characters
- **Parameters**:
  - `str`: String to analyze
- **Returns**: t_token_type enum value
- **Supported Types**:
  - WHITE_SPACE: spaces or tabs
  - PIPE, PIPE_PIPE (|, ||): pipe operators
  - STR (single quotes): string literals
  - DQSTR (double quotes): expandable strings
  - OUTPUT, APPEND (>, >>): output redirection
  - INPUT, HEREDOC (<, <<): input redirection
  - LPREN, RPREN (parentheses): subshell grouping
  - AND (&&): logical AND
  - WORD (default): command names, arguments
- **Implementation Details**:
  1. Uses first character to identify token type
  2. For operators that can be single or double (like |, >), checks next character
  3. Special handling for & to only allow && (single & is error)
  4. Returns WORD type if no other type matches
  
For example:
```c
// Different inputs and resulting types:
"|"    -> PIPE
"||"   -> PIPE_PIPE
"'"    -> STR
"\""   -> DQSTR
">"    -> OUTPUT
">>"   -> APPEND
"<"    -> INPUT
"<<"   -> HEREDOC
"("    -> LPREN
")"    -> RPREN
"&&"   -> AND
"&"    -> -1 (error)
"echo"  -> WORD
```

#### `token_len(t_token_type type, char *str)`
- **Purpose**: Calculates length of token based on its type
- **Parameters**:
  - `type`: Type of token
  - `str`: String to analyze
- **Returns**: int (length of token)

### lexer_helpers_2.c

#### `valid_env_char(char c)`
- **Purpose**: Checks if character is valid in environment variable names
- **Parameters**:
  - `c`: Character to check
- **Returns**: bool

#### `check_expanding(char *str, t_token_type type)`
- **Purpose**: Determines if token needs expansion (variables or wildcards)
- **Parameters**:
  - `str`: Token string
  - `type`: Token type
- **Returns**: bool

#### `free_tok_lst(t_token *tok)`
- **Purpose**: Frees token list and associated memory
- **Parameters**:
  - `tok`: Head of token list
- **Returns**: void

## Token List Management (token_lists.c)

#### `ft_toklast(t_token *lst)`
- **Purpose**: Finds last token in list
- **Returns**: t_token* (last token)

#### `ft_tokadd_back(t_token **lst, t_token *new_tok)`
- **Purpose**: Adds token to end of list
- **Returns**: void

#### `add_to_sublist(t_token **list, t_token *new_tok)`
- **Purpose**: Adds token to nospace_next sublist
- **Returns**: void

#### `add_token(t_token **tokens, t_token *token, bool is_space)`
- **Purpose**: Smart token addition based on spacing rules
- **Returns**: bool

## Error Handling (errors/error_msgs.c)

#### `tok_error(char err)`
- **Purpose**: Reports unexpected token error
- **Sets Exit Status**: 258

#### `unclosed_error(char err)`
- **Purpose**: Reports unclosed quote error
- **Sets Exit Status**: 258

#### `syntax_error(char *err)`
- **Purpose**: Reports syntax error
- **Sets Exit Status**: 258

#### `print_err(char *preced, int msg_code)`
- **Purpose**: Generic error message printer
- **Message Codes**:
  - 0: Use perror
  - -1: Command not found (status 127)
  - -69: Is a directory (status 126)
  - -2: Allocation error (status 255)
  - -3: $PATH var unset (status 127)
  - -4: Ambiguous redirect (status 1)
  - -5: No such file or directory (status 1)
  - -6: Numeric argument required (status 255)

## Exit Status Management (exit_status/exit_status.c)

#### `exit_status(unsigned int action)`
- **Purpose**: Manages global exit status value
- **Special Action**: GET_EXIT to retrieve current status

#### `set_exit_status(unsigned int status)`
- **Purpose**: Sets exit status

#### `get_exit_status(void)`
- **Purpose**: Retrieves current exit status
