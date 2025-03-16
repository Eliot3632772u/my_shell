/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:57:34 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 07:20:14 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <errno.h>
# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <termios.h>
# include <limits.h>

# define GET_EXIT 0xdeadbeef
# define HIDDEN_SEPARATOR 132
# define DEFAULT_PATH "/usr/local/sbin:/usr/local/ \
bin:/usr/sbin:/usr/bin:/sbin:/bin"
# define SHLVL_WARN "minishell: warning: shell level \
(1000) too high, resetting to 1"

# ifndef ECHOCTL
#  define ECHOCTL 0x00000040
# endif

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

// Forward declarations
typedef struct s_cmd	t_ast_cmd;
typedef struct s_ast_redir	t_ast_redir;

# define COLOR_CYAN "\001\033[1;36m\002"
# define COLOR_RED "\001\033[1;31m\002"
# define COLOR_BLUE "\001\033[1;34m\002"
# define COLOR_WHITE "\001\033[1;37m\002"
# define COLOR_RESET "\001\033[0m\002"

// Prompt function
char *get_prompt(void);

// lexer
typedef enum e_token_type {
    WORD = 0,
    STR,
    DQSTR,
    OUTPUT,
    APPEND,
    INPUT,
    HEREDOC,
    HEREDOC_TAB,
    LPREN,
    RPREN,
    PIPE,
    OR,
    AND,
    WHITE_SPACE,
    NEW_LINE
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    int len;
    bool to_expand;
    struct s_token *nospace_next;
    struct s_token *next;
    struct s_token *prev;
} t_token;

typedef struct s_heredoc_opts {
    bool expandable;
    bool strip_tabs;
} t_heredoc_opts;

bool lexer(char *command_line, t_token **tokens);
bool check_token(t_token *this_tok, char *cmd_line, int index_space[2], t_token **tokens);
void get_token(char *where, t_token *tok);
t_token *new_token(t_token_type type, char *value, int len);
t_token_type token_type(char *str);
int count_with_func(char *str, bool (*test_func)(char));
int token_len(t_token_type type, char *str);
bool check_expanding(char *str, t_token_type type);
bool valid_env_char(char c);
int get_exit_status(void);
void set_exit_status(unsigned int status);
int exit_status(unsigned int action);
void tok_error(char err);
void unclosed_error(char err);
void syntax_error(char *err);
int print_err(char *preced, int msg_code);
void free_tok_lst(t_token *tok);
char *handle_heredoc(char *delim, t_token_type type);
char *ft_mktmp(void);
bool is_expandable(t_token *tok);
t_token *ft_toklast(t_token *lst);
void ft_tokadd_back(t_token **lst, t_token *new_tok);
void ft_tokiter(t_token *lst, void (*f)(void *));
void add_to_sublist(t_token **list, t_token *new_tok);
bool add_token(t_token **tokens, t_token *token, bool is_space);
bool handle_heredoc_line(char *line, char *delim, t_token_type type, int fd);
int init_heredoc(char *delim, char **tmp_file, int *fd);
char *strip_leading_tabs(char *line);
bool is_expandable(t_token *tok);
bool is_delimiter(char *line, char *delim, t_token_type type);
void process_and_write_line(char *line, int fd);
size_t count_leading_tabs(char *line);
bool is_empty_quoted_string(char *str);
char *strip_quotes(char *str);
void handle_heredoc_signal(void);
void cleanup_heredoc_file(char *tmp_file);
void cleanup_all_heredoc_files(void);
void reset_stdin(void);

// ast 
typedef enum e_node_type
{
	P_AND,
	P_OR,
	P_PIPE,
	P_EXEC,
	P_REDIR,
	P_SUBSH
}			t_node_type;

typedef struct s_cmd
{
	t_node_type	type;
}			t_ast_cmd;

typedef struct s_ast_binary
{
	t_node_type	type;
	t_ast_cmd	*left;
	t_ast_cmd	*right;
}				t_ast_binary;

typedef struct s_ast_exec
{
	t_node_type	type;
	t_token		*argv_tok;
}				t_ast_exec;

typedef struct s_ast_redir
{
	t_node_type		type;
	t_token_type	direction;
	t_token			*file_tok;
	int				mode;
	int				fd;
	t_ast_cmd		*cmd;
}				t_ast_redir;

typedef struct s_ast_subsh
{
	t_node_type	type;
	t_ast_cmd	*cmd;
}				t_ast_subsh;

// ---------------------------------------------------------------- 
typedef struct          s_redirect
{
    t_token_type        type;           /* < , > , >> , << */
    t_token             *file_tok; /* tokens that will be the file name or delemeter */
	int					mode;
	int					expand;
    struct s_redirect   *next;
    struct s_redirect   *prev;
}                       t_redirect;

