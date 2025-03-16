/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:57:37 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 17:34:42 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_shell_level(void)
{
	t_env	*env_node;
	int		shlvl;

	env_node = search_in_env(get_envp(NULL), "SHLVL");
	if (env_node && env_node->value)
		shlvl = ft_atoi(env_node->value);
	else
		shlvl = 1;
	return (shlvl);
}

static void	handle_nested_shell(void)
{
	set_exit_status(130);
	g_last_signal = 69;
}

static void	handle_parent_shell(void)
{
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	set_exit_status(130);
	g_last_signal = 69;
}

void	sigint_handler(int sig)
{
	int	status;
	int	shlvl;

	(void)sig;
	shlvl = get_shell_level();
	if (shlvl > 2)
		handle_nested_shell();
	else if (waitpid(-1, &status, WNOHANG) > 0)
		g_last_signal = 0;
	else
		handle_parent_shell();
}

void	handle_default_sig_handlers(int action)
{
	if (action == SET)
	{
		g_last_signal = 0;
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		g_last_signal = 0;
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}
