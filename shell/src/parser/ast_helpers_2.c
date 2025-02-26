#include "../../includes/minishell.h"

static int count_heredocs_in_tokens(t_token *tokens)
{
    int count;
    t_token *current;

    count = 0;
    current = tokens;
    while (current && current->type != NEW_LINE)
    {
        if (current->type == HEREDOC)
            count++;
        current = current->next;
    }
    return count;
}

void free_ast(t_ast_cmd *tree)
{
    if (!tree)
        return;
    if (tree->type == P_EXEC)
        free_tok_lst(((t_ast_exec *)tree)->argv_tok);
    else if (tree->type == P_REDIR)
    {
        free_tok_lst(((t_ast_redir *)tree)->file_tok);
        free_ast(((t_ast_redir *)tree)->cmd);
    }
    else if (tree->type == P_SUBSH)
        free_ast(((t_ast_subsh *)tree)->cmd);
    else
    {
        free_ast(((t_ast_binary *)tree)->left);
        free_ast(((t_ast_binary *)tree)->right);
    }
    free(tree);
}

t_ast_cmd *redir_file(t_token **current)
{
    t_ast_cmd *node;
    t_token *start;

    node = NULL;
    if (match(*current, (t_token_type[]){OUTPUT, APPEND, INPUT, HEREDOC}, 4))
    {
        if ((*current)->type == HEREDOC && count_heredocs_in_tokens(*current) > 16)
        {
            ft_putstr_fd("bash: maximum here-document count exceeded\n", 2);
            set_exit_status(2);
            while (*current && (*current)->type != NEW_LINE)
                advance(current);
            if (*current)
                advance(current);
            return NULL;
        }
        start = *current;
        advance(current);
        if (!match(*current, (t_token_type[]){WORD, STR, DQSTR}, 3))
            return NULL;
        node = (t_ast_cmd *)tok_to_redir(start);
    }
    return node;
}

bool add_redir_node(t_ast_redir **lst, t_ast_cmd *node)
{
    t_ast_redir *tmp;

    if (!lst || !node)
        return false;
    if (!(*lst))
    {
        *lst = (t_ast_redir *)node;
        return true;
    }
    tmp = *lst;
    while (tmp && tmp->cmd)
        tmp = (t_ast_redir *)(tmp)->cmd;
    tmp->cmd = node;
    return true;
}

void free_redir(t_ast_cmd *sub_sh, t_ast_redir *redir_lst, t_token *exe_lst)
{
    free_ast(sub_sh);
    free_ast((t_ast_cmd *)redir_lst);
    free_tok_lst(exe_lst);
}
