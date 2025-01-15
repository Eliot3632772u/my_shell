/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-13 09:39:47 by irabhi            #+#    #+#             */
/*   Updated: 2025-01-13 09:39:47 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_arr(char **env_vars)
{
	int		i;

	i = 0;
	while (env_vars && i < 2)
	{
		free(env_vars[i]);
		i++;
	}
	if (env_vars)
		free(env_vars);
}

void	free_exit(t_env **env, char **env_vars, int status)
{
	t_env	*tmp;

	while (*env)
	{
		tmp = (*env)->next;
		free(*env);
		*env = tmp;
	}
	free_arr(env_vars);
	if (status)
		ft_putstr_fd("malloc failed\n", 2);
	exit(status);
}