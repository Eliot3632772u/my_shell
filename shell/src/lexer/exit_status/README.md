# Exit Status Component Documentation

## Overview

The exit status component manages the shell's exit status values, providing:
- Status code storage
- Status access control
- Status code management
- Convention compliance

## Core Functions

### 1. Exit Status Management

```c
int exit_status(unsigned int action)
```

Core exit status controller:
- Maintains static exit status
- Provides atomic access
- Handles status updates
- Supports status queries

Usage:
```c
// Set new status
exit_status(42);

// Get current status
int status = exit_status(GET_EXIT);
```

### 2. Status Setting

```c
void set_exit_status(unsigned int status)
```

Sets new exit status:
- Updates global status
- Thread-safe operation
- Maintains state consistency
- Enforces value limits

Example:
```c
// Set successful execution
set_exit_status(0);

// Set error status
set_exit_status(1);
```

### 3. Status Retrieval

```c
int get_exit_status(void)
```

Retrieves current status:
- Returns last status
- Thread-safe access
- Non-modifying operation
- Convention compliant

Example:
```c
// Check last command status
if (get_exit_status() == 0) {
    // Success handling
} else {
    // Error handling
}
```

## Status Code Conventions

### 1. Standard Codes

| Code | Description |
|------|-------------|
| 0 | Successful execution |
| 1 | General errors |
| 2 | Syntax/parsing errors |
| 126 | Command found but not executable |
| 127 | Command not found |
| 128+n | Fatal error signal n |
| 255 | Exit status out of range |

### 2. Signal Handling

Signal-based exit codes (128 + signal number):
- SIGINT (130): Ctrl+C
- SIGQUIT (131): Ctrl+\\
- SIGTERM (143): Termination request
- Others as per signal value

## Implementation Details

### 1. Static Storage

```c
static int exit_status;
```

Features:
- Persistent across calls
- Process-wide visibility
- Protected access
- State preservation

### 2. Access Control

```c
#define GET_EXIT 0xFFFFFFFF
```

Mechanism:
- Special value for queries
- No status collision
- Clear intent signaling
- Safe status retrieval

## Usage Guidelines

1. **Status Setting**
   - Set appropriate codes
   - Follow conventions
   - Handle errors properly
   - Document status meanings

2. **Status Checking**
   - Use status helpers
   - Check after commands
   - Handle all ranges
   - Propagate when needed

3. **Error Handling**
   - Set descriptive codes
   - Follow shell standards
   - Clean up on errors
   - Document error states

4. **Signal Integration**
   - Handle all signals
   - Set correct codes
   - Maintain consistency
   - Follow conventions

## Examples

### 1. Command Execution

```c
// Execute command
if (execute_command() != 0) {
    set_exit_status(1);
    return;
}
set_exit_status(0);
```

### 2. Signal Handling

```c
// Handle Ctrl+C
void sigint_handler(int sig) {
    set_exit_status(128 + sig);  // 130 for SIGINT
}
```

### 3. Error Propagation

```c
// Propagate child status
int status = wait_child();
if (WIFEXITED(status)) {
    set_exit_status(WEXITSTATUS(status));
}
```

### 4. Status Checking

```c
// Check last operation
if (get_exit_status() == 0) {
    // Success path
} else if (get_exit_status() == 127) {
    // Command not found
} else {
    // Other error handling
}
```

## Best Practices

1. **Exit Code Selection**
   - Use standard codes
   - Be consistent
   - Document meanings
   - Follow conventions

2. **Status Management**
   - Clean state updates
   - Proper propagation
   - Clear documentation
   - Error recovery

3. **Signal Handling**
   - Proper signal codes
   - Status preservation
   - Clean termination
   - State consistency

4. **Error Recovery**
   - Status preservation
   - Clean error states
   - Proper reporting
   - Resource cleanup
