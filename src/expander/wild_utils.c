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

int	is_special_case(char *file, char *pattern)
{
	if (ft_strncmp(pattern, ".", 1) == 0)
		return (0);
	return (ft_strncmp(file, ".", 1) == 0);
}

void	increment(char **arg, char **file)
{
	(*arg)++;
	(*file)++;
}

void	unmatched_wild(char *arg, t_shell *shell)
{
	ft_putstr_fd(ERR_WILDCARD_MSG, STD_ERR);
	ft_putendl_fd(arg, STD_ERR);
	shell->error = ERR_WILD;
}

int	init_dir(struct dirent **obj, DIR **dir, t_shell *shell)
{
	errno = 0;
	*dir = opendir(".");
	if (*dir == NULL)
	{
		shell->error = OP_DIR_F;
		return (1);
	}
	*obj = readdir(*dir);
	if (errno)
	{
		shell->error = RD_DIR_F;
		return (1);
	}
	return (0);
}
