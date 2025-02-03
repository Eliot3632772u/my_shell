/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:37:12 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:37:12 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_special_case(char *file)
{
	return (ft_strcmp(file, ".") == 0 || ft_strcmp(file, "..") == 0);
}

void	increment(char **arg, char **file)
{
	(*arg)++;
	(*file)++;
}

void	checkpoint(char **star, char **file_pos, char **arg, char *file)
{
	*star = *arg;
	*file_pos = file;
	(*arg)++;
}

void	unmatched_wild(char *arg, t_shell *shell)
{
	ft_putstr_fd(ERR_WILDCARD_MSG, STD_ERR);
	ft_putendl_fd(arg, STD_ERR);
	shell->error = ERR_WILD;
}

//void	check_pattern(char **arg)
//{
//	int		i;

//	i = 0;
//	while ((*arg)[i])
//	{
//		if 
//	}
//}
