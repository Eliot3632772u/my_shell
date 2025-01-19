#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include "../libft/libft.h"

# define SUCCESS 0
# define ERROR 1

/* Error messages */
# define ERR_MEMORY "Memory allocation failed"
# define ERR_PIPE "Pipe creation failed"
# define ERR_FORK "Fork failed"
# define ERR_CMD "Command not found"

// ------------------------------- PARSER AND LEXER ------------------------------------------------

/* Token types for parser */
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
} t_token_type;

/* Token structure */
typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

/* Parser structures */
typedef struct s_redirect
{
    int type;           /* < , > , >> , << */
    char *file;         /* Filename or delimiter for heredoc */
    struct s_redirect *next;
} t_redirect;

// Your parser will return a t_command structure:
typedef struct s_command
{
    char **args;        // Command and arguments
    t_redirect *redirects;  // Redirections
    int pipe_read;      // Pipe read end
    int pipe_write;     // Pipe write end
    struct s_command *next; // Next piped command
} t_command;

// your functions here
// (this functions by me, to give you a starting point, you still can implement your own one)
t_token *tokenize_input(char *input);
t_command *parse_tokens(t_token *tokens);
void free_tokens(t_token *tokens);

// ------------------------------- EXECUTION AND BUILTIN ------------------------------------------------

/* Environment structure */
typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

/* Shell state structure */
typedef struct s_shell
{
    t_env *env;         /* Environment variables */
    t_command *cmd;     /* Current command being executed */
    char *input;        /* Current input line */
    int last_status;    /* Exit status of last command */
    int running;        /* Shell running status */
    char **env_array;   /* Environment as string array */
} t_shell;

/* Global signal handler variable */
extern int g_signal_received;

/* function protorype (nothing implemented yet 4er bash n3rf ash ndir mn b3d osf hhh)*/

/* Builtin Commands */
int ft_echo(char **args);
int ft_cd(char **args, t_shell *shell);
int ft_pwd(void);
int ft_export(char **args, t_shell *shell);
int ft_unset(char **args, t_shell *shell);
int ft_env(t_shell *shell);
int ft_exit(char **args, t_shell *shell);

/* Executor Functions */
int execute_command(t_command *cmd, t_shell *shell);
int execute_pipeline(t_command *cmd, t_shell *shell);
int execute_builtin(char **args, t_shell *shell);
int is_builtin(char *cmd);

/* Command Path Resolution */
char *find_command_path(char *cmd, t_shell *shell);
char **get_path_directories(t_shell *shell);

/* Redirection Handling */
int setup_redirections(t_redirect *redirects);
int handle_heredoc(char *delimiter);

/* Pipe Handling */
int create_pipe(int pipe_fds[2]);
int setup_pipe_read(int pipe_fd);
int setup_pipe_write(int pipe_fd);

/* Environment Functions */
t_env *init_env(char **envp);
char *get_env_value(t_env *env, const char *key);
void set_env_value(t_env *env, const char *key, const char *value);
char **env_to_array(t_env *env);
void add_env_variable(t_env **env, char *key, char *value);
void remove_env_variable(t_env **env, char *key);

/* Signal Handling */
void setup_signals(void);
void handle_signal(int signo);
void reset_signals(void);

/* Memory Management */
void free_command(t_command *cmd);
void free_redirect(t_redirect *redirect);
void free_env(t_env *env);
void cleanup_shell(t_shell *shell);
void free_string_array(char **array);

#endif