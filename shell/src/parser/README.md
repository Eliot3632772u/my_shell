# Parser Module

## Architecture Overview

```mermaid
graph TD
    A[Parser Module] --> B[Token Analysis]
    A --> C[AST Construction]
    A --> D[Memory Management]
    A --> E[Error Handling]
    
    B --> F[Token Stream]
    B --> G[Grammar Rules]
    B --> H[Syntax Check]
    
    C --> I[Node Creation]
    C --> J[Tree Building]
    C --> K[Node Linking]
    
    D --> L[Node Allocation]
    D --> M[Tree Cleanup]
    D --> N[Memory Tracking]
    
    E --> O[Syntax Errors]
    E --> P[Recovery]
    E --> Q[Reporting]
```

## Component Structure

```mermaid
graph LR
    A[parser.c] --> B[Main Parser]
    
    C[rdp.c] --> D[Recursive Parser]
    
    E[ast.c] --> F[Node Creation]
    
    G[ast_helpers] --> H[Tree Utils]
    
    I[free_ast] --> J[Memory Cleanup]
```

### Main Components
1. `parser_core.c` - Main parsing entry point
2. `recursive_descent_parser.c` - Recursive descent implementation
3. `ast_builder.c` - AST node creation
4. `ast_node_operations.c` - AST node management
5. `ast_validation.c` - AST validation and checking
6. `ast_cleanup.c` - AST memory management

## Parsing Process Flow

```mermaid
graph TD
    A[Token Stream] --> B{First Token}
    B -->|Command| C[Parse Command]
    B -->|Operator| D[Parse Operator]
    B -->|Redirect| E[Parse Redirect]
    
    C --> F[Build Command Node]
    D --> G[Build Operator Node]
    E --> H[Build Redirect Node]
    
    F --> I[Connect to AST]
    G --> I
    H --> I
    
    I --> J[Next Token]
    J --> B
```

## AST Node Types and Structure

```mermaid
graph TD
    subgraph "Node Types"
        A1[P_EXEC] --> B1[Command Execution]
        A2[P_PIPE] --> B2[Pipeline]
        A3[P_REDIR] --> B3[Redirection]
        A4[P_SUBSH] --> B4[Subshell]
        A5[P_AND] --> B5[Logical AND]
        A6[P_OR] --> B6[Logical OR]
    end
    
    subgraph "Node Structure"
        C1[Node] --> D1[Type]
        C1 --> D2[Data]
        C1 --> D3[Children]
        C1 --> D4[Next]
    end
```

### Node Type Definitions
```c
enum t_node_type {
    P_EXEC,     // Command execution
    P_PIPE,     // Pipe operator
    P_REDIR,    // Redirection
    P_SUBSH,    // Subshell
    P_AND,      // Logical AND
    P_OR        // Logical OR
};
```

## Grammar Rules and Parsing

```mermaid
graph LR
    A[Grammar] --> B[Productions]
    B --> C[command]
    B --> D[logical_op]
    B --> E[pipe_sequence]
    B --> F[redirections]
    B --> G[simple_command]
```

### EBNF Grammar
```ebnf
command     → logical_op
logical_op  → pipe_sequence ((AND | OR) pipe_sequence)*
pipe_sequence → redirections (PIPE redirections)*
redirections → (subshell | simple_command) redirection*
subshell    → LPREN command RPREN
simple_command → (WORD | QUOTED_STRING)+
redirection → (> | >> | < | << | <<-) WORD
```

## Command Structure Processing

```mermaid
graph TD
    A[Command] --> B{Structure Type}
    B -->|Simple| C[Single Command]
    B -->|Pipeline| D[Command Chain]
    B -->|Logical| E[Conditional Exec]
    B -->|Subshell| F[Nested Command]
    
    C --> G[Create Node]
    D --> H[Link Nodes]
    E --> I[Build Tree]
    F --> J[Recurse Parse]
```

## Redirection Handling

