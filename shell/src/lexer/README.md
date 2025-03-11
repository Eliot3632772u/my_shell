# Lexer Module

## Architecture Overview

```mermaid
graph TD
    A[Lexer Module] --> B[Token Processing]
    A --> C[State Management]
    A --> D[Error Handling]
    A --> E[List Management]
    
    B --> F[Token Types]
    B --> G[Token Values]
    B --> H[Token Properties]
    
    C --> I[Quote States]
    C --> J[Operator States]
    C --> K[Word States]
    
    D --> L[Error Detection]
    D --> M[Error Messages]
    D --> N[Recovery]
    
    E --> O[Token Lists]
    E --> P[Memory Management]
    E --> Q[List Operations]
```

## Component Structure

```mermaid
graph LR
    A[lexer.c] --> B[Main Analysis]
    
    C[helpers] --> D[Token Creation]
    C --> E[Type Detection]
    C --> F[Env Variables]
    
    G[token_lists] --> H[List Ops]
    
    I[errors] --> J[Messages]
    
    K[exit_status] --> L[Status Track]
```

### Main Components
1. `lexer_core.c` - Main lexical analysis functionality
2. `token_creation.c` - Token creation and initialization
3. `token_analyzer.c` - Token type identification
4. `token_validation.c` - Token validation
5. `token_manipulation.c` - Token list operations
6. `errors/` - Error handling and messages
7. `exit_status/` - Exit status management

## Token Processing Flow

```mermaid
graph TD
    A[Input String] --> B{First Char}
    B -->|Quote| C[Quote Handler]
    B -->|Operator| D[Operator Handler]
    B -->|Word| E[Word Handler]
    B -->|Space| F[Space Handler]
    
    C --> G[Quote State]
    G -->|Single| H[Raw String]
    G -->|Double| I[Expandable String]
    
    D --> J[Operator Check]
    J -->|Single| K[Simple Op]
    J -->|Double| L[Compound Op]
    
    E --> M[Word Builder]
    F --> N[Space Token]
    
    H --> O[Token List]
    I --> O
    K --> O
    L --> O
    M --> O
    N --> O
```

## Token Types and States

```mermaid
graph TD
    subgraph "Token Types"
        A1[WHITE_SPACE] --> B1[Formatting]
        A2[PIPE] --> B2[Command Chain]
        A3[QUOTES] --> B3[String Content]
        A4[OPERATORS] --> B4[Flow Control]
        A5[WORDS] --> B5[Commands/Args]
    end
    
    subgraph "State Machine"
        C1[Initial] --> D1[Processing]
        D1 --> E1[Complete]
        D1 --> F1[Error]
    end
```

### Token Type Definitions
```c
enum t_token_type {
    WHITE_SPACE,    // Spaces, tabs, etc.
    PIPE,           // |
    OR,             // ||
    STR,            // Single-quoted string
    DQSTR,          // Double-quoted string
    INPUT,          // <
    OUTPUT,         // >
    APPEND,         // >>
    HEREDOC,        // <<
    HEREDOC_TAB,    // <<-
    WORD,           // Regular words/commands
    AND,            // &&
    LPREN,          // (
    RPREN,          // )
    NEW_LINE        // End of command
}
```

## Quote Handling System

```mermaid
graph TD
    A[Quote Token] --> B{Quote Type}
    B -->|Single| C[Literal Content]
    B -->|Double| D[Expandable Content]
    
    C --> E[Find Closing]
    D --> F[Process Expansions]
    
    E -->|Found| G[Complete Token]
    E -->|Not Found| H[Error]
    
    F -->|Success| G
    F -->|Failure| H
```

## Operator Processing

```mermaid
graph LR
    A[Operator Char] --> B{Look Ahead}
    B -->|Same| C[Compound Op]
    B -->|Different| D[Simple Op]
    
    C --> E[Create Token]
    D --> E
    
    E --> F[Add to List]
```

## Word Building Process

```mermaid
graph TD
    A[Word Start] --> B[Char Analysis]
    B --> C{Char Type}
    C -->|Alpha| D[Continue Word]
    C -->|Numeric| D
    C -->|Special| E[End Word]
    
    D --> B
    E --> F[Create Token]
```

## Error Handling System

```mermaid
graph TD
    A[Error Detection] --> B{Error Type}
    B -->|Syntax| C[Syntax Error]
    B -->|Quote| D[Quote Error]
    B -->|Memory| E[Memory Error]
    
    C --> F[Error Message]
    D --> F
    E --> F
    
    F --> G[Error Status]
    G --> H[Recovery]
```

## Example Token Sequences

### Simple Command
```bash
echo "Hello World"
```
```mermaid
graph LR
    A[WORD:echo] --> B[WHITE_SPACE]
    B --> C[DQSTR:Hello World]
    C --> D[NEW_LINE]
```

### Complex Pipeline
```bash
cat file.txt | grep "pattern" > output.txt
```
```mermaid
graph LR
    A[WORD:cat] --> B[WHITE_SPACE]
    B --> C[WORD:file.txt]
    C --> D[WHITE_SPACE]
    D --> E[PIPE]
    E --> F[WHITE_SPACE]
    F --> G[WORD:grep]
    G --> H[WHITE_SPACE]
    H --> I[DQSTR:pattern]
    I --> J[WHITE_SPACE]
    J --> K[OUTPUT]
    K --> L[WHITE_SPACE]
    L --> M[WORD:output.txt]
```

## Performance Optimization

```mermaid
graph TD
    A[Performance] --> B[Memory]
    A --> C[Speed]
    A --> D[Reliability]
    
    B --> E[Token Pooling]
    B --> F[List Management]
    
    C --> G[Quick Lookup]
    C --> H[Fast Parse]
    
    D --> I[Error Check]
    D --> J[Recovery]
```

### Optimization Strategies
1. Memory Management
   - Token reuse
   - Efficient allocation
   - Smart deallocation
   - Buffer management

2. Processing Speed
   - Single-pass tokenization
   - Quick character classification
   - Efficient string operations
   - State optimization

3. Error Recovery
   - Early detection
   - Precise reporting
   - Graceful handling
   - Clean recovery

## Testing Framework

```mermaid
graph TD
    A[Test Suite] --> B[Lexer Tests]
    A --> C[Token Tests]
    A --> D[Error Tests]
    
    B --> E[Input Cases]
    C --> F[Token Types]
    D --> G[Error Cases]
    
    E --> H[Validation]
    F --> H
    G --> H
```

### Test Categories
1. Token Processing
   - Quote handling
   - Operator recognition
   - Word building
   - Space handling

2. Error Detection
   - Invalid syntax
   - Unclosed quotes
   - Memory errors
   - Recovery cases

3. Integration
   - Parser interaction
   - Memory management
   - State handling
   - List operations

## Security Considerations

```mermaid
graph TD
    A[Security] --> B[Input]
    A --> C[Memory]
    A --> D[State]
    
    B --> E[Validation]
    C --> F[Bounds]
    D --> G[Consistency]
    
    E --> H[Safe Process]
    F --> H
    G --> H
```

### Security Measures
1. Input Validation
   - Character safety
   - Length limits
   - Quote balance
   - Operator validity

2. Memory Protection
   - Allocation limits
   - Buffer checks
   - Cleanup routines
   - Error recovery

3. State Protection
   - Consistent states
   - Safe transitions
   - Error states
   - Recovery paths
