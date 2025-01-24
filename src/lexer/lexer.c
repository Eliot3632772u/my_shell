#include "../../includes/minishell.h"

//void	token_word(t_shell *shell, t_token **token, char *value, char **input)
//{
//	if (value == NULL)
//	{
//		shell->error = ERR_MEMORY;
//		return ;
//	}
//	tokenize(shell, token, value, WORD);
//	check_concate(*token, input);
//}

void	lexer(t_shell *shell, t_token **token, char *input)
{
	char	*start;
	char	*value;

	while (*input)
	{
		while (*input && ft_isspace(*input))
			input++;
		if (is_special(*input))
			handle_special(shell, token, &input);
		else
		{
			start = input;
			while (*input && !is_delem(&input) && *input != '*' && *input != '$' && *input != '"' && *input != '\'')
				input++;
			value = ft_substr(start, 0, input - start);
			if (value == NULL)
				shell->error = ERR_MEMORY;
			tokenize(shell, token, value, WORD);
			check_concate(*token, &input);
		}
		if (shell->error)
			return ;
	}
}
