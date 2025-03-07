# Error Handling Component Documentation

## Overview

The error handling component provides consistent error reporting and messaging for the shell. It handles:
- Syntax errors
- Token errors
- Quote errors
- Command errors
- File system errors
- Memory allocation errors

## Core Functions

### 1. Token Error Handling

```c
void tok_error(char err)
```

Reports unexpected token errors:
- Invalid characters
- Unexpected symbols
- Sets exit status to 2
- Writes to stderr

Example:
```bash
# Input: |ls
Output: Minishell: unexpected token `|'
```

### 2. Quote Error Handling

```c
void unclosed_error(char err)
```

Reports unclosed quote errors:
- Single quotes (`'`)
- Double quotes (`"`)
- Sets exit status to 2
- Writes to stderr

Example:
```bash
# Input: echo "hello
Output: Minishell: unclosed quote `"'
```

### 3. Syntax Error Handling

```c
void syntax_error(char *err)
```

Reports syntax errors:
- Invalid command structure
- Unexpected tokens
- Sets exit status to 2
- Writes to stderr

Example:
```bash
# Input: ls >>
Output: Minishell: syntax error near unexpected token `newline'
```

### 4. General Error Reporting

```c
int print_err(char *preced, int msg_code)
```

Error Code Handling:
- `-1`: Command not found (status 127)
- `-2`: Allocation error (status 255)
- `-3`: PATH variable unset (status 127)
- `-4`: Ambiguous redirect (status 1)
- `-5`: No such file/directory (status 1)
- `-6`: Numeric argument required (status 255)
- `-69`: Is a directory (status 126)
- `0`: System error (perror)

## Implementation Details

### 1. Error Message Building

```c
t_strbuilder *sb = stringbuilder();
sb_append(sb_append(sb, "Minishell: "), preced);
```

Features:
- Dynamic message construction
- Prefix standardization
- Memory management
- String builder utilization

### 2. Error Categories

1. **Command Errors**
   ```c
   "command not found"
   "Is a directory"
   "$PATH var unset"
   ```

2. **Resource Errors**
   ```c
   "allocation error"
   "No such file or directory"
   ```

3. **Syntax Errors**
   ```c
   "unexpected token"
   "unclosed quote"
   "ambiguous redirect"
   ```

## Usage Guidelines

1. **Error Reporting**
   - Use appropriate error function
   - Set correct exit status
   - Include relevant context
   - Clean up resources

2. **Exit Status**
   - Set meaningful status codes
   - Follow shell conventions
   - Document status meanings
   - Handle status in parent

3. **Message Formatting**
   - Consistent prefix usage
   - Clear error descriptions
   - Include relevant details
   - Follow shell style

4. **Resource Management**
   - Free string builders
   - Handle allocation failures
   - Clean up on errors
   - Prevent memory leaks

## Examples

### 1. Command Not Found

```c
print_err("invalidcmd", -1);
// Output: Minishell: invalidcmd: command not found
// Status: 127
```

### 2. Directory Error

```c
print_err("dir_name", -69);
// Output: Minishell: dir_name: Is a directory
// Status: 126
```

### 3. Syntax Error

```c
syntax_error(">>");
// Output: Minishell: syntax error near unexpected token `>>'
// Status: 2
```

### 4. Quote Error

```c
unclosed_error('\'');
// Output: Minishell: unclosed quote `''
// Status: 2
```

## Error Table

| Code | Description | Exit Status |
|------|-------------|-------------|
| -1 | Command not found | 127 |
| -2 | Allocation error | 255 |
| -3 | PATH var unset | 127 |
| -4 | Ambiguous redirect | 1 |
| -5 | No such file/directory | 1 |
| -6 | Numeric argument required | 255 |
| -69 | Is a directory | 126 |
| 0 | System error (perror) | Varies |
