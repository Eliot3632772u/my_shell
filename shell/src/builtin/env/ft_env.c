/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:05:02 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 11:19:42 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	print_env(t_env *env)
{
	t_env	*ptr;

	if (!env)
		return (1);
	ptr = env;
	while (ptr)
	{
		if (ptr->equal_sign && ptr->value)
			printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return (0);
}

int	ft_env(t_env **env)
{
	return (print_env(*env));
}

static void	execute_env_command(char **argv, t_env *env)
{
	char	**env_array;

	env_array = consume_env(env);
	if (!env_array)
		exit(1);
	if (ft_strchr(argv[0], '/'))
	{
		execve(argv[0], argv, env_array);
		print_err(argv[0], 0);
	}
	else
	{
		if (execute_cmd(argv, env) == -1)
			print_err(argv[0], 0);
	}
	free_list(env_array);
	exit(127);
}

int	ft_env_with_args(int argc, char **argv, t_env **env)
{
	pid_t	pid;

	if (!env || !*env)
		return (1);
	if (argc == 0)
		return (print_env(*env));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		execute_env_command(argv, *env);
	else
	{
		wait_and_exit_status(pid);
		return (get_exit_status());
	}
	return (0);
}