```mermaid
graph LR
    A[Redirection] --> B{Type Check}
    B -->|Input| C[Input Redir]
    B -->|Output| D[Output Redir]
    B -->|Append| E[Append Redir]
    B -->|Heredoc| F[Heredoc Setup]
    
    C --> G[Create Node]
    D --> G
    E --> G
    F --> G
```

## AST Construction Process

```mermaid
graph TD
    A[Token Stream] --> B[Parse Command]
    B --> C[Create Nodes]
    C --> D[Link Structure]
    D --> E[Validate Tree]
    
    subgraph "Node Creation"
        F[Allocate] --> G[Initialize]
        G --> H[Set Properties]
    end
    
    subgraph "Tree Building"
        I[Root] --> J[Add Children]
        J --> K[Link Siblings]
    end
```

## Example AST Structures

### Simple Command
```bash
echo "Hello World"
```
```mermaid
graph TD
    A[P_EXEC] --> B[argv]
    B --> C["echo"]
    B --> D["Hello World"]
```

### Pipeline
```bash
ls -l | grep "pattern" | wc -l
```
```mermaid
graph TD
    A[P_PIPE] --> B[P_PIPE]
    A --> C[P_EXEC:wc -l]
    B --> D[P_EXEC:ls -l]
    B --> E[P_EXEC:grep pattern]
```

### Logical Operations
```bash
cmd1 && cmd2 || cmd3
```
```mermaid
graph TD
    A[P_OR] --> B[P_AND]
    A --> C[P_EXEC:cmd3]
    B --> D[P_EXEC:cmd1]
    B --> E[P_EXEC:cmd2]
```

## Error Handling System

```mermaid
graph TD
    A[Error Detection] --> B{Error Type}
    B -->|Syntax| C[Syntax Error]
    B -->|Structure| D[Tree Error]
    B -->|Memory| E[Alloc Error]
    
    C --> F[Error Report]
    D --> F
    E --> F
    
    F --> G[Recovery]
    G --> H[Cleanup]
```

## Memory Management Flow

```mermaid
graph TD
    A[Memory Ops] --> B[Allocation]
    A --> C[Tracking]
    A --> D[Cleanup]
    
    B --> E[Node Alloc]
    B --> F[Tree Alloc]
    
    C --> G[Usage Track]
    C --> H[Leak Check]
    
    D --> I[Node Free]
    D --> J[Tree Free]
```

### Memory Management Strategies
1. Node Allocation
   - Efficient node creation
   - Property initialization
   - Children allocation
   - Link management

2. Tree Management
   - Structured growth
   - Balance maintenance
   - Connection tracking
   - Cleanup preparation

3. Memory Recovery
   - Recursive cleanup
   - Complete freeing
   - Pointer nulling
   - State restoration

## Testing Framework

```mermaid
graph TD
    A[Test Suite] --> B[Parser Tests]
    A --> C[AST Tests]
    A --> D[Grammar Tests]
    
    B --> E[Input Cases]
    C --> F[Tree Validity]
    D --> G[Rule Coverage]
    
    E --> H[Validation]
    F --> H
    G --> H
```

### Test Categories
1. Parser Testing
   - Token handling
   - Grammar rules
   - Error detection
   - Recovery methods

2. AST Testing
   - Node creation
   - Tree building
   - Property setting
   - Memory usage

3. Integration
   - Lexer interface
   - Executor handoff
   - Memory management
   - Error propagation

## Security Considerations

```mermaid
graph TD
    A[Security] --> B[Input]
    A --> C[Structure]
    A --> D[Memory]
    
    B --> E[Validation]
    C --> F[Tree Safety]
    D --> G[Allocation]
    
    E --> H[Safe Parse]
    F --> H
    G --> H
```

### Security Measures
1. Input Validation
   - Token verification
   - Structure checking
   - Depth limits
   - Size constraints

2. Tree Protection
   - Node validation
   - Link verification
   - Cycle prevention
   - Depth control

3. Resource Safety
   - Memory limits
   - Stack protection
   - Cleanup guarantee
   - Error handling
