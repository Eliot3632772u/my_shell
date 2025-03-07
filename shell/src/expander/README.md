# Expander Component Documentation

## Overview

The expander component is responsible for processing and expanding various shell elements:
- Environment variables (`$VAR`, `${VAR}`)
- Special parameters (`$?`, `$$`)
- Wildcards (`*`)
- Tilde expansion (`~`)
- Quoted strings handling

## Core Features

### 1. Environment Variable Expansion

```c
char *expand_env(char *to_expand, bool in_quote, bool ignore_env)
```

- Processes environment variables in strings
- Handles both simple (`$VAR`) and complex (`${VAR}`) formats
- Special handling for quoted strings
- Supports nested variable expansion

#### Special Cases:
- `$?` - Expands to last command's exit status
- `$$` - Expands to process ID
- `$0` - Expands to shell name

### 2. Wildcard Expansion

```c
t_str *expand_wild_cards(t_str *argv_lst)
```

Features:
- Pattern matching against filenames
- Handles multiple wildcards in pattern
- Sorts matched files alphabetically
- Preserves non-matching patterns
- Hidden files handling (`.` prefix)

#### Pattern Matching Algorithm:
```c
bool wild_match(char *str, char *expr)
```
- Recursive matching for wildcards
- Supports multiple `*` in pattern
- Handles edge cases (empty patterns, all wildcards)

### 3. Tilde Expansion

```c
char *expand_tilde(char *str)
```

- Expands `~` to user's home directory
- Only expands at start of word
- Requires valid `HOME` environment variable
- Handles `~/path` format

## String Processing

### Token-based Processing

```c
void expand_nosp_arg(t_token *sub_tok, t_str **lst, bool ignore_env)
```

1. **Pre-processing**:
   - Identifies expandable tokens
   - Handles quoted strings
   - Preserves word boundaries

2. **Token Types**:
   - `WORD`: Regular text
   - `DQSTR`: Double-quoted string
   - `STR`: Single-quoted string

3. **Space Handling**:
   - Preserves spaces in quotes
   - Handles word splitting
   - Special characters for space preservation

## Examples

### Environment Variables

```bash
# Input: echo $USER
# Process:
1. Token identification: [WORD "echo"] [WORD "$USER"]
2. Variable expansion: $USER → "username"
# Output: echo username

# Input: echo "$HOME/file"
# Process:
1. Token identification: [DQSTR "$HOME/file"]
2. Variable expansion within quotes
# Output: echo /home/user/file
```

### Wildcards

```bash
# Input: ls *.txt
# Process:
1. Token identification: [WORD "ls"] [WORD "*.txt"]
2. Wildcard expansion: *.txt → [file1.txt file2.txt ...]
# Output: ls file1.txt file2.txt ...

# Input: echo */*
# Process:
1. Recursive directory matching
2. Sorted output
# Output: dir1/file1 dir1/file2 dir2/file1 ...
```

### Combined Features

```bash
# Input: echo ~/$USER/*.txt
# Process:
1. Tilde expansion: ~ → /home/user
2. Variable expansion: $USER → username
3. Wildcard expansion: *.txt → matching files
# Output: echo /home/user/username/file1.txt ...
```

## Error Handling

1. **Memory Management**:
   - Proper allocation/deallocation
   - Cleanup on expansion failure
   - String builder for safe concatenation

2. **Edge Cases**:
   - Non-existent variables
   - Invalid patterns
   - Permission denied errors
   - Quote mismatches

3. **Special Characters**:
   - Escape sequences
   - Null characters
   - Unicode handling

## Usage Guidelines

1. **Order of Operations**:
   ```c
   // Recommended expansion order
   expanded = expand_tilde(str);
   expanded = expand_env(expanded, in_quote, ignore_env);
   expanded = expand_wild_cards(expanded);
   ```

2. **Memory Management**:
   - Free original strings after expansion
   - Use string builder for complex concatenations
   - Handle NULL returns from expansion functions

3. **Error Checking**:
   - Validate environment variables
   - Check file permissions for wildcards
   - Verify HOME variable for tilde expansion

4. **Performance Considerations**:
   - Use string builder for multiple concatenations
   - Avoid redundant expansions
   - Cache frequently used environment variables
