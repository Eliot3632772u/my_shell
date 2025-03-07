# Built-in Commands Documentation

## Overview

This component implements shell built-in commands that are executed directly by the shell rather than forking a new process. Built-ins include:
- cd (Change Directory)
- echo (Display text)
- pwd (Print Working Directory)
- export (Set environment variables)
- unset (Remove environment variables)
- exit (Terminate shell)

## Command Implementation

### 1. CD (Change Directory)

```c
int ft_cd(char *arg, t_env *env)
```

Features:
- Directory navigation
- Home directory (`~`) expansion
- Previous directory (`-`) handling
- Path resolution
- PWD/OLDPWD management

Special Cases:
```bash
cd          # Go to HOME
cd ~        # Go to HOME
cd -        # Go to previous directory
cd .        # Stay in current directory
cd ..       # Go to parent directory
```

### 2. Echo

```c
int ft_echo(int argc, char **args)
```

Features:
- Text output
- `-n` flag support (no newline)
- Space handling
- Multiple argument support

Examples:
```bash
echo hello world     # Output: hello world\n
echo -n hello       # Output: hello (no newline)
echo "hello   world" # Preserves spaces
```

### 3. PWD (Print Working Directory)

```c
int ft_pwd(void)
```

Features:
- Current directory tracking
- Symlink resolution
- Error handling
- Environment variable synchronization

Functionality:
- Maintains PWD environment variable
- Updates OLDPWD on directory changes
- Handles getcwd errors

### 4. Export

```c
int ft_export(int argc, char **argv, t_env **env)
```

Features:
- Variable assignment
- Name validation
- Value concatenation
- Sorted output

Formats:
```bash
export VAR=value    # Set variable
export VAR+=value   # Append to variable
export VAR         # Mark for export
export             # List all variables
```

### 5. Unset

```c
int ft_unset(int argc, char **args, t_env **env)
```

Features:
- Variable removal
- Multiple variable support
- Environment cleanup
- Error handling

Example:
```bash
unset VAR1 VAR2    # Remove multiple variables
```

### 6. Exit

```c
int ft_exit(int argc, char *arg)
```

Features:
- Status code handling
- Numeric validation
- Integer range checking
- Error reporting

Examples:
```bash
exit            # Exit with last status
exit 42         # Exit with status 42
exit abc        # Error: numeric argument required
exit 42 43      # Error: too many arguments
```

## Error Handling

### 1. Input Validation

```c
int validate_arg(char *arg)
```
- Variable name validation
- Numeric argument checking
- Path existence verification
- Permission checking

### 2. Memory Management

```c
void free_env_node(t_env *node)
```
- Memory cleanup
- Resource deallocation
- Safe string handling
- Linked list management

### 3. Environment Management

```c
t_env *search_in_env(t_env *env, char *key)
```
- Variable lookup
- Value storage
- Path resolution
- Environment synchronization

## Examples

### 1. Complex Directory Navigation

```bash
cd /path/to/dir    # Absolute path
cd ../relative/path # Relative path
cd ~/user/docs     # Home-relative path
cd "$VAR"/path     # Variable expansion
```

### 2. Environment Variable Operations

```bash
export PATH+=:/new/path   # Path concatenation
export VAR="value"       # Variable assignment
unset VAR               # Variable removal
echo $VAR              # Variable expansion
```

### 3. Exit Status Handling

```bash
command
echo $?               # Display last exit status
exit $?              # Exit with last status
```

## Usage Guidelines

1. **Path Handling**
   - Always validate paths
   - Handle symlinks properly
   - Manage permissions
   - Update environment variables

2. **Variable Management**
   - Validate variable names
   - Handle special characters
   - Manage memory properly
   - Update environment atomically

3. **Error Handling**
   - Check return values
   - Report errors clearly
   - Clean up resources
   - Maintain shell state

4. **Security Considerations**
   - Validate input thoroughly
   - Check permissions
   - Handle environment safely
   - Prevent command injection
