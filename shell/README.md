# Minishell - A Custom Shell Implementation

> "Built with a love for coding and a serious beef with error handling… pretty sure I can C now. - YoTi"

## Table of Contents
- [Minishell Overview](#overview)
- [Component Documentation](#component-documentation)
  - [Builtin Commands](src/builtin/README.md)
  - [Environment Management](src/env/README.md)
  - [Execution System](src/execution/README.md)
  - [Expander Module](src/expander/README.md)
  - [Here-Document Handler](src/here-doc/README.md)
  - [Lexical Analyzer](src/lexer/README.md)
  - [Parser System](src/parser/README.md)
  - [Signal Handler](src/signals/README.md)
  - [Main Core](src/main/README.md)

## Overview
Minishell is a custom implementation of a Unix shell, designed to replicate core functionalities of bash. This project demonstrates advanced system programming concepts including process management, signal handling, parsing, and command execution.

## Architecture
```mermaid
graph TD
    A[User Input] --> B[Lexer]
    B --> C[Parser]
    C --> D[Expander]
    D --> E[Executor]
    
    B -->|Tokenization| F[Tokens]
    C -->|AST Generation| G[Abstract Syntax Tree]
    D -->|Variable Expansion| H[Expanded Commands]
    E -->|Execution| I[Process Management]
    
    subgraph "Signal Handling"
        J[SIGINT] --> K[Signal Handler]
        L[SIGQUIT] --> K
        M[SIGTSTP] --> K
    end
    
    K --> E
```

## Features

### Command Processing Pipeline
```mermaid
flowchart LR
    A[Input] --> B[Lexical Analysis]
    B --> C[Syntax Parsing]
    C --> D[Variable Expansion]
    D --> E[Wildcard Expansion]
    E --> F[Command Execution]
    F --> G[Output]
```

### Command Types and Execution Flow
```mermaid
graph TD
    A[Command] --> B{Type?}
    B -->|Built-in| C[Direct Execution]
    B -->|External| D[Fork Process]
    B -->|Pipeline| E[Create Pipe]
    
    D --> F[Exec Command]
    E --> G[Fork Multiple]
    G --> H[Setup Pipes]
    H --> I[Exec Commands]
    
    C --> J[Update Shell State]
    F --> K[Wait for Completion]
    I --> L[Wait for Pipeline]
```

### Built-in Commands
- `echo` with `-n` option
- `cd` with relative or absolute path
- `pwd` without options
- `export` for environment variables
- `unset` to remove variables
- `env` to display environment
- `exit` with exit status

### Advanced Features
- Command history navigation
- Syntax error handling
- Quote handling (single and double)
- Environment variable management
- Working directory tracking
- Custom prompt display

## Project Structure

### Component Overview
```mermaid
graph TD
    A[Minishell] --> B[Lexer]
    A --> C[Parser]
    A --> D[Expander]
    A --> E[Executor]
    A --> F[Built-ins]
    A --> G[Signals]
    
    B --> H[Token Management]
    C --> I[AST Generation]
    D --> J[Variable Expansion]
    D --> K[Wildcard Handling]
    E --> L[Process Management]
    E --> M[Pipe Handling]
    F --> N[Internal Commands]
    G --> O[Signal Handling]
```

### Directory Structure
```
src/
├── lexer/      - Tokenization and lexical analysis
├── parser/     - Command parsing and AST generation
├── expander/   - Variable and wildcard expansion
├── execution/  - Command execution and process management
├── env/        - Environment variable handling
├── builtin/    - Built-in command implementation
├── signals/    - Signal handling and management
├── here-doc/   - Here-document implementation
└── main/       - Shell initialization and main loop
```

## Error Handling

### Error Types and Handling Flow
```mermaid
graph TD
    A[Error Detection] --> B{Error Type}
    B -->|Syntax| C[Report Location]
    B -->|Execution| D[Show Error Message]
    B -->|Permission| E[Access Error]
    B -->|Not Found| F[Path Error]
    
    C --> G[Continue Input]
    D --> H[Set Error Status]
    E --> H
    F --> H
    H --> I[Update $?]
```

The shell handles various error cases:
- Syntax errors in command input
- Command not found in PATH
- Permission denied for execution
- Invalid redirections or pipes
- Memory allocation failures
- Signal interruptions

## Requirements

### Build Requirements
- GCC compiler
- Make build system
- Readline library
- POSIX-compliant system

### Installation
```bash
# Install readline if not present
sudo apt-get install libreadline-dev  # For Debian/Ubuntu
sudo yum install readline-devel       # For RHEL/CentOS

# Build the project
make
```

## Usage Examples

### Basic Command Execution
```bash
# Start the shell
./minishell

# Basic commands
$ echo "Hello, World!"
$ ls -l | grep ".c" > output.txt

# Here-document
$ cat << EOF > file.txt
heredoc content
EOF
```

### Environment Variable Management
```bash
# Set and export variables
$ export NAME=value
$ echo $NAME

# Unset variables
$ unset NAME

# Append to PATH
$ export PATH+=:/new/path
```

### Working with Built-ins
```bash
# Directory navigation
$ cd /path/to/dir
$ pwd

# Environment display
$ env

# Shell exit
$ exit 42
```

## Performance Considerations

### Resource Management
```mermaid
graph LR
    A[Resource Usage] --> B[Memory]
    A --> C[File Descriptors]
    A --> D[Processes]
    
    B --> E[Allocation]
    B --> F[Deallocation]
    C --> G[Opening]
    C --> H[Closing]
    D --> I[Creation]
    D --> J[Cleanup]
```

- Efficient memory management
- Proper file descriptor handling
- Process cleanup and zombie prevention
- Signal handling optimization
- Command history management

## Development Guidelines

### Code Organization
- Modular component design
- Clear separation of concerns
- Consistent error handling
- Documentation standards
- Unit test coverage

### Best Practices
- Memory leak prevention
- Error propagation
- Signal safety
- Resource cleanup
- Security considerations
