#include "../../includes/minishell.h"

char	*get_heredoc_delem(t_token *token, t_shell *shell)
{
	char	*arg;

	arg = NULL;
	while (token)
	{
		join_word(&arg, token->value, shell);
		if (shell->error)
		{
			if (arg)
				free(arg);
			return (NULL);
		}
		token = token->next;
	}
	return (arg);
}

int	count_tokens(t_token *tokens)
{
	int		i;

	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

char	*expand_redec(t_token *token, t_shell *shell)
{
	char	**arg;
	char	*tmp;

	arg = NULL;
	if (token->type == HEREDOC)
	{
		tmp = get_heredoc_delem(token, shell);
		if (shell->error)
			return (NULL);
		return (tmp);
	}
	tmp = join_tokens(token, shell, count_tokens(token));
	insert_arg(&arg, tmp, shell);
	if (shell->error)
	{
		free(arg);
		return (NULL);
	}
	return (arg);
}
