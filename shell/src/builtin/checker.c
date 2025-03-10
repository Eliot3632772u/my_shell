/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:06:01 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 11:06:40 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_exit_status(int used, int res)
{
	if (res == 69)
		used = 0;
	if (used)
		set_exit_status(res);
	return (used);
}

static int	execute_builtin(int argc, char *command, char **args)
{
	if (!ft_strcmp(command, "echo"))
		return (ft_echo(argc, args));
	if (!ft_strcmp(command, "cd"))
	{
		if (argc > 1)
		{
			ft_putendl_fd("cd: too many arguments", 2);
			return (1);
		}
		return (ft_cd(*args, get_envp(NULL)));
	}
	if (!ft_strcmp(command, "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(command, "export"))
		return (ft_export(argc, args, get_envp_internal(NULL)));
	if (!ft_strcmp(command, "unset"))
		return (ft_unset(argc, args, get_envp_internal(NULL)));
	if (!ft_strcmp(command, "exit"))
		return (ft_exit(argc, *args));
	if (!ft_strcmp(command, "env"))
		return (ft_env_with_args(argc, args, get_envp_internal(NULL)));
	return (69);
}

int	check_builtins(int argc, char *command, char **args)
{
	int	used;
	int	res;

	used = 1;
	if (!command)
		return (0);
	res = execute_builtin(argc, command, args);
	return (handle_exit_status(used, res));
}
