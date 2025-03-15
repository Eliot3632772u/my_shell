/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:07:26 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 17:14:01 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_last_signal;

char	*get_prompt(void)
{
	char	*path;
	char	*prompt;
	char	*start;
	char	*temp;

	start = build_prompt_start();
	path = get_formatted_path();
	prompt = ft_strjoin(start, path);
	free(start);
	free(path);
	temp = prompt;
	prompt = ft_strjoin(temp, COLOR_CYAN "]\n└─" COLOR_WHITE "$ " COLOR_RESET);
	free(temp);
	return (prompt);
}

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
	t_token		*tokens;
	t_ast	*ast;

	ast = NULL;
	tokens = NULL;
	if (!command_line)
	{
		ft_putendl_fd("exit", 1);
		return (free(command_line), true);
	}
	lexer2(command_line, &tokens);
	ast = parser2(&tokens);
	if (ast)
	{
		if (g_last_signal != 420 && ft_heredoc(ast))
			executor(ast, false);
		cleanup_all_heredoc_files();
	}
	else
	{
		dup2(2, 0);
		g_last_signal = 0;
	}
	if (*command_line)
	{
		add_history(command_line);
	}
	return (free_ast2(ast), free(command_line), false);
}

int	main(int _, char **__, char **envp)
{
	char			*command_line;
	char			*prompt;
	struct termios	attrs[3];

	if (_ > 1)
		return (printf("Not valid Arguments\n"), 127);
	setup(envp, attrs, _, __);
	while (true)
	{
		prompt = get_prompt();
		command_line = readline(prompt);
		free(prompt);
		if (run(command_line))
			break ;
		g_last_signal = 0;
		tty_attr(attrs, ATTR_SET);
	}
	exit(get_exit_status());
}
