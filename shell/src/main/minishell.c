/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 08:48:23 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/20 20:49:57 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	main(int _, char **__, char **envp)
{
	char			*command_line;
	char			*prompt;
	struct termios	attrs[3];

	if (_ > 1)
		return (printf("Not valid Arguments\n"), 127);
	if (!isatty(STDIN_FILENO))
	{
		write(2, "minishell: cannot run in non-interactive mode\n", 47);
		exit(1);
	}
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
