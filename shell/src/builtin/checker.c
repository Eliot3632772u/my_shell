#include "../../includes/minishell.h"

int	handle_exit_status(int used, int res)
{
	if (res == 69)
		used = 0;
	if (used)
		set_exit_status(res);
	return (used);
}

int	check_builtins(int argc, char *command, char **args)
{
	int	used;
	int	res;

	if (!command)
		return (0);
	res = 69;
	used = 1;
	if (!ft_strcmp(command, "echo"))
		res = ft_echo(argc, args);
	else if (!ft_strcmp(command, "cd"))
		res = ft_cd(*args, get_envp(NULL));
	else if (!ft_strcmp(command, "pwd"))
		res = ft_pwd();
	else if (!ft_strcmp(command, "export"))
		res = ft_export(argc, args, get_envp_internal(NULL));
	else if (!ft_strcmp(command, "unset"))
		res = ft_unset(argc, args, get_envp_internal(NULL));
	else if (!ft_strcmp(command, "exit"))
		res = ft_exit(argc, *args);
	else if (!ft_strcmp(command, "env"))
		res = ft_env(get_envp_internal(NULL));
	return (handle_exit_status(used, res));
}