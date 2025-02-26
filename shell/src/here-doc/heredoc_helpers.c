#include "../../includes/minishell.h"

void handle_heredoc_line(char *line, char *delim, int fd, bool expandable)
{
    if (line[0] != '\0')
    {
        if (!ft_strncmp(delim, line, ft_strlen(delim)))
            return;
        line = expand_env(line, false, !expandable);
        ft_putendl_fd(line, fd);
        add_history(line);
        free(line);
    }
}

int init_heredoc(char *delim, char **tmp_file, int *fd)
{
    if (!delim)
        return 0;
    *tmp_file = ft_mktmp();
    *fd = open(*tmp_file, O_RDWR | O_CREAT | O_EXCL, 0640);
    if (*fd < 0)
    {
        print_err(*tmp_file, 0);
        set_exit_status(1);
        return 0;
    }
    return 1;
}
bool is_expandable(t_token *tok)
{
    t_token *ptr;

    if (!tok)
        return (1);
    ptr = tok;
    while (ptr)
    {
        if (ptr->type == DQSTR || ptr->type == STR)
            return (0);
        ptr = ptr->nospace_next;
    }
    return (1);
}
