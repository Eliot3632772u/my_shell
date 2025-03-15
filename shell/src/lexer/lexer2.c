/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:56:54 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/15 17:56:57 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    and_tok(char **command, t_token **tokens)
{
    t_token_type    type;
    char            *value;
    int             len;

    type = WORD;
    len = 1;
    if (*(*command + 1) == '&')
    {
        len = 2;
        type = AND;
    }
    value = ft_substr(*command, 0, len);
    *command = (*command) + len;
    if (value == NULL)
        return ;
    add_token(tokens, new_token(type, value, len), true);
}

void    handle_quotes(char **command, t_token **tokens, int *concate)
{
    char	        *start;
	char	        *value;
	char	        quote;

	quote = **command;
	start = (*command)++;
	while (**command && **command != quote)
		(*command)++;
	if (!(**command))
	{
		unclosed_error(quote);
		return ;
	}
    (*command)++;
	value = ft_substr(start, 0, *command - start);
    if (!value)
    {
        return ;
    }
	if (*concate)
        add_token(tokens, new_token(check_type(quote), value, ft_strlen(value)), false);
    else
        add_token(tokens, new_token(check_type(quote), value, ft_strlen(value)), true);
    check_concate(command, concate);
}

void    tok_str(char **command, t_token **tokens, int *concate)
{
    char    *start;
    char    *value;

    start = *command;
	while (**command && !is_delem(command) && \
	**command != '"' && **command != '\'')
		(*command)++;
	value = ft_substr(start, 0, *command - start);
	if (value == NULL)
		return ;
	if (*concate)
        add_token(tokens, new_token(WORD, value, ft_strlen(value)), false);
    else
        add_token(tokens, new_token(WORD, value, ft_strlen(value)), true);
    check_concate(command, concate);
}

void    handle_special(char **command_line, t_token **tokens, int *concate)
{
    if (**command_line == '<')
		input_tok(command_line, tokens);
	else if (**command_line == '>')
		out_tok(command_line, tokens);
	else if (**command_line == '|')
		pipe_tok(command_line, tokens);
	else if (**command_line == ')' || **command_line == '(')
		paren_tok(command_line, tokens);
	else if (**command_line == '&')
		and_tok(command_line, tokens);
	else if (**command_line == '"' || **command_line == '\'')
		handle_quotes(command_line, tokens, concate);
}

void    lexer2(char *command_line, t_token **tokens)
{
    int     concate;

    concate = 0; 
    while (*command_line)
    {
        while (*command_line && ft_isspace(*command_line))
            command_line++;
        if (special_char(*command_line))
            handle_special(&command_line, tokens, &concate);
        else if (*command_line && !special_char(*command_line))
            tok_str(&command_line, tokens, &concate);
        // need to check last status for unclosed quote error and free the token list
    }
	return ;
}