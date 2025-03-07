# Signal Handling Component Documentation

## Overview

The signal handling component manages Unix signals in the shell, providing:
- Interactive signal handling (Ctrl+C, Ctrl+D, Ctrl+\)
- Here-document specific signal management
- Terminal line editing preservation
- Process group signal coordination
- Exit status management

## Core Features

### 1. Interactive Signal Handling

```c
void sigint_handler(int sig)
```

Handles SIGINT (Ctrl+C):
- Preserves command line state
- Provides visual feedback (^C)
- Resets prompt display
- Updates exit status
- Manages global signal state

### 2. Here-document Signal Management

```c
void heredoc_sigint_handler(int sig)
```

Special handling for heredocs:
- Graceful heredoc interruption
- Terminal state cleanup
- Line state preservation
- Input stream management
- Signal handler restoration

### 3. Signal Handler Management

```c
void handle_default_sig_handlers(int action)
```

Controls signal handling modes:
- SET: Install custom handlers
- RESET: Restore default handlers
- Manages SIGINT and SIGQUIT

## Signal Types

### 1. SIGINT (Ctrl+C)
```c
signal(SIGINT, sigint_handler);
```
- Interrupts current operation
- Displays ^C
- Preserves shell state
- Updates exit status to 1
- Sets global signal flag

### 2. SIGQUIT (Ctrl+\)
```c
signal(SIGQUIT, SIG_IGN);
```
- Ignored in interactive mode
- Restored to default in child processes
- Core dump management in executables

### 3. EOF (Ctrl+D)
- Handled at readline level
- Shell termination management
- History preservation

## Terminal Management

### 1. Line Editing

```c
// Terminal state preservation
rl_on_new_line();
rl_replace_line("", 0);
rl_redisplay();
```

Features:
- Line state cleanup
- Prompt redisplay
- Cursor positioning
- Input buffer management

### 2. Heredoc Specific

```c
// Heredoc cleanup
rl_free_line_state();
rl_cleanup_after_signal();
```

Operations:
- State cleanup
- Buffer management
- Terminal restoration
- Input stream handling

## Examples

### 1. Interactive Mode

```bash
$ long_running_command
^C
$             # New prompt, clean state
```

### 2. Heredoc Interruption

```bash
$ cat << EOF
> first line
^C
$             # Clean exit from heredoc
```

### 3. Child Process

```bash
$ ./program
^C            # SIGINT propagated to child
$             # Shell remains stable
```

## Implementation Details

### 1. Global State Management

```c
int g_last_signal;  // Tracks signal state
```

Usage:
- Signal coordination
- State tracking
- Process synchronization
- Exit status management

### 2. Handler Installation

```c
void install_default_sig_handlers(void)
{
    handle_default_sig_handlers(SET);
}
```

Features:
- Custom handler installation
- Default handler preservation
- Mode-specific handling
- Error checking

### 3. Handler Reset

```c
void reset_default_sig_handlers(void)
{
    handle_default_sig_handlers(RESET);
}
```

Operations:
- Restore default handlers
- Clean up custom handlers
- Prepare for execution
- State restoration

## Usage Guidelines

1. **Signal Handler Installation**
   - Install handlers at shell initialization
   - Reset handlers in child processes
   - Preserve handler state
   - Handle errors gracefully

2. **Terminal Management**
   - Clean up line state
   - Restore terminal settings
   - Handle input streams
   - Manage prompt display

3. **Process Management**
   - Track active processes
   - Handle group signals
   - Manage exit status
   - Coordinate child processes

4. **Error Handling**
   - Check signal registration
   - Handle terminal errors
   - Manage state corruption
   - Recovery procedures

## Security Considerations

1. **Signal Race Conditions**
   - Use volatile variables
   - Atomic operations
   - State synchronization
   - Safe signal handling

2. **Terminal State**
   - Save terminal settings
   - Restore on exit
   - Handle corruption
   - Clean up resources

3. **Process Management**
   - Track all processes
   - Clean up zombies
   - Handle orphans
   - Manage groups safely
