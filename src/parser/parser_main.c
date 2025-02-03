/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 11:10:21 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 11:10:21 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_fail(t_ast *ast, t_token **token)
{
	free_ast(ast);
	free_tokens(*token);
	ft_putendl_fd(ERR_MEMORY_MSG, STD_ERR);
	exit(ERROR);
}

t_ast	*unexpec_tok(t_token **tok, t_ast *ast)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", STD_ERR);
	if (*tok)
		ft_putendl_fd((*tok)->value, STD_ERR);
	else
		ft_putendl_fd("\\n", STD_ERR);
	free_ast(ast);
	free_tokens(*tok);
	*tok = NULL;
	return (NULL);
}

t_ast	*check_parse_error(t_shell *shell, t_token **tok, t_ast *ast)
{
	if ((*tok) || shell->error == UNEXPECTED_TOKEN)
	{
		shell->error = 0;
		return (unexpec_tok(tok, ast));
	}
	if (shell->error == ERR_MEMORY)
		exit_fail(ast, tok);
	return (NULL);
}

t_ast	*parser(t_token **tok, t_shell *shell)
{
	t_ast	*ast;

	if (!(*tok))
		return (NULL);
	ast = parse_logical(tok, shell);
	if ((*tok) || shell->error)
		return (check_parse_error(shell, tok, ast));
	return (ast);
}
