/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 08:48:23 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 08:48:27 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
