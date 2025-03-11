# Built-in Commands Implementation

## Architecture Overview

```mermaid
graph TD
    A[Built-in Command] --> B{Command Type}
    B -->|CD| C[Directory Navigation]
    B -->|Export| D[Environment Management]
    B -->|Echo| E[Output Handling]
    B -->|PWD| F[Directory Info]
    B -->|Exit| G[Shell Control]
    B -->|Env| H[Environment Display]
    B -->|Unset| I[Variable Removal]
    
    C --> J[Path Resolution]
    D --> K[Variable Management]
    E --> L[String Output]
    F --> M[Current Directory]
    G --> N[Status Management]
    H --> O[Environment List]
    I --> P[Variable Cleanup]
```

## Command Flow Diagrams

### CD Command Flow
```mermaid
graph TD
    A[CD Input] --> B{Path Type}
    B -->|Absolute| C[Validate Path]
    B -->|Relative| D[Resolve Path]
    B -->|Special| E[Handle Special]
    
    C --> F{Access Check}
    D --> F
    E --> F
    
    F -->|Success| G[Change Directory]
    F -->|Failure| H[Error Message]
    
    G --> I[Update PWD]
    G --> J[Update OLDPWD]
```

### Export Command Flow
```mermaid
graph TD
    A[Export Input] --> B{Input Type}
    B -->|No Args| C[Display Variables]
    B -->|Assignment| D[Parse Assignment]
    
    D --> E{Valid Name?}
    E -->|Yes| F[Set Variable]
    E -->|No| G[Error Message]
    
    F --> H[Update Environment]
```

### Echo Command Flow
```mermaid
graph LR
    A[Echo Input] --> B{Has -n?}
    B -->|Yes| C[No Newline Mode]
    B -->|No| D[Normal Mode]
    C --> E[Process Arguments]
    D --> E
    E --> F[Output String]
    F -->|Normal Mode| G[Add Newline]
    F -->|No Newline| H[Direct Output]
```

## Implementation Details

### Command Components
```mermaid
graph TD
    subgraph "Command Structure"
        A[Built-in Command] --> B[Argument Parser]
        A --> C[Option Handler]
        A --> D[Core Logic]
        A --> E[Error Handler]
    end
    
    subgraph "State Management"
        F[Environment] --> G[Variable Store]
        F --> H[Directory State]
        F --> I[Exit Status]
    end
    
    B --> D
    C --> D
    D --> E
    D --> F
```

## Directory Structure
```
builtin/
├── builtin_dispatcher.c # Command identification
├── cd/
│   ├── ft_cd.c        # CD implementation
│   └── ft_cd_helpers.c # Path resolution helpers
├── echo/
│   └── ft_echo.c      # Echo implementation
├── env/
│   └── ft_env.c       # Environment display
├── export/
│   ├── ft_export.c    # Export implementation
│   └── ft_export_helpers.c
├── pwd/
│   └── ft_pwd.c       # PWD implementation
├── unset/
│   └── ft_unset.c     # Unset implementation
└── exit/
    └── ft_exit.c      # Exit implementation
```

## Error Handling System

```mermaid
graph TD
    A[Error Detection] --> B{Error Type}
    B -->|Path Error| C[Directory Errors]
    B -->|Permission| D[Access Errors]
    B -->|Variable| E[Environment Errors]
    B -->|Syntax| F[Input Errors]
    
    C --> G[Error Reporting]
    D --> G
    E --> G
    F --> G
    
    G --> H[Status Update]
    H --> I[Shell Response]
```

### Error Categories

#### Directory Operations
- Not found: Directory doesn't exist
- Permission denied: Insufficient access rights
- Not a directory: Path points to non-directory
- Path resolution: Invalid path components

#### Environment Management
- Invalid identifiers: Malformed variable names
- Read-only variables: Attempt to modify protected vars
- Memory allocation: Resource exhaustion
- Duplicate variables: Name conflicts

#### Command Processing
- Invalid arguments: Wrong parameter format
- Missing arguments: Required parameters absent
- Too many arguments: Excess parameters
- Syntax errors: Malformed command structure

## Integration Flow

```mermaid
graph LR
    A[Built-in Command] --> B[Input Processing]
    B --> C[Validation]
    C --> D[Execution]
    D --> E[State Update]
    
    subgraph "Environment"
        F[Variables]
        G[Directory]
        H[Status]
    end
    
    E --> F
    E --> G
    E --> H
```

## Usage Examples

### Directory Navigation
```bash
# Basic navigation
cd /path/to/dir  # Absolute path
cd ..            # Parent directory
cd ~            # Home directory
cd -            # Previous directory
pwd             # Show current path
```

### Environment Management
```bash
# Variable operations
export PATH=/usr/bin          # Set
export PATH+=:/new/path      # Append
unset PATH                   # Remove
env                         # Display all

# Multiple operations
export A=1 B=2              # Multiple exports
unset A B                   # Multiple unsets
```

### Shell Control
```bash
# Exit variations
exit            # Normal exit
exit 42         # Exit with status
exit $?         # Exit with last status
```

## Performance Optimization

```mermaid
graph TD
    A[Performance Areas] --> B[Memory Usage]
    A --> C[Execution Speed]
    A --> D[Resource Management]
    
    B --> E[Minimal Allocation]
    B --> F[Quick Deallocation]
    
    C --> G[Direct Execution]
    C --> H[No Forking]
    
    D --> I[FD Management]
    D --> J[State Tracking]
```

### Optimization Strategies
- Minimize memory allocations
- Efficient string handling
- Quick variable lookup
- Smart path resolution
- State caching when beneficial

## Security Model

```mermaid
graph TD
    A[Security Checks] --> B{Input Validation}
    A --> C{Permission Check}
    A --> D{Resource Limits}
    
    B --> E[Sanitize Input]
    C --> F[Access Control]
    D --> G[Resource Monitor]
    
    E --> H[Safe Execution]
    F --> H
    G --> H
```

### Security Measures
- Input validation and sanitization
- Path traversal prevention
- Environment isolation
- Resource usage limits
- Error containment

## Testing Framework

```mermaid
graph TD
    A[Test Suite] --> B[Unit Tests]
    A --> C[Integration Tests]
    A --> D[Edge Cases]
    
    B --> E[Function Tests]
    C --> F[Component Tests]
    D --> G[Error Tests]
    
    E --> H[Test Results]
    F --> H
    G --> H
```

### Test Categories
- Normal operations
- Error conditions
- Edge cases
- Resource management
- Integration scenarios
- Performance metrics
