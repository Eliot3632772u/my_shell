# Here-Document (Heredoc) Module

## Architecture Overview

```mermaid
graph TD
    A[Heredoc Module] --> B[Input Processing]
    A --> C[File Management]
    A --> D[Signal Handling]
    A --> E[Cleanup System]
    
    B --> F[Content Parse]
    B --> G[Expansion]
    B --> H[Tab Control]
    
    C --> I[Temp Files]
    C --> J[Permissions]
    C --> K[Cleanup]
    
    D --> L[Interrupt]
    D --> M[Termination]
    D --> N[Recovery]
    
    E --> O[Resource Free]
    E --> P[File Delete]
    E --> Q[State Reset]
```

## Component Structure

```mermaid
graph LR
    A[Heredoc Module] --> B[heredoc_processor.c]
    B --> C[Core Logic]
    
    A --> D[Support Modules]
    D --> E[heredoc_utils.c]
    D --> F[heredoc_io.c]
    D --> G[heredoc_signals.c]
    D --> H[heredoc_cleanup.c]
    
    E --> I[Basic Utilities]
    F --> J[I/O Operations]
    G --> K[Signal Management]
    H --> L[Resource Cleanup]
```

### Main Components
1. `heredoc_processor.c` - Core heredoc functionality and main processing loop
2. `heredoc_utils.c` - Utility functions and helper operations
3. `heredoc_io.c` - File I/O and input/output stream management
4. `heredoc_signals.c` - Signal handling and interrupt management
5. `heredoc_cleanup.c` - Resource cleanup and temporary file management

## Processing Pipeline

```mermaid
graph TD
    A[Start Heredoc] --> B{Check Delimiter}
    B -->|Quoted| C[Raw Mode]
    B -->|Unquoted| D[Expand Mode]
    B -->|Tab Strip| E[Strip Mode]
    
    C --> F[Read Input]
    D --> F
    E --> F
    
    F --> G{Process Line}
    G -->|Content| H[Apply Rules]
    G -->|Delimiter| I[End Input]
    
    H --> J[Write Line]
    J --> G
    
    I --> K[Cleanup]
```

## Delimiter Processing

```mermaid
graph TD
    A[Delimiter Input] --> B{Type Check}
    B -->|Quoted| C[Literal Match]
    B -->|Unquoted| D[Expand Match]
    B -->|Invalid| E[Error]
    
    C --> F[Store State]
    D --> F
    
    F --> G[Begin Input]
    E --> H[Report Error]
```

## Content Handling Flow

```mermaid
graph LR
    A[Input Line] --> B{Content Type}
    B -->|Regular| C[Direct Write]
    B -->|Variables| D[Expansion]
    B -->|Tabbed| E[Strip Tabs]
    
    C --> F[Write File]
    D --> F
    E --> F
    
    F --> G[Next Line]
```

## File Management System

```mermaid
graph TD
    A[File Operations] --> B[Create Temp]
    A --> C[Set Permissions]
    A --> D[Write Content]
    
    B --> E[Unique Name]
    C --> F[Access Rights]
    D --> G[Buffer IO]
    
    E --> H[Track File]
    F --> H
    G --> H
    
    H --> I[Cleanup List]
```

## Features and Usage

### Heredoc Types
```bash
# Basic heredoc
cat << EOF
content
EOF

# Tab-stripping heredoc
cat <<- EOF
    indented content
EOF

# Quoted delimiter (no expansion)
cat << 'EOF'
$HOME (not expanded)
EOF
```

## Signal Handling System

```mermaid
graph TD
    A[Signal System] --> B{Signal Type}
    B -->|SIGINT| C[Interrupt]
    B -->|SIGTERM| D[Terminate]
    B -->|Custom| E[Special]
    
    C --> F[Save State]
    D --> G[Clean Exit]
    E --> H[Handle]
    
    F --> I[Recovery]
    G --> I
    H --> I
```

## Error Management

```mermaid
graph TD
    A[Error Detection] --> B{Error Type}
    B -->|File| C[File Error]
    B -->|Input| D[Input Error]
    B -->|System| E[System Error]
    
    C --> F[Report]
    D --> F
    E --> F
    
    F --> G[Recovery]
    G --> H[Cleanup]
```

### Error Categories

1. **File Operations**
   - Creation failures
   - Permission denied
   - Disk space issues
   - Path validation
   - Cleanup errors

2. **Input Processing**
   - Premature EOF
   - Invalid content
   - Expansion errors
   - Format issues
   - Quote mismatches

3. **System Resources**
   - Memory limits
   - File descriptors
   - Process limits
   - Signal handling
   - State management

## Resource Management

```mermaid
graph TD
    A[Resources] --> B[Memory]
    A --> C[Files]
    A --> D[Processes]
    
    B --> E[Allocation]
    B --> F[Deallocation]
    
    C --> G[Descriptors]
    C --> H[Cleanup]
    
    D --> I[Tracking]
    D --> J[Recovery]
```

### Resource Optimization
1. Memory Management
   - Buffer optimization
   - String handling
   - State tracking
   - Cleanup routines

2. File Operations
   - Buffered I/O
   - Descriptor limits
   - Access patterns
   - Cleanup triggers

3. Process Control
   - Signal handling
   - State preservation
   - Recovery procedures
   - Resource limits

## Security Framework

```mermaid
graph TD
    A[Security] --> B[File]
    A --> C[Input]
    A --> D[Process]
    
    B --> E[Permissions]
    C --> F[Validation]
    D --> G[Isolation]
    
    E --> H[Protection]
    F --> H
    G --> H
```

### Security Measures
1. File Security
   - Permission control
   - Path validation
   - Access limits
   - Cleanup verification

2. Input Protection
   - Content validation
   - Expansion safety
   - Quote handling
   - Size limits

3. Process Safety
   - Resource limits
   - Signal handling
   - State isolation
   - Recovery procedures

## Testing Framework

```mermaid
graph TD
    A[Test Suite] --> B[Unit Tests]
    A --> C[Integration]
    A --> D[Security]
    
    B --> E[Components]
    C --> F[System]
    D --> G[Validation]
    
    E --> H[Results]
    F --> H
    G --> H
```

### Test Categories
1. Unit Testing
   - Component functions
   - Error handling
   - State management
   - Resource usage

2. Integration Testing
   - Shell interaction
   - Signal handling
   - File operations
   - Process control

3. Security Testing
   - Permission checks
   - Input validation
   - Resource limits
   - Cleanup verification

## Integration Points

```mermaid
graph LR
    A[Heredoc] --> B[Parser]
    A --> C[Shell]
    A --> D[System]
    
    B --> E[AST]
    C --> F[Environment]
    D --> G[Resources]
    
    E --> H[Integration]
    F --> H
    G --> H
```

### Integration Areas
1. Parser Integration
   - Token processing
   - AST building
   - State management
   - Error handling

2. Shell Integration
   - Environment access
   - Signal management
   - Resource control
   - Status tracking

3. System Integration
   - File operations
   - Process control
   - Resource management
   - Security enforcement
