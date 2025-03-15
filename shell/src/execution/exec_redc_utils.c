/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:49:02 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/15 17:51:02 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void save_fd(int flag)
{
    static int fd_in = -1;
    static int fd_out = -1;

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

int proc_herdoc_file2(int fd, char *file, char *result)
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

int proc_heredoc_file(int fd, char *file)
{
    char    *tmp;
    char    *result;
    char    *line;

    tmp = NULL;
    result = NULL;
    line = get_next_line(fd);
    while (line)
    {
        tmp = expand_env(line, false, 0);
        line = result;
        result = ft_strjoin(result, tmp);
        if (line)
            free(line);
        free(tmp);
        if (result == NULL)
            return (1);
        line = get_next_line(fd);
    }
    return(proc_herdoc_file2(fd, file, result));
}

int exec_heredoc(t_redirect *redc, char **file)
{
    int     fd;

    *file = redc->file_tok->value;
    if (redc->expand == 0)
        return (0);
    fd = open(redc->file_tok->value, O_RDWR);
    if (fd == -1)
        return (1);
    return (proc_heredoc_file(fd, *file));
}

int open_dup_wrap(t_redirect *redc,char *file, int STD_FD)
{
    int     fd;

    errno = 0;
    fd = open(file, redc->mode, 0644);
    if (redc->type != HEREDOC)
        free(file);
    if (fd == -1)
    {
        if (errno == ENOENT)
            return (write(2, "Minishell: ", 11), write(2, file,\
             ft_strlen(file)), perror(""), set_exit_status(1), 1);
    }
    if (dup2(fd, STD_FD) == -1)
        return (perror("Minishell: dup2"), set_exit_status(1), close(fd), 1);
    close(fd);
    return (0);
}