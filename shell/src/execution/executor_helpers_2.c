/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:13:09 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 09:48:58 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_if_executable(char **cmd, char *exec, char **paths, t_env *env)
{
	char	**envp;

	if (access(exec, X_OK) == 0)
	{
		envp = consume_env(env);
		execve(exec, cmd, envp);
		free_list(envp);
		free_list(paths);
	}
	return (false);
}

bool	check_absolute_path(char **cmd, t_env *env)
{
	if (ft_strchr(cmd[0], '/'))
	{
		try_execute_file(cmd, env);
		return (true);
	}
	return (false);
}

int	check_expanded_length(t_token *file_tok, char **expanded)
{
	int	len;

	len = split_len(expanded);
	if (len == 0)
	{
		free_list(expanded);
		print_err(file_tok->value, -4);
		set_exit_status(1);
		return (0);
	}
	if (len > 1)
	{
		print_err(file_tok->value, -4);
		free_list(expanded);
		return (0);
	}
	return (1);
}

int	validate_expanded_token(t_token *file_tok, char **expanded)
{
	if (ft_strchr(file_tok->value, '$') && ft_strcmp(file_tok->value, "$") != 0)
	{
		if (expanded[0][0] == '\0' || !ft_strcmp(expanded[0], file_tok->value))
		{
			print_err(file_tok->value, -4);
			free_list(expanded);
			return (0);
		}
	}
	return (1);
}

int	validate_expanded_args(t_token *file_tok, char **expanded)
{
	if (!check_expanded_length(file_tok, expanded))
		return (0);
	return (validate_expanded_token(file_tok, expanded));
}
