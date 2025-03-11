# Expander Module

## Architecture Overview

```mermaid
graph TD
    A[Expander Module] --> B[Variable Expansion]
    A --> C[Wildcard Handling]
    A --> D[String Processing]
    A --> E[Quote Management]
    
    B --> F[Environment]
    B --> G[Special Params]
    B --> H[Word Splitting]
    
    C --> I[Pattern Matching]
    C --> J[File Listing]
    C --> K[Sort Order]
    
    D --> L[List Operations]
    D --> M[String Building]
    D --> N[Memory Management]
    
    E --> O[Quote Parsing]
    E --> P[Escape Handling]
    E --> Q[Context Track]
```

## Component Structure

```mermaid
graph LR
    A[Core expander.c] --> B[Main Logic]
    
    C[helpers] --> D[Basic Utils]
    C --> E[Env Vars]
    C --> F[Tilde]
    C --> G[Commands]
    C --> H[Special Cases]
    
    I[wildcard.c] --> J[Pattern Match]
    
    K[str_lists] --> L[Basic Ops]
    K --> M[Advanced Ops]
    K --> N[Memory]
    K --> O[Sorting]
```

### Main Components
1. `expander_core.c` - Core expansion functionality
2. `expansion_utils.c` - Basic expansion utilities
3. `variable_expansion.c` - Environment variable handling
4. `quote_handling.c` - Quote processing
5. `pattern_matching.c` - Pattern matching operations
6. `wildcard_expansion.c` - Wildcard handling
7. `path_expansion.c` - Path and tilde expansion
8. `string_lists.c` - String list core operations
9. `string_operations.c` - String transformations
10. `string_manipulation.c` - String content handling
11. `string_utils.c` - String utility functions

## Expansion Process Flow

```mermaid
graph TD
    A[Input Token] --> B{Quote Check}
    B -->|Quoted| C[Quote Handler]
    B -->|Unquoted| D[Full Expansion]
    
    C --> E[Limited Expand]
    D --> F[Variable Expand]
    
    F --> G{Pattern Check}
    G -->|Has Pattern| H[Wildcard]
    G -->|No Pattern| I[Direct]
    
    H --> J[File Match]
    I --> K[Word Split]
    J --> K
    
    E --> K
    K --> L[Result List]
```

## Variable Expansion System

```mermaid
graph TD
    A[Variable Token] --> B{Token Type}
    B -->|Basic $VAR| C[Direct Lookup]
    B -->|${VAR}| D[Brace Handler]
    B -->|Special| E[Special Param]
    
    C --> F[Env Check]
    D --> F
    E --> G[Value Fetch]
    
    F -->|Found| H[Value Process]
    F -->|Not Found| I[Empty String]
    
    H --> J[Word Split]
    I --> J
    G --> J
```

### Special Parameter Handling
```mermaid
graph LR
    A[Special Param] --> B{Type}
    B -->|$?| C[Exit Status]
    B -->|$$| D[Process ID]
    B -->|$0| E[Shell Name]
    
    C --> F[Format Value]
    D --> F
    E --> F
    
    F --> G[Return Result]
```

## Wildcard Processing

```mermaid
graph TD
    A[Pattern] --> B[Parse Pattern]
    B --> C[Build Regex]
    C --> D[List Files]
    D --> E[Apply Pattern]
    E --> F[Sort Results]
    
    subgraph "Pattern Types"
        G[* Any]
        H[? Single]
        I[[] Class]
    end
```

### Pattern Matching Flow
```mermaid
graph LR
    A[Input Pattern] --> B[Tokenize]
    B --> C[Build Matcher]
    C --> D[Directory Scan]
    D --> E[Match Files]
    E --> F[Filter Results]
    F --> G[Sort Output]
```

## Quote Handling System

```mermaid
graph TD
    A[Input String] --> B{Quote Type}
    B -->|Single| C[Literal]
    B -->|Double| D[Partial Expand]
    B -->|None| E[Full Expand]
    
    C --> F[Preserve]
    D --> G[Var Expand]
    E --> H[All Expand]
    
    G --> I[Result]
    H --> I
    F --> I
```

## String List Operations

```mermaid
graph LR
    A[String List] --> B[Add]
    A --> C[Remove]
    A --> D[Join]
    A --> E[Split]
    
    B --> F[Memory]
    C --> F
    D --> F
    E --> F
    
    F --> G[Optimize]
```

## Error Handling System

```mermaid
graph TD
    A[Error Detection] --> B{Error Type}
    B -->|Memory| C[Allocation]
    B -->|Pattern| D[Invalid]
    B -->|Access| E[Permission]
    
    C --> F[Cleanup]
    D --> F
    E --> F
    
    F --> G[Report]
    G --> H[Recovery]
```

## Example Expansions

### Environment Variables
```bash
# Basic expansion
echo $HOME           # /home/user

# Multiple variables
echo $USER@$HOSTNAME # user@host

# Special parameters
echo $?             # Last exit status
echo $$             # Shell PID

# Nested variables
echo ${PATH##*:}    # Last PATH entry
```

### Wildcard Patterns
```bash
# Basic matching
ls *.txt           # All .txt files

# Multiple patterns
ls *.[ch]          # All .c and .h files

# Hidden files
ls .*              # All hidden files

# Complex patterns
ls [a-z]*[0-9]     # Letter-start, number-end
```

### Tilde Expansion
```bash
# Home directory
cd ~               # Go to home
cd ~/Documents     # Go to Documents
cd ~user          # Go to user's home
```

## Performance Optimization

```mermaid
graph TD
    A[Performance] --> B[Memory]
    A --> C[Speed]
    A --> D[Resource]
    
    B --> E[Pooling]
    B --> F[Reuse]
    
    C --> G[Caching]
    C --> H[Early Exit]
    
    D --> I[Efficient IO]
    D --> J[Smart Alloc]
```

### Optimization Areas
1. Memory Management
   - String pooling
   - List node reuse
   - Buffer management
   - Smart allocation

2. Processing Speed
   - Pattern caching
   - Early termination
   - Result caching
   - Quick lookups

3. Resource Usage
   - Efficient file IO
   - Smart sorting
   - Minimal copying
   - State tracking

## Testing Framework

```mermaid
graph TD
    A[Test Suite] --> B[Unit]
    A --> C[Integration]
    A --> D[Performance]
    
    B --> E[Components]
    C --> F[System]
    D --> G[Benchmarks]
    
    E --> H[Results]
    F --> H
    G --> H
```

### Test Categories
1. Unit Testing
   - Variable expansion
   - Pattern matching
   - Quote handling
   - List operations

2. Integration Testing
   - Full expansions
   - Complex patterns
   - Error handling
   - Memory management

3. Performance Testing
   - Large directories
   - Complex patterns
   - Memory usage
   - Processing time

## Security Considerations

```mermaid
graph TD
    A[Security] --> B[Input]
    A --> C[Access]
    A --> D[Resource]
    
    B --> E[Validation]
    C --> F[Permission]
    D --> G[Limits]
    
    E --> H[Safe Expand]
    F --> H
    G --> H
```

### Security Measures
1. Input Validation
   - Pattern safety
   - Path validation
   - Variable checks
   - Quote integrity

2. Access Control
   - Directory permissions
   - File access
   - Environment safety
   - User boundaries

3. Resource Protection
   - Memory limits
   - Pattern complexity
   - Expansion depth
   - Result size
