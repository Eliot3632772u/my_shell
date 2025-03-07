/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:57:34 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/07 07:06:42 by yrafai           ###   ########.fr       */
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
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <termios.h>
# include <sys/utsname.h>
# include <limits.h>

# define GET_EXIT 0xdeadbeef
# define TROLL 132
# define DEFAULT_PATH "/usr/local/sbin:/usr/local/\
bin:/usr/sbin:/usr/bin:/sbin:/bin"
# define SHLVL_WARN "minishell: warning: shell level\
 (1000) too high, resetting to 1"

# ifndef ECHOCTL
#  define ECHOCTL 0x00000040
# endif

// Forward declarations
typedef struct s_cmd		t_ast_cmd;
typedef struct s_ast_redir	t_ast_redir;

# define COLOR_CYAN "\001\033[1;36m\002"
# define COLOR_RED "\001\033[1;31m\002"
# define COLOR_BLUE "\001\033[1;34m\002"
# define COLOR_WHITE "\001\033[1;37m\002"
# define COLOR_RESET "\001\033[0m\002"

// Prompt function
char			*get_prompt(void);

// lexer

typedef enum e_token_type
{
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
}		t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				len;
	bool			to_expand;
	struct s_token	*nospace_next;
	struct s_token	*next;
}			t_token;

typedef struct s_heredoc_opts
{
	bool	expandable;
	bool	strip_tabs;
}				t_heredoc_opts;

bool			lexer(char *command_line, t_token **tokens);
bool			check_token(t_token *this_tok, char *cmd_line, \
	int index_space[2], t_token **tokens);
void			get_token(char *where, t_token *tok);
t_token			*new_token(t_token_type type, char *value, int len);
t_token_type	token_type(char *str);
int				count_with_func(char *str, bool (*test_func)(char));
int				token_len(t_token_type type, char *str);
bool			check_expanding(char *str, t_token_type type);
bool			valid_env_char(char c);
int				get_exit_status(void);
void			set_exit_status(unsigned int status);
int				exit_status(unsigned int action);
void			tok_error(char err);
void			unclosed_error(char err);
void			syntax_error(char *err);
int				print_err(char *preced, int msg_code);
void			free_tok_lst(t_token *tok);
char			*handle_heredoc(char *delim, t_heredoc_opts opts);
char			*ft_mktmp(void);
int				ft_heredoc(t_ast_cmd *tree);
int				patch_token(t_ast_redir *tree);
bool			is_expandable(t_token *tok);
t_token			*ft_toklast(t_token *lst);
void			ft_tokadd_back(t_token **lst, t_token *new_tok);
void			ft_tokiter(t_token *lst, void (*f)(void *));
void			add_to_sublist(t_token **list, t_token *new_tok);
bool			add_token(t_token **tokens, t_token *token, bool is_space);
bool			handle_heredoc_line(char *line, char *delim, \
	int fd, t_heredoc_opts opts);
int				init_heredoc(char *delim, char **tmp_file, int *fd);
char			*strip_leading_tabs(char *line);
bool			is_expandable(t_token *tok);
bool			is_delimiter(char *line, char *delim, t_heredoc_opts opts);
void			process_and_write_line(char *line, int fd, t_heredoc_opts opts);
size_t			count_leading_tabs(char *line);
bool			is_empty_quoted_string(char *str);
char			*strip_quotes(char *str);
void			handle_heredoc_signal(void);
int				process_heredoc_tree(t_ast_cmd *tree);

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

void			update_token_after_heredoc(t_token *tok, t_ast_redir *tree, \
	t_str *s_ptr);
char			*get_processed_delimiter(t_ast_redir *tree, t_str **s_ptr);
t_str			*expand_wild_cards(t_str *argv_lst);
void			wild_card(t_str **lst, t_str *expr);
bool			wild_match(char *str, char *expr);
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

extern int					g_exit_status;
extern int					g_last_signal;

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

int				try_direct_path(char **cmd, t_env *env);
int				check_dir(char **cmd);
void			executor(t_ast_cmd *tree, bool forked);
void			free_list(char **list);
pid_t			ft_fork(void);
char			*find_path(t_env *envp);
int				is_dir(char *str);
int				execute_cmd(char **cmd, t_env *env);
bool			check_if_executable(char **cmd, char *exec, char **paths, \
	t_env *env);
bool			check_absolute_path(char **cmd, t_env *env);
char			*check_file_tok(t_token *file_tok);
void			handle_dups(t_ast_cmd *sub_tree, int *fd, int fd_num);
void			exec_pipe(t_ast_binary *tree, bool forked);
void			exec_or(t_ast_binary *tree, bool forked);
void			exec_and(t_ast_binary *tree, bool forked);
void			exit_if_forked(bool forked);
void			exec_redir(t_ast_redir *tree, bool forked);
void			exec_exe(t_ast_exec *exe, bool forked);
void			exec_subsh(t_ast_subsh *tree, bool forked);
void			wait_and_exit_status(int pid);

// builtins
int				ft_cd(char *arg, t_env *env);
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

#endif