typedef struct      s_ast 
{
    t_node_type      type;  // Node type (e.g., CMD, PIPE, etc.) 
    t_token         *argv_tok; // tokens to be arguments      
    t_redirect      *redc; // Linked list of redirections
    struct s_ast    *left;    // Left subtree
    struct s_ast    *right;   // Right subtree
}                   t_ast;


/* Lexer2 */

void    lexer2(char *command_line, t_token **tokens);
void    handle_special(char **command_line, t_token **tokens, int *concate);
void    tok_str(char **command, t_token **tokens, int *concate);
void    handle_quotes(char **command, t_token **tokens, int *concate);
void    and_tok(char **command, t_token **tokens);
void    paren_tok(char **command, t_token **tokens);
void    pipe_tok(char **command, t_token **tokens);
void    out_tok(char **command, t_token **tokens);
void    input_tok(char **command, t_token **tokens);
t_token_type    check_type(char quote);
void 	check_concate(char **command, int *concate);
int	special_char(char c);
int	is_wild_special(char **input);
int	check_expand_status(char **input);
int	is_delem(char **input);
t_token_type    check_type(char quote);
void check_concate(char **command, int *concate);
void    input_tok(char **command, t_token **tokens);
void    out_tok(char **command, t_token **tokens);
void    pipe_tok(char **command, t_token **tokens);
void    paren_tok(char **command, t_token **tokens);
/* Parser2 */

t_ast		*parser2(t_token **tok);
t_ast		*unexpec_tok(t_token **tok, t_ast *ast);
t_ast		*parse_logical(t_token **tok, int *err);
t_ast		*parse_pipe2(t_token **tok, int *err);
t_ast		*parse_redi(t_token **tok, int *err);
t_ast		*parse_cmd2(t_token **tok, int *err);
t_ast		*parse_sub(t_token **tok, int *err);
void		free_redi(t_redirect *red);
t_ast		*free_ast2(t_ast *ast);
t_ast		*new_cmd(t_token *token);
void		add_arg(t_token **argv_tok, t_token **tok);
t_redirect	*new_redic(t_token_type type);
t_token		*get_file_tokens(t_token **tok);
int			add_redic(t_redirect **redic, t_token **tok);
int			get_redic_mode(t_token *redic);
t_ast		*new_ast(t_node_type type);
void		free_tok_node(t_token **tok);
int			check_logical(t_token_type type);
int			check_redir(t_token_type type);
int			check_cmd(t_token_type type);
int			check_word(t_token_type type);
t_ast	*parse_sub(t_token **tok, int *err);
t_ast	*parse_cmd2(t_token **tok, int *err);

/* REDIRECTIONS */

int exec_redc(t_redirect *redc, int mode);
int expand_redc_file(t_redirect *redc, char **file);


int		ft_heredoc(t_ast *tree);
int		patch_token(t_redirect *redc);
int		process_heredoc_tree(t_ast *tree);
void 	save_fd(int flag);
int 	proc_heredoc_file(int fd, char *file);
int 	exec_heredoc(t_redirect *redc, char **file);
int 	open_dup_wrap(t_redirect *redc,char *file, int STD_FD);
// ---------------------------------------------------------------- 

bool			parser(t_token *tokens, t_ast_cmd **tree);
t_ast_cmd		*parse_cmd(t_token **current);
t_ast_cmd		*parse_pipe(t_token **current);
t_ast_cmd		*parse_redir(t_token **current);
t_ast_cmd		*parse_paren(t_token **current);
bool			parse_term(t_token **current, t_ast_cmd *sub_sh, \
	t_token **exe_lst, t_ast_redir **redir_lst);
t_ast_cmd		*subsh_node(t_ast_cmd *cmd);
t_ast_cmd		*exec_node(t_token *argv);
t_ast_cmd		*redir_node(t_token_type direction, t_token *file_tok, \
	int mode_fd[2], t_ast_cmd *cmd);
t_ast_cmd		*binary_node(t_node_type type, t_ast_cmd *left, \
	t_ast_cmd *right);
void			advance(t_token **current);
bool			match(t_token *tok, t_token_type types[], int size);
t_token			*clone_tok(t_token *tok);
t_ast_redir		*tok_to_redir(t_token *redir_ptr);
void			free_redir(t_ast_cmd *sub_sh, t_ast_redir *redir_lst, \
	t_token *exe_lst);
bool			add_redir_node(t_ast_redir **lst, t_ast_cmd *node);
t_ast_cmd		*redir_file(t_token **current);
void			free_ast(t_ast_cmd *tree);

// expander
typedef struct s_str
{
	char			*str;
	bool			wild_card;
	struct s_str	*next;
}				t_str;

