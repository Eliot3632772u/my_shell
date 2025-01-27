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
# define STD_ERR 2
# define ERR_MEMORY 3
# define ERR_QUOTE 4
/* Error messages */
# define ERR_MEMORY_MSG "Memory allocation failed"
# define ERR_PIPE "Pipe creation failed"
# define ERR_FORK "Fork failed"
# define ERR_CMD "Command not found"
# define ERR_QUOTE_MSG "Syntax error: unclosed quote"

// ------------------------------- PARSER AND LEXER ------------------------------------------------

/* Token types for parser */
typedef enum e_token_type
{
    WORD,
    WORD_VAR,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    APPEND,
    HEREDOC,
    T_EOF,
    T_AND, /* && */
    T_OR,/* || */
    WILD,/* wildcard *.c */
    O_P,/* open parentheses --> ( */
    C_P,/* closing parentheses --> ) */
    //EXIT_STATUS,/* $? */
    VARIABL, /* $HOME */
    NONE // just a none type (instead of null)
} t_token_type;

/* Token structure */
typedef struct      s_token
{
    t_token_type    type;
    char            *value;
    int             concate;
    int             expand;
    struct s_token  *next;
    struct s_token  *prev;
}                   t_token;

/* Parser structures */
typedef struct          s_redirect
{
    t_token_type        type;           /* < , > , >> , << */
    char                *file;         /* Filename or delimiter for heredoc */
    t_token             *tok_file; /* tokens that will be the file name or delemeter */
    struct s_redirect   *next;
    struct s_redirect   *prev;
}                       t_redirect;

// Your parser will return a t_command structure:
typedef struct          s_command
{
    char                **args;        // Command and arguments
    t_redirect          *redirects;  // Redirections
    int                 pipe_read;      // Pipe read end
    int                 pipe_write;     // Pipe write end
    struct s_command    *next; // Next piped command
} t_command;

/* Environment structure */
typedef struct      s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}                   t_env;

/* Shell state structure */
typedef struct      s_shell
{
    t_env           *env;         /* Environment variables */
    t_command       *cmd;     /* Current command being executed */
    char            *input;        /* Current input line */
    int             last_status;    /* Exit status of last command */
    int             running;        /* Shell running status */
    int             error;  /* handling errors */
    //int             is_DOLLAR; /* check inside the double quotes if there is a dollar sign to expand in excution*/
    t_token_type    type;
    char            *buffer;
    char            **env_array;   /* Environment as string array */
} t_shell;

typedef enum        e_ast_type
{
    ast_pip,
    ast_none,
    ast_cmd
}                   t_ast_type;

typedef struct      s_ast 
{
    t_ast_type      type;  // Node type (e.g., CMD, PIPE, etc.)
    //t_command       *cmd; 
    char            *args;
    t_token         *tok_args; // tokens to be arguments      
    t_redirect      *redc; // Linked list of redirections
    struct s_ast    *left;    // Left subtree
    struct s_ast    *right;   // Right subtree
}                   t_ast;

// your functions here
// (this functions by me, to give you a starting point, you still can implement your own one)
//t_token *tokenize_input(char *input);
//t_command *parse_tokens(t_token *tokens);
//void free_tokens(t_token *tokens);

/*Lexer*/
void	lexer(t_shell *shell, t_token **token, char *input);

/*lexer utiles*/
int	    is_special(char c);
void	handle_special(t_shell *shell, t_token **token, char **input);
void	check_concate(t_token *token, char **input);
void	tokenize(t_shell *sh, t_token **token, char *val, t_token_type type);
int	    is_delem(char **input);

/*Parser*/

/*Parser utiles*/
t_ast   *parse_sub(t_token **tok);
t_ast   *parse_pipe(t_token **tok);
t_ast   *parse_logical(t_token **tok);
t_ast   *parse_redi(t_token **tok);
t_ast	*parse_cmd(t_token **tok);
t_ast   *new_cmd(t_token *token);

// ------------------------------- EXECUTION AND BUILTIN ------------------------------------------------



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
void free_tokens(t_token *tokens);

#endif