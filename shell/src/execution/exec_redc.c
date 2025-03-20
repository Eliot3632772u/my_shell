/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:49:12 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/20 20:52:23 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	heredoc_handling(t_redirect *redc)
{
	char	*file;
	int		fd;

	if (exec_heredoc(redc, &file))
		return (-1);
	redc->file_tok->value = file;
	fd = open(file, O_RDONLY);
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		if (fd != -1)
			close(fd);
		perror("Minishell: dup2");
		set_exit_status(1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	open_file(t_redirect *redc)
{
	char	*file;
	int		std_fd;

	std_fd = STDIN_FILENO;
	if (redc->type == HEREDOC || redc->type == HEREDOC_TAB)
		return (heredoc_handling(redc));
	if (expand_redc_file(redc, &file))
		return (-1);
	if (redc->type == APPEND || redc->type == OUTPUT)
		std_fd = STDOUT_FILENO;
	if (open_dup_wrap(redc, file, std_fd))
		return (-1);
	return (0);
}

int	expand_redc_file(t_redirect *redc, char **file)
{
	*file = check_file_tok(redc->file_tok);
	if (*file == NULL || **file == '\0')
	{
		if (*file)
			free(*file);
		if (!get_exit_status())
			write(2, "Minishell: : No such file or directory\n", 39);
		return (set_exit_status(1), 1);
	}
	return (0);
}

void	handle_redc(t_redirect *redc)
{
	int			error_occurred;
	t_redirect	*current;

	save_fd(SET);
	current = redc;
	error_occurred = 0;
	while (current && !error_occurred)
	{
		if (open_file(current) == -1)
		{
			set_exit_status(1);
			error_occurred = 1;
			break ;
		}
		current = current->next;
	}
	if (error_occurred)
		save_fd(RESET);
}

int	exec_redc(t_redirect *redc, int mode)
{
	if (!redc)
		return (0);
	if (mode == SET)
	{
		set_exit_status(0);
		handle_redc(redc);
	}
	else if (mode == RESET)
		save_fd(RESET);
	return (get_exit_status() == 1);
}
