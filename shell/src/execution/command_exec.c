/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:44:13 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 17:38:07 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_from_paths(char **cmd, t_env *env, char *path_var)
{
	char	**paths;
	char	**tmp;
	char	*full_path;
	int		result;

	paths = ft_split(path_var, ':');
	if (!paths)
		return (-1);
	tmp = paths;
	while (*tmp)
	{
		full_path = ft_strjoin(*tmp, "/");
		result = try_exec_env(cmd, env, full_path, paths);
		if (result == 0)
			return (0);
		tmp++;
	}
	free_list(paths);
	return (-1);
}

int	execute_cmd(char **cmd, t_env *env)
{
	char	*path_var;

	if (!cmd[0] || !cmd[0][0])
	{
		set_exit_status(127);
		print_err("", -1);
		exit(127);
	}
	check_dir(cmd);
	if (try_direct_path(cmd, env) == 0)
		return (0);
	path_var = find_path(env);
	if (!path_var)
	{
		if (!ft_strchr(cmd[0], '/') && try_local_path(cmd, env) == 0)
			return (0);
		set_exit_status(127);
		print_err(cmd[0], -1);
		exit(127);
	}
	if (execute_from_paths(cmd, env, path_var) == 0)
		return (0);
	set_exit_status(127);
	print_err(cmd[0], -1);
	exit(127);
}

int	execute_direct_cmd(char **cmd, t_env *env)
{
	char	**envp;
	char	*sh_argv[3];

	envp = consume_env(env);
	execve(cmd[0], cmd, envp);
	if (errno == ENOEXEC)
	{
		sh_argv[0] = "/bin/sh";
		sh_argv[1] = cmd[0];
		sh_argv[2] = NULL;
		execve("/bin/sh", sh_argv, envp);
	}
	free_list(envp);
	return (-1);
}

int	execute_direct_path(char **cmd, t_env *env)
{
	struct stat	file_stat;
	char		**envp;

	if (stat(cmd[0], &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode) && !ft_strchr(cmd[0], '/'))
			handle_dir_error(cmd);
		if (!S_ISDIR(file_stat.st_mode) && !ft_strchr(cmd[0], '/'))
		{
			if (access(cmd[0], X_OK) == 0)
			{
				envp = consume_env(env);
				execve(cmd[0], cmd, envp);
				free_list(envp);
				return (-1);
			}
			handle_dir_error(cmd);
		}
	}
	return (0);
}

void	exec_exe(t_ast *exe, bool forked)
{
	char	**argv;
	pid_t	pid;
	t_env	*envp;

	// set_exit_status(0);
	if (exec_redc(exe->redc, SET))
		return ((void)(exec_redc(exe->redc, RESET)));
	if (!prepare_execution(&argv, exe))
		return ((void)(exec_redc(exe->redc, RESET)));
	envp = get_envp(NULL);
	if (handle_builtins(argv, forked))
		return ((void)(exec_redc(exe->redc, RESET)));
	if (!forked)
		pid = ft_fork();
	if (forked || !pid)
	{
		reset_default_sig_handlers();
		print_err(argv[0], execute_cmd(argv, envp));
		exit(get_exit_status());
	}
	wait_and_exit_status(pid);
	exec_redc(exe->redc, RESET);
	free_list(argv);
	exit_if_forked(forked);
}
