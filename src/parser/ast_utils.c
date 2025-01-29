#include "../../includes/minishell.h"

t_ast   *new_cmd(t_token *token, t_shell *shell)
{
    t_ast   *tmp;
    t_token *list;

    tmp = malloc(sizeof(t_ast));
    if (!tmp)
    {
        shell->error = ERR_MEMORY;
        return (NULL);
    }    
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->redc = NULL;
    tmp->args = NULL;
    tmp->type = ast_cmd;
	tmp->tok_args = NULL;
    if (!token || (token->type != WORD && token->type != WORD_VAR && token->type != VARIABL && token->type != WILD))
		return tmp;	
	tmp->tok_args = token; // handle if theres no command like echo hello > file | > file2 , after pipe it will create a cmd but with no args 
    list = token;          // or cmd , than it will segvault DOWN
    while (list->next && (list->next->type == WORD || list->next->type == WORD_VAR || list->next->type == VARIABL || list->next->type == WILD))
        list = list->next;
    if (list->next)
        list->next->prev = NULL; // here
    list->next = NULL; 
	return (tmp);
}

t_redirect  *new_redic(t_token_type type, t_shell *shell)
{
    t_redirect  *new;

    new = malloc(sizeof(t_redirect));
    if (!new)
    {
        shell->error = ERR_MEMORY;
        return (NULL);
    }
    new->type = type;
    new->file = NULL;
    new->tok_file = NULL;
    new->next = NULL;
    new->prev = NULL;
    return (new);
}

t_ast   *new_ast(t_ast_type type, t_shell *shell)
{
    t_ast   *new;

    new = malloc(sizeof(t_ast));
    if (!new)
    {
        shell->error = ERR_MEMORY;
        return (NULL);
    }
    new->args = NULL;
    new->type = type;
    new->redc = NULL;
    new->tok_args = NULL;
    new->left = NULL;
    new->right = NULL;
    return (new);
}