# Builtin Commands Documentation

## Overview
This directory contains implementations of shell builtin commands including cd, echo, export, and other standard shell builtins. Each command follows POSIX specifications while handling edge cases and providing detailed error reporting.

## CD Implementation

### Core Function (`ft_cd.c`)

#### `ft_cd(char *arg, t_env *env)`
- **Purpose**: Changes current working directory
- **Implementation Details**:
  1. No argument handling:
     - Looks up HOME in environment
     - Falls back with error if HOME not set
  2. With argument:
     - Direct change to specified directory
  3. Returns status (0 for success, 1 for error)

### CD Helpers (`ft_cd_helpers.c`, `ft_cd_helpers_2.c`, `ft_cd_helpers_3.c`)

#### `change_directory(char *dir)`
- **Purpose**: Core directory changing logic
- **Implementation Details**:
  1. Directory validation
  2. Path resolution steps:
     - Handles absolute paths
     - Resolves relative paths
     - Processes .. and . components
  3. Error handling:
     - Permission issues
     - Non-existent directories
     - Path resolution failures

#### `structure_path(char *curr_dir, char *dir)`
- **Purpose**: Constructs proper path from current directory and target
- **Implementation Details**:
  1. Special case handling:
     - "." directory
     - Absolute paths
     - Relative paths
  2. ".." resolution with dot_dot handler
  3. Path normalization

#### Path Manipulation Functions
- `join_paths`: Combines directory components
- `handle_dot_dot_path`: Processes .. in paths
- `contruct_path`: Builds final path string
- `trim_path`: Removes redundant components

## Echo Implementation (`ft_echo.c`)

#### `ft_echo(int argc, char **args)`
- **Purpose**: Prints arguments with optional newline
- **Implementation Details**:
  1. Option parsing:
     - Handles -n flag
     - Multiple -n flags supported
  2. Argument printing:
     - Space handling between arguments
     - Newline control
  3. Return value handling

Helper Functions:
- `is_option`: Validates -n flag format
- `consist_of`: Checks character repetition
- `print_args`: Handles argument output

Example Usage:
```bash
echo Hello World     -> "Hello World\n"
echo -n Hello World  -> "Hello World" (no newline)
echo -nnn Hello      -> "Hello" (no newline)
```

## Export Implementation

### Core Function (`ft_export.c`)

#### `ft_export(int argc, char **argv, t_env **env)`
- **Purpose**: Sets/modifies environment variables
- **Implementation Details**:
  1. No arguments: prints all variables
  2. With arguments:
     - Validates variable names
     - Handles value assignment
     - Supports += concatenation
  3. Error handling for invalid identifiers

### Export Helpers (`ft_export_helpers.c`, `ft_export_helpers_2.c`)

#### Variable Management
- `print_exports`: Formats and displays variables
- `validate_arg`: Checks variable name validity
- `handle_concate_case`: Handles += operator

#### Validation Functions
- `validate_var_name`: Checks variable name format
- `ft_is_alphanum_underscore`: Validates character set
- `is_bad_env_name_start`: Validates first character

Example Usage:
```bash
export PATH=/new/path     # Sets PATH
export PATH+=":/add/this" # Appends to PATH
export                    # Shows all variables
```

## Error Handling
All builtin commands follow these error handling principles:
1. Proper exit status codes:
   - 0 for success
   - 1 for general errors
   - 2 for usage errors
2. Descriptive error messages
3. Proper cleanup on failure
4. Environment consistency maintenance

## Memory Management
- Careful allocation/deallocation in path processing
- Proper string duplication and freeing
- Protection against memory leaks in error cases
- Use of shared utility functions for common operations

## Input Validation
Each builtin implements thorough input validation:
1. Argument count checking
2. String format validation
3. Path component validation
4. Environment variable name validation

## Environment Integration
- Consistent environment variable handling
- Path resolution with environment context
- Current directory state management
- Working directory synchronization
