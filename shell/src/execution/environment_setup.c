/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_8.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:47:36 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 11:00:14 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	try_direct_path(char **cmd, t_env *env)
{
	char	*try_path;

	check_dir(cmd);
	if (check_absolute_path(cmd, env))
		return (-1);
	if (execute_direct_path(cmd, env) == -1)
		return (-1);
	try_path = ft_strjoin("./", cmd[0]);
	if (access(try_path, F_OK) == 0)
	{
		try_local_execution(cmd, try_path, env);
		return (-1);
	}
	free(try_path);
	return (0);
}
