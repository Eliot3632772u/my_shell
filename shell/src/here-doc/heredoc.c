#include "../../includes/minishell.h"

char *handle_heredoc(char *delim, bool expandable)
{
    char *line;
    char *tmp_file;
    int fd;

    if (!init_heredoc(delim, &tmp_file, &fd))
        return NULL;
    signal(SIGINT, heredoc_sigint_handler);
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            close(fd);
            return tmp_file;
        }
        handle_heredoc_line(line, delim, fd, expandable);
        if (!ft_strncmp(delim, line, ft_strlen(delim)))
        {
            free(line);
            break;
        }
    }
    close(fd);
    return tmp_file;
}

char *ft_mktmp(void)
{
    t_strbuilder *sb;
    char *name;
    int fd;
    int i;

    i = 0;
    while (++i)
    {
        sb = stringbuilder();
        sb_append(sb, "/tmp/Minishell_HEREDOC_");
        sb_append_int(sb, i);
        name = ft_strdup(sb->str);
        sb_free(sb);
        fd = open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd != -1)
        {
            unlink(name);
            close(fd);
            return (name);
        }
        free(name);
    }
    return (NULL);
}

static int process_heredoc_tree(t_ast_cmd *tree)
{
    int type;

    if (!tree)
        return 1;
    type = tree->type;
    if (type == P_AND || type == P_OR || type == P_PIPE)
    {
        if (!process_heredoc_tree(((t_ast_binary *)tree)->left))
            return 0;
        return process_heredoc_tree(((t_ast_binary *)tree)->right);
    }
    else if (tree->type == P_SUBSH)
        return process_heredoc_tree(((t_ast_subsh *)tree)->cmd);
    else if (tree->type == P_REDIR)
    {
        if (((t_ast_redir *)tree)->direction == HEREDOC)
            if (!patch_token((t_ast_redir *)tree))
                return 0;
        return process_heredoc_tree(((t_ast_redir *)tree)->cmd);
    }
    return 1;
}

int ft_heredoc(t_ast_cmd *tree)
{
    if (!process_heredoc_tree(tree))
        return 0;
    return (g_last_signal != 420);
}

int patch_token(t_ast_redir *tree)
{
    t_token *tok;
    t_str *s_ptr;

    if (!tree)
        return 1;
    s_ptr = NULL;
    tok = NULL;
    expand_nosp_arg(tree->file_tok, &s_ptr, 1);
    tok = tree->file_tok;
    free(tok->value);
    tok->value = handle_heredoc(s_ptr->str, is_expandable(tree->file_tok));
    if (!tok->value)
    {
        free(s_ptr->str);
        free(s_ptr);
        return 0;
    }
    signal(SIGINT, sigint_handler);
    tok->len = ft_strlen(tok->value);
    tree->direction = INPUT;
    free_tok_lst(tok->nospace_next);
    tok->nospace_next = NULL;
    free(s_ptr->str);
    free(s_ptr);
    tok->to_expand = false;
    return 1;
}
