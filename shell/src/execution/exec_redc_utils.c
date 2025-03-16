/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:49:02 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/16 21:48:24 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	save_fd(int flag)
{
	static int	fd_in = -1;
	static int	fd_out = -1;

	if (flag == SET)
	{
		fd_in = dup(STDIN_FILENO);
		fd_out = dup(STDOUT_FILENO);
	}
	else
	{
		if (fd_in > 0)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
			fd_in = -1;
		}
		if (fd_out > 0)
		{
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
			fd_out = -1;
		}
	}
}

int	proc_herdoc_file2(int fd, char *file, char *result)
{
	close(fd);
	unlink(file);
	fd = open(file, O_RDWR | O_CREAT, 0644);
	if (fd == -1)
		return (1);
	ft_putstr_fd(result, fd);
	close(fd);
	return (0);
}

char	*read_and_expand_lines(int fd)
{
	char	*tmp;
	char	*result;
	char	*line;
	char	*old_result;

	result = NULL;
	line = get_next_line(fd);
	while (line)
	{
		tmp = expand_env(line, false, 0);
		old_result = result;
		result = ft_strjoin(result, tmp);
		free(old_result);
		free(tmp);
		if (result == NULL)
			return (NULL);
		line = get_next_line(fd);
	}
	if (result)
		return (result);
	return (ft_strdup(""));
}

int	proc_heredoc_file(int fd, char *file)
{
	char	*result;
	int		ret;

	result = read_and_expand_lines(fd);
	if (!result)
		return (1);
	ret = proc_herdoc_file2(fd, file, result);
	free(result);
	return (ret);
}

int	exec_heredoc(t_redirect *redc, char **file)
{
	int	fd;

	*file = redc->file_tok->value;
	if (redc->expand == 0)
		return (0);
	fd = open(redc->file_tok->value, O_RDWR);
	if (fd == -1)
		return (1);
	return (proc_heredoc_file(fd, *file));
}