typedef struct s_env_var
{
	char	*chunk;
	char	*ptr;
	size_t	len;
	bool	in_quote;
	size_t	*offset;
}			t_env_var;

typedef struct s_chunk_info
{
	t_strbuilder	*sb;
	char			**chunk;
	char			*ptr;
	bool			in_quote;
	bool			ignore_env;
	size_t			*offset;
}				t_chunk_info;

typedef struct s_split_args
{
	char			*to_join;
	t_str			**lst;
	int				iter;
	t_token			*sub_tok;
	char			split_char;
}					t_split_args;

void			update_token_after_heredoc(t_token *tok, t_redirect *redc, \
	t_str *s_ptr);
char			*get_processed_delimiter(t_redirect*redc, t_str **s_ptr);
t_str			*expand_wild_cards(t_str *argv_lst);
void			wild_card(t_str **lst, t_str *expr);
bool			wild_match(char *str, char *expr);
char			**handle_wildcard_expansion(t_str *argv_lst, t_str *args);
char			**expand_args(t_token *tok_lst);
void			expand_nosp_arg(t_token *sub_tok, t_str **lst, bool ignore_env);
char			*expand(t_token *tok, bool ignore_env);
char			*expand_env(char *to_expand, bool in_quote, bool ignore_env);
void			ghost_char(char *str);
unsigned int	get_chunk_len(char *ptr, char *sp);
void			ft_striter(t_str *lst, void (*f)(void *));
int				ft_strlstlen(t_str *lst);
void			free_strnode(t_str *node);
t_str			*ft_strlast(t_str *lst);
t_str			*new_str(char *str, bool to_expand);
int				split_len(char **lst);
void			ft_stradd_back(t_str **lst, t_str *new);
void			ft_join_last(t_str **lst, char *str, bool to_expand);
void			add_str_lst(char *str, t_str **lst, bool join_to_last, \
	t_token *tok);
void			free_str_lst(t_str *lst);
char			**consume_argv(t_str *lst);
char			*handle_env_var(t_env_var *env);
bool			handle_chunk(t_chunk_info *info);
void			handle_split_args(t_split_args *args);
char			*expand_tilde(char *str);
char			*handle_double_dollar(char *chunk, char *ptr, size_t *offset);
char			*handle_exit_status_2(char *chunk);
char			*handle_regular_env_var(t_env_var *env);
bool			is_dollar_quoted_str(const char *str);
char			*handle_dollar_quoted(char *str);
bool			is_standalone_dollar_star(const char *str);
t_str			*sort_merge(t_str *left, t_str *right);
void			split_list(t_str *source, t_str **left, t_str **right);
void			sort_str_list(t_str **lst);
bool			expand_environment_variable(t_chunk_info *info, size_t len);
char			**handle_argv_cases(t_str *argv_lst, t_str *args);
char			**handle_empty_str_case(void);
t_str			*process_command(t_token *cmd_token);
void			add_to_args_list(t_str **args, t_str *new_list);
void			handle_wildcard_argument(t_str *arg_list, t_str **args);
void			process_single_argument(t_token *arg_token, t_str **args);
t_str			*process_arguments(t_token *args_token);
int process_heredoc_input(int fd, char *delim, t_token_type type);

// attributs
enum
{
	ATTR_SET,
	ATTR_GET,
	ATTR_CHG
};

// env
typedef struct s_env
{
	bool			equal_sign;
	char			*key;
	char			*value;
	struct s_env	*prev;
	struct s_env	*next;
}				t_env;

char			*get_env_value(char *var);
char			**consume_env(t_env *env);
t_env			*get_envp(t_env *envp);
t_env			**get_envp_internal(t_env *envp);
t_env			*search_in_env(t_env *env, char *key);
void			set_env_value(char *key, char *value, int equal_sign);
char			*get_value(char *line);
char			*get_key(char *line);
void			log_last_command(char *value);
void			create_env(char **envp);
void			set_env_value(char *key, char *value, int equal_sign);
void			concate_env(char *key, char *value);
void			edit_env(t_env *node, char *value, int equal_sign);
void			append_to_env(t_env **env, char *key, char *value, \
	int equal_sign);
t_env			*ft_lstnew2(char *key, char *value, int equal_sign, \
	t_env *prev);
void			ft_lstadd_back2(t_env **lst, t_env *new);
void			ft_lstdelone2(t_env *lst, void (*del)(void *));
int				ft_lstsize2(t_env *lst);
t_env			*ft_lstlast2(t_env *lst);
void			handle_shlvl(void);

// signals

extern int						g_exit_status;
extern int						g_last_signal;

enum
{
	SET,
	RESET
};

