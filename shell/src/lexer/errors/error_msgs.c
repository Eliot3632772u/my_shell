/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msgs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 01:07:52 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/13 11:05:14 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	tok_error(char err)
{
	set_exit_status(2);
	write(2, "Minishell: unexpected token `", 29);
	write(2, &err, 1);
	write(2, "'\n", 2);
}

void	unclosed_error(char err)
{
	set_exit_status(2);
	write(2, "Minishell: unclosed quote `", 27);
	write(2, &err, 1);
	write(2, "'\n", 2);
}

void	syntax_error(char *err)
{
	set_exit_status(2);
	write(2, "Minishell: syntax error near unexpected token `", 47);
	ft_putstr_fd(err, 2);
	write(2, "'\n", 2);
}

static void	handle_err_messages(t_strbuilder *sb, int msg_code)
{
	if (msg_code == -1)
		(sb_append(sb, ": command not found\n"), set_exit_status(127));
	else if (msg_code == -69)
		(sb_append(sb, ": Is a directory\n"), set_exit_status(126));
	else if (msg_code == -2)
		(sb_append(sb, ": allocation error\n"), set_exit_status(255));
	else if (msg_code == -3)
		(sb_append(sb, ": $PATH var unset\n"), set_exit_status(127));
	else if (msg_code == -4)
		(sb_append(sb, ": ambiguous redirect\n"), set_exit_status(1));
	else if (msg_code == -5)
		(sb_append(sb, ": No such file or directory\n"), set_exit_status(127));
	else if (msg_code == -6)
		(sb_append(sb, ": numeric argument required\n"), set_exit_status(255));
	else if (msg_code == -7)
		(sb_append(sb, ": Permission denied\n"), set_exit_status(126));
}

int	print_err(char *preced, int msg_code)
{
	t_strbuilder	*sb;

	sb = stringbuilder();
	sb_append(sb_append(sb, "Minishell: "), preced);
	handle_err_messages(sb, msg_code);
	if (msg_code == 0)
		perror(sb->str);
	else
		write(2, sb->str, ft_strlen(sb->str));
	sb_free(sb);
	return (msg_code);
}
