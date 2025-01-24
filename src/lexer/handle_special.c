#include "../../includes/minishell.h"

void	token_var(t_shell *shell, t_token **token, char **input)
{
	char	*var_name;
	char	*start;

	if (*(*input + 1) != '_' && !ft_isalnum(*(*input + 1)) && *(*input + 1) != '?')  // test $* is it a variable or a wildcard ?
	{
		tokenize(shell, token, "$", WORD);
		check_concate(*token, input);
		(*input)++;
	}
	else
	{
		(*input)++; // Skip the '$'
		start = *input;
		while (**input && (ft_isalnum(**input) || **input == '_' || **input == '?'))
			(*input)++;
		var_name = ft_substr(start, 0, *input - start); // Get variable name
        if (!var_name)
		{
            shell->error = ERR_MEMORY;
			return ;
		}
        tokenize(shell, token, var_name, VARIABL);// Tokenize the variable value
		check_concate(*token, input);
	}
}

void	token_and(t_shell *shell, t_token **token, char **input)
{
	char	*value;

	if (*(*input + 1) == '&')
	{	
		value = ft_strdup("&&");
		if (value == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		tokenize(shell, token, value, T_AND);
		(*input) += 2;
		return ;
	}
	value = ft_strdup("&");
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, WORD);
	check_concate(*token, input);
	(*input)++;
}

void	token_wild(t_shell *shell, t_token **token, char **input)
{
	char	*start;
	char	*value;

	start = *input;
	while (**input && !is_delem(input)) // the dollar sign get ignored if alone f*$*s 
		(*input)++;											// or if no variable found f*$NOTHING*s 
	value = ft_substr(start, 0, *input - start);			// and get expanded if found f*$HOME*s 
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, WILD);
}

void	token_paran(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (**input == '(')
	{
		value = ft_strdup("(");
		type = O_P;
	}
	else
	{
		value = ft_strdup(")");
		type = C_P;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}
void	token_pipe(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (*(*input + 1) == '|')
	{
		value = ft_strdup("||");
		type = T_OR;
		(*input)++;
	}
	else
	{
		value = ft_strdup("|");
		type = PIPE;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}

void	out_token(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (*(*input + 1) == '>')
	{
		value = ft_strdup(">>");
		type = APPEND;
		(*input)++;
	}
	else
	{
		value = ft_strdup(">");
		type = REDIRECT_OUT;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}

void	input_token(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (*(*input + 1) == '<')
	{
		value = ft_strdup("<<");
		type = HEREDOC;
		(*input)++;
	}
	else
	{
		value = ft_strdup("<");
		type = REDIRECT_IN;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}

void	token_quote(t_shell *shell, t_token **token, char *value, int expand)
{
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	if (expand)
		tokenize(shell, token, value, WORD_VAR);
	else
		tokenize(shell, token, value, WORD);
}

void	handle_quotes(t_shell *shell, t_token **token, char **input)
{
	char	*start;
	char	*value;
	char	quote;
	int		expand;

	quote = **input;
	start = ++(*input);
	expand = 0;
	while (**input && **input != quote)
	{
		if (**input == '$' && quote == '"')
			expand = 1;
		(*input)++;
	}
	if (**input != quote)
	{
		shell->error = ERR_QUOTE;
		ft_putendl_fd(ERR_QUOTE_MSG, STD_ERR);
		return ;
	}	
	value = ft_substr(start, 0, *input - start);
	token_quote(shell, token, value, expand);
	(*input)++;
	check_concate(*token, input);
}

void	handle_special(t_shell *shell, t_token **token, char **input)
{
	if (**input == '<')
		input_token(shell, token, input);
	else if (**input == '>')
		out_token(shell, token, input);
	else if (**input == '|')
		token_pipe(shell, token, input);
	else if (**input == ')' || **input == '(')
		token_paran(shell, token, input);
	else if (**input == '$')
		token_var(shell, token, input);
	else if (**input == '&')
		token_and(shell, token, input);
	else if (**input == '*')
		token_wild(shell, token, input);
	else if (**input == '"' || **input == '\'')
		handle_quotes(shell, token, input);
}
