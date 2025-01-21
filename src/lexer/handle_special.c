#include "../../includes/minishell.h"

void	token_redirect(t_shell *shell, t_token **token, char **input)
{
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			tokenize(shell, token, ft_strdup(">>"), APPEND);
			(*input)++;
		}
		else
			tokenize(shell, token, ft_strdup(">"), REDIRECT_OUT);
		(*input)++;
	}
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			tokenize(shell, token, ft_strdup("<<"), HEREDOC);
			(*input)++;
		}
		else
			tokenize(shell, token, ft_strdup("<"), REDIRECT_IN);
		(*input)++;
	}
}
void	token_pipe(t_shell *shell, t_token **token, char **input)
{
	if (*(input + 1) == '|')
	{
		tokenize(shell, token, ft_strdup("||"), T_OR);
		(*input)++;
	}
	else
		tokenize(shell, token, ft_strdup("|"), PIPE);
	(*input)++;
}

void	token_and(t_shell *shell, t_token *token, char **input)
{
	if (*(input + 1) == '&')
	{
		tokenize(shell, token, ft_strdup("&&"), T_AND);
		(*input)++;
	}
	else
		tokenize(shell, token, ft_strdup("&"), WORD);
	(*input)++;
}

char	*find_var(t_shell *shell, char *to_find)
{
	t_env	*env;

	env = shell->env;
	while (env)
	{
		if (ft_strcmp(env->key, to_find) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	token_var(t_shell *shell, t_token **token, char **input)
{
	char	*var_name;
	char	*var_value;
	char	*start;


	if (*(*input + 1) == '?')
	{
		tokenize(shell, token, ft_strdup("$?"), EXIT_STATUS_VAR);
		(*input) += 2;
	}
	else
	{
		(*input)++; // Skip the '$'
        start = *input;
        while (**input && (ft_isalnum(**input) || **input == '_')) // Valid env variable characters
            (*input)++;
		var_name = ft_substr(start, 0, *input - start); // Get variable name
        if (!var_name)
            handle_error(shell, *token, NULL, ERR_MEMORY);
        var_value = find_var(shell->env, var_name);// Search for the variable in the environment
        free(var_name); // Free the variable name after search
        if (var_value)
            tokenize(shell, token, ft_strdup(var_value), WORD);// Tokenize the variable value if it exists
	}
}

void	handle_special(t_shell *shell, t_token **token, char **input)
{
	if (**input == '<' || **input == '>')
		token_redirect(shell, token, input);
	else if (**input == '|')
		token_pipe(shell, token, input);
	else if (**input == '&')
		token_and(shell, token, input);
	else if (**input == '$')
		token_var(shell, token, input);
	else if (**input == '*') // todo
		token_wilde();
}
