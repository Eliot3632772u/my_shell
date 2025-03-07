/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:13:09 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/07 07:04:52 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_if_executable(char **cmd, char *exec, char **paths, t_env *env)
{
	char	**envp;

	if (access(exec, X_OK) == 0)
	{
		envp = consume_env(env);
		if (execve(exec, cmd, envp) == -1)
			return (free_list(envp), free_list(paths), true);
	}
	return (false);
}

bool	check_absolute_path(char **cmd, t_env *env)
{
	char	**envp;

	if (ft_strchr(cmd[0], '/'))
	{
		if (access(cmd[0], F_OK) == 0)
		{
			if (access(cmd[0], X_OK) == 0)
			{
				envp = consume_env(env);
				execve(cmd[0], cmd, envp);
				free_list(envp);
				exit(127);
			}
			set_exit_status(126);
			print_err(cmd[0], -69);
			exit(126);
		}
		set_exit_status(127);
		print_err(cmd[0], -1);
		exit(127);
	}
	return (false);
}

static int	validate_expanded_args(t_token *file_tok, char **expanded)
{
	int	len;

	len = split_len(expanded);
	if (len == 0)
	{
		free_list(expanded);
		set_exit_status(1);
		return (0);
	}
	if (len > 1 || (*file_tok->value == '$' && len == 1 && **expanded == '\0'))
	{
		print_err(file_tok->value, -4);
		free_list(expanded);
		return (0);
	}
	return (1);
}

char	*check_file_tok(t_token *file_tok)
{
	char	**expanded;
	char	*file_name;

	file_name = NULL;
	expanded = expand_args(file_tok);
	if (!expanded)
	{
		set_exit_status(1);
		return (NULL);
	}
	if (!validate_expanded_args(file_tok, expanded))
		return (NULL);
	file_name = ft_strdup(*expanded);
	free_list(expanded);
	return (file_name);
}

int	try_direct_path(char **cmd, t_env *env)
{
	char	*try_path;

	check_dir(cmd);
	if (check_absolute_path(cmd, env))
		return (-1);
	try_path = ft_strjoin("./", cmd[0]);
	if (access(try_path, F_OK) == 0)
	{
		if (access(try_path, X_OK) == 0)
		{
			execve(try_path, cmd, consume_env(env));
			free(try_path);
			return (-1);
		}
		free(try_path);
		set_exit_status(126);
		print_err(cmd[0], -69);
		exit(126);
	}
	free(try_path);
	return (0);
}
