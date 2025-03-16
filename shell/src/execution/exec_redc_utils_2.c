/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redc_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 08:42:32 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 08:43:11 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_file_with_error_handling(t_redirect *redc, char *file)
{
	int	fd;

	errno = 0;
	fd = open(file, redc->mode, 0644);
	if (fd == -1 && errno == ENOENT)
	{
		write(2, "Minishell: ", 11);
		write(2, file, ft_strlen(file));
		perror("");
		if (redc->type != HEREDOC)
			free(file);
		set_exit_status(1);
		return (-1);
	}
	return (fd);
}

int	open_dup_wrap(t_redirect *redc, char *file, int STD_FD)
{
	int	fd;

	fd = open_file_with_error_handling(redc, file);
	if (fd == -1)
		return (1);
	if (dup2(fd, STD_FD) == -1)
	{
		perror("Minishell: dup2");
		set_exit_status(1);
		close(fd);
		if (redc->type != HEREDOC)
			free(file);
		return (1);
	}
	close(fd);
	if (redc->type != HEREDOC)
		free(file);
	return (0);
}
