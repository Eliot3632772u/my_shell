/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 10:04:37 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 10:04:37 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_word(t_token_type type)
{
	return (type == WORD || type == WORD_VAR || \
	type == VARIABL || type == WILD);
}

int	check_cmd(t_token_type type)
{
	return (type == C_P || type == PIPE || check_logical(type));
}

int	check_redir(t_token_type type)
{
	return (type == HEREDOC || type == APPEND || \
	type == REDIRECT_IN || type == REDIRECT_OUT);
}

int	check_logical(t_token_type type)
{
	return (type == T_AND || type == T_OR);
}
