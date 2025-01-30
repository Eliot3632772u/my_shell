#include "../../includes/minishell.h"

int	count_args(t_token *tokens)
{
	int		i;

	i = 0;
	while (tokens)
	{
		if (!tokens->concate)
			i++;
		tokens = tokens->next;
	}
	return (i);
}

void	join_word(char **arg, char *value, t_shell *shell)
{
	char	*tmp;

	if (*arg == NULL)
	{
		*arg = ft_strdup(value);
		if (*arg == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		return ;
	}
	tmp = *arg;
	*arg = ft_strjoin(*arg, value);
	free(tmp);
	if (*arg == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
}
char	*find_var_value(char **value, t_shell *shell)
{
	char	*var;
	char	*tmp;

	tmp = *value;
	var = NULL;
	while (**value && (!ft_isalnum(**value) || **value == '_' || **value == '?'))
	{
		if (**value == "?" && (*value - 1) ==)
		{
			(*value)++;
			break;
		}
		(*value)++;
	}
		
	var = ft_substr(tmp, 0, value - tmp) ;
	if (var == NULL)
	{
		shell->error = ERR_MEMORY;
		return (NULL);
	}
	find_env(var);
}

void	join_word_var(char **arg, char *value, t_shell *shell)
{
	char	*tmp;

	if (*arg == NULL)
	{
		tmp = value;
		while (*value && *value != '$')
			value++;
		arg = ft_substr(tmp, 0, value - tmp);
		tmp = find_var_value(&value, shell);
	}
}

char	*join_tokens(t_token *tokens, t_shell *shell)
{
	char	*arg;

	arg = NULL;
	while (tokens)
	{
		if (tokens->type == WORD)
			join_word(&arg, tokens->value, shell);
		else if (tokens->type == WILD)
			join_wild(tokens, &arg, shell);
		else if (tokens->type == WORD_VAR)
			join_word_var(&arg, tokens->value, shell);
		else if (tokens->type == VARIABL)
			join_variabl(&arg, tokens->value, shell);
		if(arg == NULL)
		{
			shell->error = ERR_MEMORY;
			return (NULL);
		}
		tokens = tokens->next;
	}
	return (arg);
}

char	*get_args(t_token *tokens, t_shell *shell)
{
	char	*arg;
	t_token	*tmp_token;

	tmp_token = tokens;
	while (tokens && tokens->concate)
		tokens = tokens->next;
	tokens = tokens->next;
	if (tokens)
	{
		tokens->prev->next = NULL;
		tokens->prev = NULL;
	}
	arg = join_tokens(tmp_token, shell);
	if (arg == NULL)
		return (NULL);
	return (arg);
}

char	**expand(t_token *tokens, t_shell *shell)
{
	char	**args;
	int		i;

	i = count_args(tokens);
	args = malloc((i + 1) * sizeof(char *));
	if (!args)
	{
		shell->error = ERR_MEMORY;
		return (NULL);
	}
	args[i] = NULL;
	i = 0;
	while (args[i])
	{
		args[i] = get_args(tokens, shell);
		i++;
	}
}
