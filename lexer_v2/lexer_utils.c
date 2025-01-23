#include "../includes/minishell.h"

int	is_delem(char **input)
{
	if (ft_isspace(**input))
		return (1);
	if (**input == '|' || **input == '<' ||
		**input == '>' || **input == ')' || **input == '(')
		return (1);
	if (**input == '&' && *(*input + 1) == '&')
		return (1);
	return (0);
}

void	check_concate(t_token *token, char **input)
{
	while (token->next)
		token = token->next;
	if (**input && !is_delem(input))
		token->concate = 1;
}
/*
	will get concatenated if :  only WORD, VARIABALE, WILD type tokens get concatenated
								only call this function after making tokens with types like above
								$"hello"  "hi""by" "hi"'by' 'by'"hi" &hello &"hoe" "hoe"& 'hi'$ 'hi'&   "hello"&$"hi"  'hoe'$&"lol"
*/