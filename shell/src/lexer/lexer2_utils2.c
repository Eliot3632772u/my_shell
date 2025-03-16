/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:57:06 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/16 08:17:31 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_concate(char **command, int *concate)
{
	if (**command && !is_delem(command))
		*concate = 1;
	else
		*concate = 0;
}

void	input_tok(char **command, t_token **tokens)
{
	int		len;
	char	*value;

	len = 1;
	if (*(*command + 1) == '<')
	{
		len = 2;
		if (*(*command + 2) == '-')
			len = 3;
	}
	value = ft_substr(*command, 0, len);
	*command = (*command) + len;
	if (value == NULL)
		return ;
	if (len == 3)
		add_token(tokens, new_token(HEREDOC_TAB, value, len), true);
	else
		add_token(tokens, new_token(INPUT + (len - 1), value, len), true);
}

void	out_tok(char **command, t_token **tokens)
{
	int		len;
	char	*value;

	len = 1;
	if (*(*command + 1) == '>')
		len = 2;
	value = ft_substr(*command, 0, len);
	*command = (*command) + len;
	if (value == NULL)
		return ;
	add_token(tokens, new_token(OUTPUT + (len - 1), value, len), true);
}

void	pipe_tok(char **command, t_token **tokens)
{
	int		len;
	char	*value;

	len = 1;
	if (*(*command + 1) == '|')
		len = 2;
	value = ft_substr(*command, 0, len);
	*command = (*command) + len;
	if (value == NULL)
		return ;
	add_token(tokens, new_token(PIPE + (len - 1), value, len), true);
}

void	paren_tok(char **command, t_token **tokens)
{
	t_token_type	type;
	char			*value;

	type = LPREN;
	if (**command == ')')
		type = RPREN;
	value = ft_substr(*command, 0, 1);
	(*command)++;
	if (value == NULL)
		return ;
	add_token(tokens, new_token(type, value, 1), true);
}
