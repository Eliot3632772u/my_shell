#include "../../includes/minishell.h"

void	add_char(t_shell *shell, char **str, char *c)
{
	char	*tmp;
	if (c == NULL)
		handle_error(shell, NULL, *str, ERR_MEMORY);
	if (*str == NULL)
	{
		*str = ft_strdup(c);
		if (!(*str))
		{
			free(c);
			handle_error(shell, NULL, *str, ERR_MEMORY);
		}
		return ;
	}
	tmp = *str;
	*str = ft_strjoin(*str, c);
	if (!(*str))
		handle_error(shell, NULL, tmp, ERR_MEMORY);
	free(tmp);
	free(c);
}

char	*search_env(t_env *env, char *to_find)
{
	while (env)
	{
		if (ft_strcmp(env->key, to_find) == 0)
		{
			free(to_find);
			return (env->value);
		}
		env = env->next;
	}
	free(to_find);
	return (NULL);
}
int	join_env_var(char **str, char *env_value)
{
	char	*s;

	if (*str == NULL)
	{
		*str = ft_strdup(env_value);
		if (!(*str))
			return (1);
		return (0);
	}
	s = ft_strjoin(*str, env_value);
	if (!s)
		return (1);
	free(*str);
	*str = s;
	return (0);
}

void	expand_var(t_shell *shell, char **str, char ***input, char quote)
{
	char	*tmp;
	int		i;

	if (check_special_case(shell, input, str))
		return ;
	i = 0;
	tmp = **input;
	while (***input && (ft_isalnum(***input) || ***input == '_'))
	{
		(**input)++;
		i++;
	}
	tmp = ft_substr(tmp, 0, i);
	if (!tmp)
		handle_error(shell, NULL, *str, ERR_MEMORY);
	tmp = search_env(shell->env, tmp);
	if (tmp)
	{
		if (join_env_var(str, tmp))
			handle_error(shell, NULL, *str, ERR_MEMORY);
	}
}

void	handle_quote(t_token **token, char **input, t_shell *shell, char quote)
{
	char	*tmp;
	char	*str;

	str = NULL;
	while (**input && **input != quote)
	{
		if (**input == '$' && quote == '"')
			expand_var(shell, &str, &input, quote);
		else
		{
			tmp = ft_substr(*input, 0, 1);
			add_char(shell, &str, tmp);
			(*input)++;
		}
	}
	if (**input != quote)
	{
		lex_err(str, shell);
		return ;
	}
	if (shell->is_exit_status)
		tokenize(shell, token, str, EXIT_STATUS_VAR);
	else
		tokenize(shell, token, str, WORD);
}
