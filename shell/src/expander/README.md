# Expander Documentation

## Overview
The expander module handles expansion of environment variables, wildcards, and argument splitting in the Minishell project. It processes tokens to resolve variables, handle quotes, and expand wildcards into matching filenames.

## Core Functions

### expander.c

#### `ft_chr(char *str, char c)`
- **Purpose**: Wrapper for ft_strchr
- **Returns**: Pointer to first occurrence of character
- **Implementation**: Simple wrapper around ft_strchr for consistent naming convention

#### `expand_env(char *to_expand, bool in_quote, bool ignore_env)`
- **Purpose**: Expands environment variables in a string
- **Parameters**:
  - `to_expand`: String containing potential variables
  - `in_quote`: Whether processing inside quotes
  - `ignore_env`: Whether to skip environment expansion
- **Returns**: Expanded string with variables replaced
- **Implementation Details**:
  1. Uses stringbuilder for efficient string manipulation
  2. Processes string character by character:
     - Identifies potential variables ($)
     - Handles special case $? for exit status
     - Expands environment variables
  3. Special handling for quotes:
     - In quotes: preserves spaces
     - Outside quotes: replaces spaces with TROLL character
  4. Memory management:
     - Frees original string
     - Returns newly allocated expanded string

Example expansion process:
```
Input: "Hello $USER in $PWD"
1. Scan until $: append "Hello "
2. Find USER variable: append "yoti"
3. Append " in "
4. Find PWD variable: append "/home/yoti"
Output: "Hello yoti in /home/yoti"
```

#### `expand(t_token *tok, bool ignore_env)`
- **Purpose**: Expands a single token
- **Implementation Details**:
  1. Quote handling:
     - DQSTR: removes double quotes, allows variable expansion
     - STR: removes single quotes, no expansion
     - WORD: direct expansion
  2. Token expansion flags:
     - Checks to_expand flag
     - Respects ignore_env parameter
  3. Memory management:
     - Creates new string without quotes
     - Handles expansion allocation

Example processing:
```
1. Token: DQSTR("$HOME/file")
   - Remove quotes
   - Expand $HOME
   - Result: "/home/user/file"

2. Token: STR('$HOME/file')
   - Remove quotes
   - No expansion
   - Result: "$HOME/file"

3. Token: WORD(file*.txt)
   - No quotes to remove
   - Mark for wildcard expansion
   - Result: "file*.txt"
```

#### `expand_nosp_arg(t_token *sub_tok, t_str **lst, bool ignore_env)`
- **Purpose**: Expands tokens that are part of a no-space sequence
- **Implementation Details**:
  1. Token processing loop:
     - Expands each token
     - Handles special TROLL character
     - Manages space splitting
  2. List management:
     - Adds to string list
     - Handles joining conditions
  3. Memory cleanup after processing

#### `expand_args(t_token *tok_lst)`
- **Purpose**: Main function for expanding command arguments
- **Implementation Details**:
  1. Token list traversal:
     - Processes each token sequentially
     - Handles no-space sequences
  2. Wildcard expansion:
     - Converts patterns to matches
     - Maintains ordering
  3. Final conversion:
     - Builds argument array
     - Proper memory management

## Helper Functions

### expander_helpers.c

#### `get_chunk_len(char *ptr, char *sp)`
- **Purpose**: Calculates length of next chunk to process
- **Implementation Details**:
  1. Special character handling:
     - Checks for $ and quotes
     - Special case for $?
  2. Word length calculation:
     - Counts until special char
     - Handles environment variables

#### `ghost_char(char *str)`
- **Purpose**: Replaces spaces/tabs with TROLL character
- **Implementation Details**:
  - In-place string modification
  - Preserves non-space characters

### expander_helpers_2.c

#### `handle_env_var(char *chunk, char *ptr, size_t len, bool in_quote, size_t *offset)`
- **Purpose**: Processes environment variable expansion
- **Implementation Details**:
  1. Special cases:
     - $? returns exit status
     - Handles empty variables
  2. Space handling:
     - In quotes: preserves spaces
     - Outside quotes: replaces with TROLL
  3. Memory management:
     - Frees old chunk
     - Returns expanded value

#### `handle_chunk(t_strbuilder *sb, char **chunk, char *ptr, bool in_quote, bool ignore_env, size_t *offset)`
- **Purpose**: Processes a chunk of text for expansion
- **Implementation Details**:
  1. Chunk extraction:
     - Gets length using get_chunk_len
     - Creates substring
  2. Variable handling:
     - Checks for $ prefix
     - Calls handle_env_var if needed
  3. Builder appending:
     - Manages string construction
     - Handles memory

#### `handle_split_args(char *to_join, t_str **lst, int iter, t_token *sub_tok, char split_char)`
- **Purpose**: Splits and processes expanded arguments
- **Implementation Details**:
  1. Splitting process:
     - Uses ft_split for separation
     - Handles special characters
  2. List building:
     - Adds each part to list
     - Manages joining conditions

## String List Management

### str_lists.c & str_lists_2.c
(These functions follow standard linked list patterns, implementation details are straightforward)

## Wildcard Expansion (wildcard.c)

#### `wild_match(char *str, char *expr)`
- **Purpose**: Pattern matching for wildcards
- **Implementation Details**:
  1. Recursive matching algorithm:
     - Handles * as zero or more characters
     - Direct character comparison for non-wildcards
  2. Optimization for trailing wildcards
  3. Special cases:
     - Empty string matching
     - Multiple consecutive wildcards
     - Pattern boundaries

Example pattern matching:
```
Pattern: "file*.txt"
Matches:
  "file.txt"        -> true
  "file1.txt"       -> true
  "files.txt"       -> true
Does not match:
  "file.doc"        -> false
  ".txt"           -> false
  "other.txt"      -> false

Implementation walk-through:
1. "file*.txt" matching "file1.txt":
   - Match 'f','i','l','e' directly
   - * matches '1'
   - Match '.','t','x','t' directly
   - Both strings exhausted -> true
```

#### `wild_card(t_str **lst, t_str *expr)`
- **Purpose**: Expands wildcard pattern to matching filenames
- **Implementation Details**:
  1. Directory handling:
     - Opens current directory
     - Reads entries one by one
     - Proper cleanup on errors
  2. Hidden file rules:
     - Only matches hidden files if pattern starts with '.'
     - Skips . and .. entries
  3. Pattern matching:
     - Calls wild_match for each entry
     - Maintains sorted order
  4. No-match behavior:
     - Preserves original pattern
     - Adds to list unchanged

#### `expand_wild_cards(t_str *argv_lst)`
- **Purpose**: Processes all wildcard patterns in argument list
- **Implementation Details**:
  1. List traversal:
     - Checks each node's wild_card flag
     - Calls wild_card for matching
  2. Memory management:
     - Frees old nodes
     - Creates new expanded list
  3. Pattern preservation:
     - Maintains non-wildcard entries
     - Handles no-match cases

## Error Handling
- Memory allocation failures:
  - Proper cleanup on malloc fails
  - Return NULL or false as appropriate
- Directory access errors:
  - Graceful handling of opendir failures
  - Proper closedir in all cases
- Environment variable resolution:
  - Handles undefined variables
  - Special case for $? exit status
- Quote matching:
  - Proper handling of quoted spaces
  - Preservation of quoted content
- Pattern matching failures:
  - Preserves original pattern
  - Maintains command integrity
