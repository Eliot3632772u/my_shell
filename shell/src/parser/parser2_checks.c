/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:01:43 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/15 18:01:47 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_word(t_token_type type)
{
	return (type == WORD || type == STR || \
	type == DQSTR);
}

int	check_cmd(t_token_type type)
{
	return (type == RPREN || type == PIPE || check_logical(type));
}

int	check_redir(t_token_type type)
{
	return (type == HEREDOC || type == APPEND || \
	type == INPUT || type == OUTPUT || type == HEREDOC_TAB);
}

int	check_logical(t_token_type type)
{
	return (type == AND || type == OR);
}
