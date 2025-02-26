# Signal Handling Documentation

## Overview
The signal handling system manages interrupt signals (SIGINT, SIGQUIT) in different shell contexts, providing appropriate responses for interactive shell operation, heredoc processing, and command execution.

## Core Functions

### Signal Handlers (`signal_handler.c`)

#### `sigint_handler(int sig)`
- **Purpose**: Handles Ctrl+C (SIGINT) in main shell
- **Implementation Details**:
  1. Process checking:
     - Uses waitpid to check for running child processes
     - Returns if child is running
  2. Shell interface updates:
     - Prints ^C
     - Clears current line
     - Redraws prompt
  3. State management:
     - Sets exit status to 1
     - Updates global signal state (g_last_signal = 69)

Example behavior:
```bash
user$ long_running_command
^C
user$ # Line cleared, new prompt displayed
```

#### `heredoc_sigint_handler(int sig)`
- **Purpose**: Special SIGINT handler for heredoc input
- **Implementation Details**:
  1. Status management:
     - Sets exit status to 1
     - Sets special heredoc signal state (g_last_signal = 420)
  2. Signal reconfiguration:
     - Resets SIGINT to default behavior
     - Ignores SIGQUIT
  3. Clean termination of heredoc input

#### `handle_default_sig_handlers(int action)`
- **Purpose**: Configures signal handlers based on context
- **Implementation Details**:
  1. SET mode:
     - Installs custom SIGINT handler
     - Ignores SIGQUIT
  2. RESET mode:
     - Restores default SIGINT behavior
     - Restores default SIGQUIT behavior

#### `reset_default_sig_handlers(void)`
- **Purpose**: Restores default signal handling
- **Used**: Before executing external commands

#### `install_default_sig_handlers(void)`
- **Purpose**: Sets up shell's custom signal handling
- **Used**: During shell initialization

## Signal Behavior Matrix

### Interactive Shell
1. SIGINT (Ctrl+C):
   - Interrupts current line
   - Displays new prompt
   - Sets exit status
   - Preserves shell session

2. SIGQUIT (Ctrl-\\):
   - Ignored
   - No effect on shell

### Heredoc Processing
1. SIGINT:
   - Terminates heredoc input
   - Cleans up temporary files
   - Sets special signal state
   - Returns to shell prompt

2. SIGQUIT:
   - Ignored during heredoc
   - Preserves shell stability

### Command Execution
1. Child processes:
   - Receive default signal handling
   - Can be interrupted normally
   - Exit status propagated to shell

2. Parent shell:
   - Maintains custom handlers
   - Manages child process cleanup
   - Updates prompt as needed

## Implementation Notes

### Global State Management
- `g_last_signal`: Tracks signal states
  - 69: Normal SIGINT
  - 420: Heredoc interruption
  - Other: Various states

### Readline Integration
- `rl_on_new_line()`: Manages line state
- `rl_replace_line()`: Updates display
- `rl_redisplay()`: Redraws prompt

### Process Management
- Proper child process handling
- Exit status propagation
- Terminal state preservation

## Error Handling
1. Signal race conditions:
   - Safe handler implementation
   - State consistency checks
   - Proper cleanup sequences

2. Terminal state:
   - Proper line clearing
   - Prompt preservation
   - Display consistency

## Security Considerations
- Signal handler safety
- Process group management
- Terminal state protection

## Usage Examples

### Main Shell
```c
// Initialize shell signal handling
install_default_sig_handlers();
// Shell now handles Ctrl+C gracefully
```

### Command Execution
```c
// Before executing command
reset_default_sig_handlers();
execve(cmd, args, env);
// Child gets default handlers
```

### Heredoc Processing
```c
// During heredoc
signal(SIGINT, heredoc_sigint_handler);
// Special interrupt handling for heredoc
