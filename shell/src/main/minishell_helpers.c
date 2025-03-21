/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:07:26 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/20 21:56:08 by yrafai           ###   ########.fr       */
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

void	set_up_token_prev(t_token *tokens)
{
	t_token		*prev;

	if (tokens == NULL)
		return ;
	prev = NULL;
	while (tokens)
	{
		tokens->prev = prev;
		prev = tokens;
		tokens = tokens->next;
	}
}

void	setup(char **envp, struct termios *attrs, ...)
{
	char	*tmp;

	create_env(envp);
	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		perror("getcwd failed");
		exit(EXIT_FAILURE);
	}
	rl_catch_signals = false;
	pwd(tmp);
	free(tmp);
	install_default_sig_handlers();
	tty_attr(attrs, ATTR_GET);
	tty_attr(attrs, ATTR_CHG);
}

bool	process_command_2(char *command_line)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = NULL;
	lexer2(command_line, &tokens);
	set_up_token_prev(tokens);
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
	free_ast2(ast);
	return (false);
}

bool	run(char *command_line)
{
	bool	result;

	if (!command_line)
	{
		ft_putendl_fd("exit", 1);
		free(command_line);
		return (true);
	}
	result = process_command_2(command_line);
	if (*command_line)
		add_history(command_line);
	free(command_line);
	return (result);
}
