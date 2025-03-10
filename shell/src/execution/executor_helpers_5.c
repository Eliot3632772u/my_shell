/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_5.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:47:58 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/09 23:28:10 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	shift_argv(char ***argv)
{
	int	i;

	i = 0;
	free((*argv)[0]);
	while ((*argv)[i + 1] != NULL)
	{
		(*argv)[i] = (*argv)[i + 1];
		i++;
	}
	(*argv)[i] = NULL;
}

static bool	sanitize_argv(char ***argv, t_ast_exec *exe)
{
	char	*first_arg;
	t_token	*tok;

	first_arg = (*argv)[0];
	tok = exe->argv_tok;
	if (first_arg && ft_strchr(first_arg, ' ') && tok->type != STR \
		&& tok->type != DQSTR)
	{
		shift_argv(argv);
		if ((*argv)[0] == NULL)
		{
			print_err(tok->value, -1);
			set_exit_status(127);
			free_list(*argv);
			return (false);
		}
	}
	return (true);
}

static bool	handle_builtins(char **argv, bool forked)
{
	if (check_builtins(split_len(argv) - 1, argv[0], argv + 1))
	{
		exit_if_forked(forked);
		free_list(argv);
		return (true);
	}
	return (false);
}

static bool	prepare_execution(char ***argv, t_ast_exec *exe)
{
	*argv = expand_args(exe->argv_tok);
	if (!*argv || !(*argv)[0])
	{
		free_list(*argv);
		return (false);
	}
	if (!sanitize_argv(argv, exe))
		return (false);
	log_last_command((*argv)[split_len(*argv) - 1]);
	return (true);
}

void	exec_exe(t_ast_exec *exe, bool forked)
{
	char	**argv;
	pid_t	pid;
	t_env	*envp;

	if (!prepare_execution(&argv, exe))
		return ;
	envp = get_envp(NULL);
	if (handle_builtins(argv, forked))
		return ;
	if (!forked)
		pid = ft_fork();
	if (forked || !pid)
	{
		reset_default_sig_handlers();
		print_err(argv[0], execute_cmd(argv, envp));
		exit(get_exit_status());
	}
	wait_and_exit_status(pid);
	free_list(argv);
	exit_if_forked(forked);
}
