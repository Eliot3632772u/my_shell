# Execution System Documentation

## Overview
The execution system is responsible for running commands and managing processes in the shell. It handles different types of command execution including simple commands, pipes, logical operators (AND/OR), redirections, and subshells.

## Core Execution (`executor.c`)

#### `executor(t_ast_cmd *tree, bool forked)`
- **Purpose**: Main execution function that handles all command types
- **Implementation Details**:
  1. Command type branching:
     - P_AND: Logical AND execution
     - P_OR: Logical OR execution
     - P_PIPE: Pipeline execution
     - P_EXEC: Simple command execution
     - P_REDIR: Redirection handling
     - P_SUBSH: Subshell execution
  2. Forked mode handling:
     - Manages process isolation
     - Handles exit status propagation

## Command Execution Types

### Pipeline Execution (`executor_helpers_3.c`)

#### `exec_pipe(t_ast_binary *tree, bool forked)`
- **Purpose**: Handles command pipelines (cmd1 | cmd2)
- **Implementation Details**:
  1. Pipe creation and error checking
  2. Process creation for each command:
     - Left side writes to pipe
     - Right side reads from pipe
  3. File descriptor management:
     - Proper dup2() usage
     - Cleanup of unused descriptors
  4. Exit status handling from both processes

Example pipeline execution:
```
ls | grep txt
1. Creates pipe
2. Forks for 'ls':
   - Redirects stdout to pipe write end
   - Executes ls
3. Forks for 'grep':
   - Redirects stdin from pipe read end
   - Executes grep
4. Parent waits for both processes
```

### Logical Operators (`executor_helpers_3.c`)

#### `exec_and(t_ast_binary *tree, bool forked)`
- **Purpose**: Executes AND (&&) operator
- **Implementation**:
  - Executes left side
  - Only executes right if left succeeds
  - Propagates exit status

#### `exec_or(t_ast_binary *tree, bool forked)`
- **Purpose**: Executes OR (||) operator
- **Implementation**:
  - Executes left side
  - Only executes right if left fails
  - Propagates exit status

### Redirection Handling (`executor_helpers_4.c`)

#### `exec_redir(t_ast_redir *tree, bool forked)`
- **Purpose**: Handles input/output redirections
- **Implementation Details**:
  1. File handling:
     - Opens file with appropriate modes
     - Handles permission checks
     - Creates files if needed
  2. Descriptor management:
     - Backs up original descriptors
     - Performs redirection with dup2
     - Restores descriptors after execution
  3. Error handling:
     - File access errors
     - Redirection failures

Example redirection process:
```
cmd > output.txt
1. Opens output.txt (create/truncate)
2. Backs up stdout
3. Redirects stdout to file
4. Executes command
5. Restores original stdout
```

### Simple Command Execution (`executor_helpers_4.c`)

#### `exec_exe(t_ast_exec *exe, bool forked)`
- **Purpose**: Executes simple commands
- **Implementation Details**:
  1. Argument processing:
     - Expands arguments (variables, wildcards)
     - Handles empty commands
  2. Builtin checking:
     - Checks if command is builtin
     - Executes builtin directly if found
  3. External command execution:
     - Forks new process
     - Searches PATH
     - Handles execution errors
  4. Exit status management

### Subshell Execution (`executor_helpers_4.c`)

#### `exec_subsh(t_ast_subsh *tree, bool forked)`
- **Purpose**: Handles commands in parentheses
- **Implementation Details**:
  1. Process isolation:
     - Creates new process
     - Independent execution environment
  2. Exit status handling:
     - Waits for completion
     - Propagates status to parent

## Helper Functions

### Path and Command Resolution (`executor_helpers.c`)

#### `find_path(t_env *envp)`
- **Purpose**: Locates command in PATH
- **Implementation**:
  - Searches PATH variable
  - Handles missing PATH
  - Validates executable permissions

#### `execute_cmd(char **cmd, t_env *env)`
- **Purpose**: Resolves and executes commands
- **Implementation**:
  1. Absolute path handling
  2. PATH search implementation
  3. Error code generation

### File and Permission Checks (`executor_helpers_2.c`)

#### `check_if_executable(char **cmd, char *exec, char **paths, t_env *env)`
- **Purpose**: Validates command executability
- **Checks**:
  - File existence
  - Execute permissions
  - Directory vs file

#### `check_file_tok(t_token *file_tok)`
- **Purpose**: Processes redirection files
- **Implementation**:
  - Expands variables
  - Handles ambiguous redirects
  - Validates file names

## Process Management

### Fork and Wait Functions

#### `ft_fork(void)`
- **Purpose**: Safe process creation
- **Implementation**:
  - Error checking
  - Resource cleanup
  - Exit status handling

#### `wait_and_exit_status(int pid)`
- **Purpose**: Process completion handling
- **Implementation**:
  - Signal handling
  - Status code extraction
  - Error propagation

## Error Handling
- Consistent error reporting
- Resource cleanup
- Exit status propagation
- Signal handling
- File descriptor management

## Memory Management
- Process cleanup
- File descriptor tracking
- Environment variable handling
- Command argument cleanup
