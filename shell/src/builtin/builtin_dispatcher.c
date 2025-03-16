/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:06:01 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 22:05:41 by irabhi           ###   ########.fr       */
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

static char	*join_cd_args(char **args)
{
	char	*result;
	char	*temp;

	if (!args || !args[0])
		return (NULL);
	if (ft_strcmp(args[0], "--") == 0)
	{
		if (!args[1])
			return (NULL);
		return (ft_strdup(args[1]));
	}
	if (!args[1])
		return (ft_strdup(args[0]));
	result = ft_strdup(args[0]);
	if (!result)
		return (NULL);
	temp = ft_strjoin(result, " ");
	free(result);
	return (temp);
}

static int	handle_cd(char **args)
{
	char	*cd_arg;
	int		result;
	int		use_dash_dash;

	use_dash_dash = 0;
	if (args && args[0] && ft_strcmp(args[0], "--") == 0)
		use_dash_dash = 1;
	cd_arg = join_cd_args(args);
	result = ft_cd(cd_arg, get_envp(NULL), use_dash_dash);
	free(cd_arg);
	return (result);
}

static int	execute_builtin(int argc, char *command, char **args)
{
	if (!ft_strcmp(command, "echo"))
		return (ft_echo(argc, args));
	if (!ft_strcmp(command, "cd"))
		return (handle_cd(args));
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
