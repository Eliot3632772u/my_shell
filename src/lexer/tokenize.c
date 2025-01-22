# include "../../includes/minishell.h"

t_token	*new_token(char *val, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->prev = NULL;
	// todo initialize type_struct 
	new->value = val;
	new->type = type;
	return (new);
}

int	check_buffer(t_shell *shell, t_token **token, char *valu)
{
	char	*tmp;

	if (shell->buffer == NULL);
		return (0);
	tmp = shell->buffer;
	shell->buffer = ft_strjoin(shell->buffer, valu);
	free(tmp);
	free(valu);
	if (shell->buffer == NULL)
		handle_error(shell, *token, valu, ERR_MEMORY);
	return (1);
}

void	tokenize(t_shell *shell, t_token **token, char *valu, t_token_type type)
{
	t_token	*tmp;

	if (!valu)
	{
		valu = ft_strdup("");
		if (!valu)
			handle_error(shell, *token, NULL, ERR_MEMORY);  
	}
	if (check_buffer(shell, token, valu))
		return ;
	if (*token == NULL)
	{
		*token = new_token(valu, type);
		if (!(*token) == NULL)
			handle_error(shell, *token, valu, ERR_MEMORY);
		return ;
	}
	tmp = *token;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token(valu, type);
	if (!tmp->next)
		handle_error(shell, *token, valu, ERR_MEMORY);
	tmp->next->next = NULL;
	tmp->next->prev = tmp;
	// set type_struct;
}
