/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:03:07 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/14 12:08:42 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_dir(char *str)
{
	DIR		*ptr;
	int		res;

	if (!str)
		return (0);
	ptr = opendir(str);
	res = !!ptr;
	if (ptr)
		closedir(ptr);
	return (res);
}

int	check_dir(char **cmd)
{
	if (!ft_strcmp(cmd[0], "."))
	{
		set_exit_status(2);
		print_err(cmd[0], -69);
		exit(2);
	}
	if (!ft_strcmp(cmd[0], ".."))
	{
		set_exit_status(127);
		print_err(cmd[0], -1);
		exit(127);
	}
	if (is_dir(cmd[0]))
	{
		if (ft_strchr(cmd[0], '/'))
		{
			set_exit_status(126);
			print_err(cmd[0], -69);
			exit(126);
		}
		set_exit_status(127);
		print_err(cmd[0], -1);
		exit(127);
	}
	return (0);
}

char	*check_file_tok(t_token *file_tok)
{
	char	**expanded;
	char	*file_name;

	file_name = NULL;
	expanded = expand_args(file_tok);
	if (ft_strchr(file_tok->value, '$') && ft_strcmp(file_tok->value, "$") != 0)
	{
		if (!expanded)
		{
			print_err(file_tok->value, -4);
			set_exit_status(1);
			return (NULL);
		}
	}
	else if (!expanded)
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

void	free_list(char **list)
{
	unsigned int	i;

	if (!list)
		return ;
	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
}

int	try_exec_env(char **cmd, t_env *env, char *full_path,
	char **paths)
{
	char	*exec_path;
	char	**envp;

	exec_path = ft_strjoin(full_path, cmd[0]);
	free(full_path);
	if (access(exec_path, X_OK) == 0)
	{
		envp = consume_env(env);
		execve(exec_path, cmd, envp);
		free_list(envp);
		free(exec_path);
		free_list(paths);
		return (0);
	}
	free(exec_path);
	return (-1);
}
