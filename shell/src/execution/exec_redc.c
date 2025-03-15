/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:49:12 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/15 17:49:14 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int open_file(t_redirect *redc)
{
    char *file;
    int STD_FD;

    STD_FD = STDIN_FILENO;
    if (redc->type == HEREDOC)
    {
        if (exec_heredoc(redc, &file))
            return (-1);
    }
    else if (expand_redc_file(redc, &file))
        return (-1);
    if (redc->type == APPEND || redc->type == OUTPUT)
        STD_FD = STDOUT_FILENO;
    if (open_dup_wrap(redc, file, STD_FD))
        return (-1);
    return (0);
}

int expand_redc_file(t_redirect *redc, char **file)
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

void handle_redc(t_redirect *redc)
{
    int error_occurred;
    t_redirect *current;

    save_fd(SET);
    current = redc;
    error_occurred = 0;
    while (current && !error_occurred)
    {
        if (open_file(current) == -1)
        {
            error_occurred = 1;
            break;
        }
        current = current->next;
    }
    if (error_occurred)
        save_fd(RESET);
}

int exec_redc(t_redirect *redc, int mode)
{
    if (!redc || get_exit_status())
        return (0);
    if (mode == SET)
        handle_redc(redc);
    else if (mode == RESET)
        save_fd(RESET);

    return (get_exit_status() == 1);
}
