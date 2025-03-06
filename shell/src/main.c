/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:07:26 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/06 08:26:53 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_last_signal;

void	tty_attr(struct termios *attrs, int action)
{
	if (action == ATTR_GET)
	{
		tcgetattr(STDIN_FILENO, &attrs[0]);
		tcgetattr(STDOUT_FILENO, &attrs[1]);
		tcgetattr(STDERR_FILENO, &attrs[2]);
	}
	else if (action == ATTR_SET)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &attrs[0]);
		tcsetattr(STDOUT_FILENO, TCSANOW, &attrs[1]);
		tcsetattr(STDERR_FILENO, TCSANOW, &attrs[2]);
	}
	else if (action == ATTR_CHG)
	{
		attrs[0].c_lflag &= ~ECHOCTL;
		attrs[1].c_lflag &= ~ECHOCTL;
		attrs[2].c_lflag &= ~ECHOCTL;
		tty_attr(attrs, ATTR_SET);
	}
}

void	setup(char **envp, struct termios *attrs, ...)
{
	char	*tmp;

	create_env(envp);
	tmp = getcwd(NULL, 0);
	rl_catch_signals = false;
	pwd(tmp);
	free(tmp);
	install_default_sig_handlers();
	tty_attr(attrs, ATTR_GET);
	tty_attr(attrs, ATTR_CHG);
}

bool	run(char *command_line)
{
	t_token			*tokens;
	t_ast_cmd		*ast;

	if (!command_line)
	{
		ft_putendl_fd("exit", 1);
		return (free(command_line), true);
	}
	lexer(command_line, &tokens);
	if (parser(tokens, &ast))
		executor(ast, false);
	else
	{
		dup2(2, 0);
		g_last_signal = 0;
	}
	if (*command_line)
	{
		add_history(command_line);
	}
	return (free_ast(ast), free(command_line), false);
}

int	main(int _, char **__, char **envp)
{
	char			*command_line;
	struct termios	attrs[3];

	if (_ > 1)
		return (printf("Not valid Arguments\n"), 127);
	setup(envp, attrs, _, __);
	while (true)
	{
		command_line = readline("Minishell$ ");
		if (run(command_line))
			break ;
		g_last_signal = 0;
		tty_attr(attrs, ATTR_SET);
	}
	exit(get_exit_status());
}
