/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_front.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-13 09:28:09 by irabhi            #+#    #+#             */
/*   Updated: 2025-01-13 09:28:09 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	push_front(t_env **env, char *name, char *value)
{
	t_env	*tmp;

	if (*env == NULL)
	{
		*env = malloc(sizeof(t_env));
		if (*env == NULL)
			return (1);
		(*env)->name = ft_strdup(name);
		(*env)->value = ft_strdup(value);
		(*env)->next = NULL;
		return (0);
	}
	tmp = malloc(sizeof(t_env));
	if (tmp == NULL)
		return (1);
	tmp->name = ft_strdup(name);
	tmp->value = ft_strdup(value);
	tmp->next = *env;
	*env = tmp;
	return (0);
}