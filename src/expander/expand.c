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

char	*find_env(char *key, t_shell *shell)
{
	char	*tmp;

	while (shell->env)
	{
		if (ft_strcmp(key, shell->env->key) == 0)
		{
			tmp = ft_strdup(shell->env->value);
			if (tmp == NULL)
			{
				shell->error = ERR_MEMORY;
				return (NULL);
			}
			return (tmp);
		}
		shell->env = shell->env->next;
	}
	return (NULL);
}

char	*strip_var(char **arg, char *value, t_shell *shell)
{
	char	*res;                   //   "hell $PWD ddd $?$$PWD$_$BS   "
	char	*tmp;
	char	*start;

	if (check_not_alnum(arg, value, shell))
		return (value + 1);
	start = value;
	while (*value && (ft_isalnum(*value) || *value == '_'))
		value++;
	tmp = ft_substr(start, 0, value - start);
	if (tmp == NULL)
	{
		shell->error = ERR_MEMORY;
		return (NULL);
	}
	res = find_env(tmp, shell);
	free(tmp);
	if (res == NULL)
		return (value);
	join_word(arg, res, shell);
	free(res);
	return (value);
}

void	strip_word_var(char **res, char *start, char *value, t_shell *shell)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_substr(start, 0, value - start);
	if (tmp == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tmp2 = res;
	res = ft_strjoin(res, tmp);
	if (tmp2)
		free(tmp2);
	free(tmp);
	if (res == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
}

void	join_word_var(char **arg, char *value, t_shell *shell)
{
	char	*res;					
	char	*start;
	

	res = NULL;
	start = value;
	while (*value)
	{
		if (*value == '$')
		{
			strip_word_var(&res, start, value, shell);
			if (shell->error)
				return ;
			value++;
			value = strip_var(&res, value, shell);
			if (shell->error)
				return ;
			start = value;
		}
		else
			value++;
	}
	join_word(arg, res, shell);
	free(res);
}

void	join_variable(char **arg, char *value, t_shell *shell)
{
	char	*tmp;

	if (ft_strcmp(value, "?") == 0)
	{
		tmp = ft_itoa(shell->last_status);
		if (tmp == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		join_word(arg, tmp, shell);
		free(tmp);
		return ;
	}
	tmp = find_env(value, shell);
	if (shell->error)
		return ;
	join_word(arg, tmp, shell);
	free(tmp);
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
			join_variable(&arg, tokens->value, shell);
		if(arg == NULL)
		{
			shell->error = ERR_MEMORY;
			return (NULL);
		}
		tokens = tokens->next;
	}
	return (arg);
}

char	*get_args(t_token **tokens, t_shell *shell)
{
	char	*arg;
	t_token	*tmp_token;

	tmp_token = *tokens;
	while (*tokens && (*tokens)->concate)
		*tokens = (*tokens)->next;
	*tokens = (*tokens)->next;
	if (*tokens)
	{
		(*tokens)->prev->next = NULL;
		(*tokens)->prev = NULL;
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
		args[i] = get_args(&tokens, shell);
		if (shell->error)
		{
			free_arr();
			return (NULL);
		}
		i++;
	}
}
