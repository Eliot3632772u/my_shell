#include "../../includes/minishell.h"

void    free_tok_node(t_token **tok)
{
    t_token *tmp;

    tmp = (*tok);
    (*tok) = (*tok)->next;
    if (*tok)
        (*tok)->prev = NULL;
    free(tmp);
}

int    add_redic(t_redirect **redic, t_token **tok, t_shell *shell)
{
    t_redirect  *tmp;

    if (!(*redic))
    {
        *redic = new_redic((*tok)->type, shell);
        if (!(*redic))
            return (1);
        free_tok_node(tok);
        (*redic)->tok_file = get_file_tokens(tok);
        return (0);
    }
    tmp = *redic;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_redic((*tok)->type, shell);
    if (!tmp->next)
        return (1);
    free_tok_node(tok);
    tmp->next->tok_file = get_file_tokens(tok);
    return (0);
}

void    add_arg(t_token **tok_args, t_token **tok)
{
    t_token *tmp;

    if (*tok_args == NULL)
        *tok_args = *tok;
    else 
    {
        tmp = *tok_args;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = *tok;
    }

    while ((*tok) && ((*tok)->type == WORD || (*tok)->type == WORD_VAR || (*tok)->type == VARIABL || (*tok)->type == WILD))
            (*tok) = (*tok)->next;
    if (*tok)
    {
        (*tok)->prev->next = NULL;
        (*tok)->prev = NULL;
    }
}

t_token *get_file_tokens(t_token **tok)
{
    t_token *tmp;

    tmp = (*tok);
    while ((*tok) && (*tok)->concate)
        (*tok) = (*tok)->next;
	(*tok) = (*tok)->next;
	if ((*tok))
	{
    	(*tok)->prev->next = NULL;
    	(*tok)->prev = NULL;
	}
    return (tmp);
}