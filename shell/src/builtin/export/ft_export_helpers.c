/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:05:23 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:05:24 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	print_exports(t_env *env)
{
	t_env	*ptr;

	if (!env)
		return (1);
	ptr = env;
	while (ptr)
	{
		if (ptr->equal_sign)
		{
			if (ptr->value)
				printf("declare -x %s=\"%s\"\n", ptr->key, ptr->value);
			else
				printf("declare -x %s=\"\"\n", ptr->key);
		}
		else
			printf("declare -x %s\n", ptr->key);
		ptr = ptr->next;
	}
	return (0);
}

int	is_concate(char *str)
{
	char	*concat_sign;
	char	*equal_sign;

	if (!str)
		return (false);
	equal_sign = ft_strchr(str, '=');
	concat_sign = ft_strnstr(str, "+=", ft_strlen(str));
	if (concat_sign && (size_t)concat_sign < (size_t)equal_sign)
		return (true);
	return (false);
}
