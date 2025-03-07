# Here Document (Heredoc) Component Documentation

## Overview

The heredoc component implements shell's here-document functionality, allowing multiline input redirection until a specified delimiter is encountered. It supports:
- Variable expansion
- Tab stripping
- Signal handling
- Temporary file management
- Quote handling in delimiters

## Core Features

### 1. Here Document Processing

```c
char *handle_heredoc(char *delim, t_heredoc_opts opts)
```

Manages the heredoc input process:
- Creates temporary file
- Reads input line by line
- Processes delimiter matching
- Handles expansions
- Manages signal interruption

### 2. Options Structure

```c
typedef struct s_heredoc_opts {
    bool expandable;    // Whether to expand variables
    bool strip_tabs;    // Whether to strip leading tabs (<<-)
} t_heredoc_opts;
```

## Implementation Details

### 1. Temporary File Management

```c
char *ft_mktmp(void)
```

- Creates unique temporary files
- Uses /tmp directory
- Ensures secure file permissions (0600)
- Automatically cleans up files
- Handles name collisions

### 2. Input Processing

```c
bool handle_heredoc_line(char *line, char *delim, int fd, t_heredoc_opts opts)
```

Features:
- Line-by-line reading using readline
- Delimiter matching
- Variable expansion if enabled
- Tab stripping for `<<-` operator
- Signal handling for interruption

## Examples

### 1. Basic Heredoc

```bash
# Input
cat << EOF
hello
world
EOF

# Process:
1. Creates temp file: /tmp/Minishell_HEREDOC_X
2. Reads until "EOF" is matched exactly
3. Writes content to temp file
4. Replaces heredoc with input redirection
```

### 2. Variable Expansion

```bash
# Input
cat << EOF
User: $USER
Path: $PATH
EOF

# Process:
1. Each line is processed for variables
2. Environment variables are expanded
3. Result is written to temp file
```

### 3. Tab Stripping (<<-)

```bash
cat <<- END
        indented
    not stripped
        END

# Process:
1. Leading tabs are stripped from each line
2. Delimiter must also be tab-indented
3. Only tabs are stripped, not spaces
```

### 4. Quoted Delimiters

```bash
# Input
cat << "EOF"
$USER
EOF

# Process:
1. Quotes are stripped from delimiter
2. Variables are not expanded (quoted delimiter)
3. Content is written literally
```

## Signal Handling

### 1. SIGINT (Ctrl+C) Management

```c
void heredoc_sigint_handler(int sig)
```

- Sets global signal flag
- Restores terminal state
- Cleans up temporary files
- Sets appropriate exit status (130)

### 2. Terminal State Management

```c
void handle_heredoc_signal(void)
```

- Restores terminal input mode
- Reconnects to /dev/tty if needed
- Handles interrupted input gracefully

## Error Handling

### 1. File Operations

```c
int init_heredoc(char *delim, char **tmp_file, int *fd)
```

- Validates delimiter
- Handles file creation errors
- Sets appropriate permissions
- Reports system errors

### 2. Memory Management

- Proper cleanup of temporary files
- Handling of allocation failures
- Resource deallocation on interruption

## AST Integration

### 1. Tree Processing

```c
int process_heredoc_tree(t_ast_cmd *tree)
```

- Recursively processes heredocs in AST
- Handles nested heredocs
- Manages redirection nodes
- Updates tokens after processing

### 2. Token Patching

```c
int patch_token(t_ast_redir *tree)
```

- Updates token information
- Converts heredoc to input redirection
- Manages file descriptors
- Handles quote stripping

## Usage Guidelines

1. **Delimiter Handling**
   - Use quotes to prevent variable expansion
   - Ensure exact delimiter matching
   - Handle empty delimiters

2. **Signal Management**
   - Always restore signal handlers
   - Clean up temporary files
   - Handle terminal state

3. **Resource Limits**
   - Check heredoc count (max 16)
   - Monitor file descriptor usage
   - Handle temp file space

4. **Security Considerations**
   - Use secure file permissions
   - Clean up temporary files
   - Validate file operations
