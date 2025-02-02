#include "../../includes/minishell.h"

void	insert_arg(char ***args, char *arg, t_shell *shell)
{
	if (shell->error)
		return ;
	if (shell->is_wild)
	{
		expand_wild(args, arg, shell);
		return ;
	}	
	else if (*args == NULL)
	{
		*args = malloc(2 * sizeof(char *));
		if (*args == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		**args = arg;
		*(*args + 1) = NULL;
		return ;
	}
	realloc_arr(args, arg, shell);
}

char	*get_args(t_token **tokens, t_shell *shell)
{
	char	*arg;
	t_token	*tmp_token;
	int		i;

	tmp_token = *tokens;
	i = 1;
	while (*tokens && (*tokens)->concate)
	{
		*tokens = (*tokens)->next;
		i++;
	}
	*tokens = (*tokens)->next;
	arg = join_tokens(tmp_token, shell, i);
	return (arg);
}

char	*join_tokens(t_token *tokens, t_shell *shell, int iter)
{
	char	*arg;
	int		i;

	arg = NULL;
	i = -1;
	while (++i < iter)
	{
		if (tokens->type == WORD)
			join_word(&arg, tokens->value, shell);
		else if (tokens->type == WILD)
			join_wild(&arg, tokens->value, shell);
		else if (tokens->type == WORD_VAR)
			join_word_var(&arg, tokens->value, shell);
		else if (tokens->type == VARIABL)
			join_variable(&arg, tokens->value, shell);
		if(shell->error)
			return (NULL);
		tokens = tokens->next;
	}
	return (arg);
}

char	*find_env(char *key, t_shell *shell)
{
	char	*tmp;
	t_env	*tmp_env;

	tmp_env = shell->env;
	while (tmp_env)
	{
		if (ft_strcmp(key, tmp_env->key) == 0)
		{
			tmp = ft_strdup(tmp_env->value);
			if (tmp == NULL)
			{
				shell->error = ERR_MEMORY;
				return (NULL);
			}
			return (tmp);
		}
		tmp_env = tmp_env->next;
	}
	return (NULL);
}


int	check_not_alnum(char **arg, char *value, t_shell *shell)
{
	char	*tmp;

	if (*value != '_' && !ft_isalnum(*value))
	{
		if (*value == '?')
			tmp = ft_itoa(shell->last_status);
		else
			tmp = ft_strdup("$");
		if (tmp == NULL)
		{
			shell->error = ERR_MEMORY;
			return (ERROR);
		}
		join_word(arg, tmp, shell);
		free(tmp);
		return (1);
	}
	return (0);
}
