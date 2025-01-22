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

void	token_wilde(t_shell *shell, t_token **token, char **input)
{
	char	*start;
	char	quote;

	start = *input;
	while (**input && !is_delem(**input))
	{
		if (**input == '\'' || **input == '"')
		{
			quote = **input;
			(*input)++;
			while (**input && **input != quote)
				(*input)++;
			if (**input != quote)
			{
				lex_err(NULL, shell);
				break ;
			}
			(*input)++;
		}
		else
			(*input)++;
	}
	shell->type = WILD;
	insert_str(shell, token, input, start);
	(*input)++;
}

void	token_p(t_shell *shell, t_token **token, char **input)
{
	if (**input == '(')
		tokenize(shell, token, ft_strdup("("), O_P);
	else if (**input == ')')
		tokenize(shell, token, ft_strdup(")"), C_P);
}