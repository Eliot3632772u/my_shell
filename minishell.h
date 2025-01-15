/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-11 09:03:01 by irabhi            #+#    #+#             */
/*   Updated: 2025-01-11 09:03:01 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>


typedef struct      s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

// add enum type

typedef struct      s_token
{
	char			*type; // change to enum type
	void			*token_struct;
	struct s_token	*next;
}			 		t_token;

/*
* LIBFT 
*/
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *src);
size_t	ft_strlen(const char *str);
void	ft_putchar_fd(char c, int fd);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strchr(const char *s, int c);

/*
* UTILS
*/
void	free_exit(t_env **env, char **env_vars, int status);
void	free_arr(char **env_vars);
int		push_front(t_env **env, char *name, char *value);

/*
* LEX_UTILS
*/
int		is_spaces(char c);
int		is_special(char c);

#endif