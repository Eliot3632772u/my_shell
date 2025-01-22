#include "../../includes/minishell.h"
//int	collect_word(t_shell *shell, char **input, char *flag)
//{
//	char	quote;
//	char	*str;

//	while (**input && !is_delem(**input))
//	{
//		if (**input == '\'' || **input == '"')
//		{
//			quote = **input;
//			if (**input == '$' && quote == '"')
//				*flag = 1;
//			(*input)++;
//			while (**input && **input != quote) // to fix, it doesn't handle &"hello""aaa" --> &"hello""aaa
//				(*input)++;
//			if (**input != quote)
//			{
//				lex_err(NULL, shell);
//				return (1);
//			}
//		}
//		else
//			(*input)++;
//	}
//	return (0);
//}

void	token_and(t_shell *shell, t_token **token, char **input, char **str)
{
	char	flag;
	char	*start;

	flag = 0;
	if (*(input + 1) == '&')
	{
		tokenize(shell, token, ft_strdup("&&"), T_AND);
		(*input) += 2;
	}
	else
	{
		add_char(shell, token, str, ft_strdup("&"));
		//start = (*input) + 1;
		//if (collect_word(shell, input, &flag))
		//	return ;
		//if (flag)
		//	shell->type = VARIABL;
		//else
		//	shell->type = WORD;
		//insert_str(shell, token, input, start);
		(*input)++;
	}
}

void	token_var(t_shell *shell, t_token **token, char **input)
{
	char	*var_name;
	char	*start;

	//if (*(*input + 1) == '?')
	//{
	//	tokenize(shell, token, ft_strdup("$?"), EXIT_STATUS);
	//	(*input) += 2;
	//}
	if (*(*input + 1) != '_' && !ft_isalnum(*(*input + 1)))
	{
		tokenize(shell, token, ft_strdup("$"), WORD);
		(*input)++;
	}
	else
	{
		//(*input)++; // Skip the '$'
		start = *input;
		while (**input && (ft_isalnum(**input) || **input == '_'))
			(*input)++;
		var_name = ft_substr(start, 0, *input - start); // Get variable name
        if (!var_name)
            handle_error(shell, *token, NULL, ERR_MEMORY);
        tokenize(shell, token, var_name, VARIABL);// Tokenize the variable value if it exists
	}
}

void	handle_special(t_shell *shell, t_token **token, char **input, char **str)
{
	if (**input == '<' || **input == '>')
		token_redirect(shell, token, input);
	else if (**input == '|')
		token_pipe(shell, token, input);
	else if (**input == '&')
		token_and(shell, token, input, str); // to fix, check function &"hello""fuckers"
	else if (**input == '$')
		token_var(shell, token, input);
	else if (**input == '*')
		token_wilde(shell, token, input);
	else if (**input == ')' || **input == '(')
		toknen_p(shell, token, input);
}
