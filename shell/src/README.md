# Minishell Implementation Documentation

## Overview
The main program implements a POSIX-compliant shell with support for command execution, pipelines, redirections, environment variables, and signal handling. It provides an interactive command-line interface with history support and terminal control.

## Core Functions

### Main Program Flow (`main.c`)

#### `main(int _, char **__, char **envp)`
- **Purpose**: Shell entry point and main loop
- **Implementation Details**:
  1. Initialization:
     - Sets up environment
     - Configures terminal
     - Installs signal handlers
  2. Main loop:
     - Displays prompt
     - Reads command line
     - Processes commands
  3. Exit handling:
     - Cleanup resources
     - Return final status

Example flow:
```
1. Initialize environment
2. Configure terminal
3. Loop:
   - Display "Minishell$ "
   - Read command
   - Process command
   - Update history
4. Exit with status
```

#### `run(char *command_line)`
- **Purpose**: Processes a single command line
- **Implementation Details**:
  1. Command processing pipeline:
     ```
     Input -> Lexer -> Parser -> Executor
     ```
  2. Steps:
     - Lexical analysis (tokens)
     - Syntax parsing (AST)
     - Command execution
  3. History management:
     - Adds non-empty commands
     - Maintains command history

Example processing:
```bash
Input: "ls -l | grep .txt"
1. Lexer creates tokens:
   [WORD(ls), WORD(-l), PIPE(|), WORD(grep), WORD(.txt)]
2. Parser builds AST:
   PIPE
   ├── EXEC("ls -l")
   └── EXEC("grep .txt")
3. Executor runs commands
```

### Terminal Management

#### `tty_attr(struct termios *attrs, int action)`
- **Purpose**: Manages terminal attributes
- **Implementation Details**:
  1. Actions:
     - ATTR_GET: Saves current attributes
     - ATTR_SET: Restores saved attributes
     - ATTR_CHG: Modifies specific flags
  2. Controls:
     - Input echo
     - Signal display
     - Line editing

#### `setup(char **envp, struct termios *attrs, ...)`
- **Purpose**: Initializes shell environment
- **Implementation Details**:
  1. Environment setup:
     - Creates environment table
     - Sets working directory
  2. Terminal configuration:
     - Disables signal catching
     - Configures line editing
  3. Signal handling:
     - Installs custom handlers
     - Sets up interrupt handling

## Program Components Integration

### 1. Command Processing Pipeline
```
           Lexical          Syntactic        Command
Input ---> Analysis ---> Parsing -----> Execution
           (Lexer)     (Parser)        (Executor)
```

### 2. State Management
- Global signal state (`g_last_signal`)
- Terminal attributes
- Environment variables
- Working directory

### 3. Error Handling
- Command parsing errors
- Execution failures
- Signal interruptions
- Memory allocation failures

## Terminal Handling

### 1. Terminal Modes
- Raw mode for input
- Canonical mode for execution
- Control character handling

### 2. Signal Display
- Custom Ctrl+C handling
- Signal character suppression
- Prompt preservation

### 3. Line Editing
- Command history
- Cursor movement
- Line clearing

## Environment Management

### 1. Variable Handling
- Environment table creation
- Variable lookup
- Value modification

### 2. Working Directory
- Path resolution
- Directory tracking
- PWD management

## Signal Integration

### 1. Shell Level
- Interactive signals
- Child process handling
- Terminal state preservation

### 2. Command Level
- Command interruption
- Status propagation
- Cleanup handling

## Error Recovery

### 1. Parser Errors
- Syntax error reporting
- AST cleanup
- State restoration

### 2. Execution Errors
- Command failures
- Resource cleanup
- Status propagation

### 3. Signal Errors
- Interrupt handling
- Terminal restoration
- State recovery

## Memory Management

### 1. Resource Tracking
- Command line allocation
- Token list management
- AST node allocation

### 2. Cleanup Procedures
- AST destruction
- Token list freeing
- Environment cleanup

## Usage Examples

### Interactive Use
```bash
Minishell$ ls -l
# Lists files
Minishell$ echo "Hello" > file.txt
# Redirects output
Minishell$ cat file.txt | grep Hello
# Pipes commands
```

### Signal Handling
```bash
Minishell$ long_running_command
^C
Minishell$ # Clean interrupt, new prompt
```

### Environment Operations
```bash
Minishell$ export VAR=value
Minishell$ echo $VAR
value
