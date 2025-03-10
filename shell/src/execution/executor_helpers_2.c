/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:13:09 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 05:51:21 by yrafai           ###   ########.fr       */
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
	char		*try_path;
	struct stat	file_stat;

	check_dir(cmd);
	if (check_absolute_path(cmd, env))
		return (-1);
	if (stat(cmd[0], &file_stat) == 0 && S_ISDIR(file_stat.st_mode)
		&& !ft_strchr(cmd[0], '/'))
		handle_dir_error(cmd);
	if (stat(cmd[0], &file_stat) == 0 && !S_ISDIR(file_stat.st_mode)
		&& !ft_strchr(cmd[0], '/'))
		handle_dir_error(cmd);
	try_path = ft_strjoin("./", cmd[0]);
	if (access(try_path, F_OK) == 0)
	{
		try_local_execution(cmd, try_path, env);
		return (-1);
	}
	free(try_path);
	return (0);
}