void			sigint_handler(int sig);
void			heredoc_sigint_handler(int sig);
void			handle_default_sig_handlers(int action);
void			reset_default_sig_handlers(void);
void			install_default_sig_handlers(void);

// execution

int				try_exec_env(char **cmd, t_env *env, char *full_path, \
	char **paths);
int				check_dir(char **cmd);
int				try_local_path(char **cmd, t_env *env);
int				execute_from_paths(char **cmd, t_env *env, char *path_var);
void			executor(t_ast *tree, bool forked);
void			free_list(char **list);
pid_t			ft_fork(void);
char			*find_path(t_env *envp);
int				is_dir(char *str);
int				execute_cmd(char **cmd, t_env *env);
bool			check_if_executable(char **cmd, char *exec, char **paths, \
	t_env *env);
bool			check_absolute_path(char **cmd, t_env *env);
char			*check_file_tok(t_token *file_tok);
void			handle_dups(t_ast *sub_tree, int *fd, int fd_num);
void			exec_pipe(t_ast *tree, bool forked);
void			exec_or(t_ast *tree, bool forked);
void			exec_and(t_ast *tree, bool forked);
void			exit_if_forked(bool forked);
void			exec_redir(t_ast_redir *tree, bool forked);
void			exec_exe(t_ast *exe, bool forked);
void			exec_subsh(t_ast *tree, bool forked);
void			wait_and_exit_status(int pid);
void			try_path_execution(char **cmd, char *try_path, t_env *env);
int				try_direct_path(char **cmd, t_env *env);
void			try_execute_file(char **cmd, t_env *env);
void			handle_dir_error(char **cmd);
void			try_local_execution(char **cmd, char *try_path, t_env *env);
void			handle_no_execute_perm(char **cmd);
void			handle_file_not_found(char **cmd);
void			handle_non_exec_path(char **cmd, char *try_path);
void			handle_directory_case(char **cmd);
int				execute_direct_path(char **cmd, t_env *env);
int				check_expanded_length(t_token *file_tok, char **expanded);
int				validate_expanded_token(t_token *file_tok, char **expanded);
int				validate_expanded_args(t_token *file_tok, char **expanded);
void			handle_child_redir(t_ast_redir *tree, int fd_to_dup);
pid_t			ft_fork_wrapper(int *fds_to_close, int num_fds);
int				ft_dup2_wrapper(int oldfd, int newfd);
void			handle_nonforked_redir(t_ast_redir *tree);
void			handle_forked_redir(t_ast_redir *tree);
int				open_redir_file(t_ast_redir *tree, \
	char *file_name, bool forked);
int				setup_builtin_redirection(t_ast_redir *tree, int fd_to_dup, \
	int *old_fd);
int				handle_builtin_redir(t_ast_redir *tree, int fd_to_dup);
bool			prepare_execution(char ***argv, t_ast *exe);
bool			handle_builtins(char **argv, bool forked);
bool			sanitize_argv(char ***argv, t_ast *exe);
int				execute_direct_cmd(char **cmd, t_env *env);

// builtins
int				ft_cd(char *arg, t_env *env, int use_dash_dash);;
char			*join_path_with_dir(char *curr_pwd, char *dir);
char			*construct_manual_path(char *dir, char *curr_pwd);
char			*handle_fallback_case(char *dir, char *curr_pwd);
char			*get_new_pwd(char *dir, char *curr_pwd);
char			*resolve_new_pwd(char *curr_pwd, char *dir);
void			update_pwd(char *curr_pwd, char *new_pwd);
int				handle_new_pwd(char *curr_pwd, char *dir);
int				change_directory(char *dir);
int				check_builtins(int argc, char *command, char **args);

int				ft_echo(int argc, char **args);
size_t			count(const char *s, int c);
int				consist_of(char *line, char c);
int				is_option(char *str);

int				print_env(t_env *env);
int				ft_env(t_env **env);
int				ft_env_with_args(int argc, char **argv, t_env **env);

int				ft_exit(int argc, char *arg);

int				ft_export(int argc, char **argv, t_env **env);
int				handle_concate_case(char *line);
int				is_concate(char *str);
int				print_exports(t_env *env);
int				is_bad_env_name_start(char c);
int				validate_var_name(char *str);
int				ft_is_alphanum_underscore(char *str);
int				validate_arg(char *arg);

int				ft_pwd(void);
char			*trim_path_pwd(char *pwd);
void			shift_slice(char **slices);
char			*pwd(char *str);

int				ft_unset(int argc, char **args, t_env **env);
void			del_from_env(t_env **env, char *key);

// prompt
char			*get_formatted_path(void);
char			*build_prompt_start(void);
void			get_system_info(char *username, char *hostname);
void			get_hostname(char *hostname);
void			get_username(char *username);

#endif
