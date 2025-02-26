# Here Document (Heredoc) Implementation Documentation

## Overview
The heredoc system implements bash-style here documents, allowing for multi-line input with variable expansion control and delimiter matching. It uses temporary files for storing the document content and handles signals appropriately.

## Core Functions

### Main Heredoc Processing (`heredoc.c`)

#### `handle_heredoc(char *delim, bool expandable)`
- **Purpose**: Main function for handling heredoc input
- **Implementation Details**:
  1. Initialization:
     - Creates temporary file
     - Sets up signal handlers
  2. Input processing loop:
     - Reads lines with readline
     - Processes each line
     - Checks for delimiter
  3. Cleanup:
     - Closes file descriptors
     - Returns temporary file path

Example usage:
```bash
cat << EOF
line1
line2
EOF
```

#### `ft_mktmp(void)`
- **Purpose**: Creates unique temporary file for heredoc content
- **Implementation Details**:
  1. File naming:
     - Uses /tmp/Minishell_HEREDOC_[number]
     - Increments number until unique
  2. Security:
     - Sets 0600 permissions
     - Immediately unlinks file
     - Keeps file descriptor open
  3. Error handling:
     - Checks for creation failures
     - Handles permission issues

#### `ft_heredoc(t_ast_cmd *tree)`
- **Purpose**: Processes heredocs in AST
- **Implementation Details**:
  1. Tree traversal:
     - Recursively processes all nodes
     - Handles binary nodes (AND, OR, PIPE)
     - Processes redirections
  2. Signal handling:
     - Manages interruption signals
     - Sets appropriate exit status

### Helper Functions (`heredoc_helpers.c`)

#### `handle_heredoc_line(char *line, char *delim, int fd, bool expandable)`
- **Purpose**: Processes individual heredoc lines
- **Implementation Details**:
  1. Line processing:
     - Checks for delimiter match
     - Handles variable expansion if allowed
     - Writes to temporary file
  2. History management:
     - Adds non-empty lines to history
  3. Memory management:
     - Proper cleanup of allocated strings

Example line processing:
```
Input: "Hello $USER"
1. Check if line is delimiter -> No
2. If expandable:
   - Expands to "Hello username"
3. Writes to temp file
4. Adds to history
```

#### `init_heredoc(char *delim, char **tmp_file, int *fd)`
- **Purpose**: Initializes heredoc processing
- **Implementation Details**:
  1. Validation:
     - Checks delimiter
     - Creates temporary file
  2. File setup:
     - Opens with proper modes
     - Sets permissions
  3. Error handling:
     - Reports file creation errors
     - Sets appropriate exit status

#### `is_expandable(t_token *tok)`
- **Purpose**: Determines if heredoc content allows expansion
- **Implementation Details**:
  1. Token analysis:
     - Checks quote types
     - Handles token sequences
  2. Rules:
     - No expansion if delimiter is quoted
     - Default to expandable

## Signal Handling
- SIGINT (Ctrl+C) management
- Proper cleanup on interruption
- Status code propagation

## File Management
1. Temporary File Creation:
   - Unique naming scheme
   - Proper permissions
   - Automatic cleanup

2. File Descriptor Handling:
   - Safe opening/closing
   - Permission management
   - Error checking

## Variable Expansion
1. Expansion Control:
   - Based on delimiter quoting
   - Environment variable resolution
   - Special variable handling ($?)

2. Quote Processing:
   - Single quotes prevent expansion
   - Double quotes allow expansion
   - Unquoted allows expansion

## AST Integration
1. Tree Processing:
   - Recursive node traversal
   - Redirection handling
   - Command integration

2. Token Patching:
   - Updates redirection nodes
   - Manages file paths
   - Handles expansion flags

## Error Handling
- File creation failures
- Permission issues
- Signal interruptions
- Memory allocation errors

## Memory Management
1. Resource Cleanup:
   - Temporary file deletion
   - File descriptor closure
   - String deallocation

2. History Management:
   - Line addition
   - History cleanup

## Security Considerations
- Temporary file permissions
- Path handling
- Immediate file unlinking
- Process isolation

## Signal Behavior
1. During Input:
   - SIGINT handling
   - Clean temporary file cleanup
   - Status code propagation

2. After Processing:
   - Restore original handlers
   - Proper exit status management
