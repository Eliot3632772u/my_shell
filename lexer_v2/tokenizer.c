#include "../includes/minishell.h"

t_token	*new_token(char *val, t_token_type type)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->expand = 0;
	new->concate = 0;
	new->next = NULL;
	new->prev = NULL;
	new->type = type;
	new->value = val;
}

void	tokenize(t_shell *sh, t_token **token, char *val, t_token_type type)
{
	t_token	*tmp;

	if (*token == NULL)
	{
		*token = new_token(val, type);
		if (!(*token))
		{
			sh->error = ERR_MEMORY;
			return ;
		}
		return ;
	}
	tmp = *token;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token(val, type);
	if (!tmp->next)
	{
		sh->error = ERR_MEMORY;
		return ;
	}
	tmp->next->prev = tmp;
}
