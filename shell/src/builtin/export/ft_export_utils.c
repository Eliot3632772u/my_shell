/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:05:15 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 12:50:56 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_is_alphanum_underscore(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (1);
		str++;
	}
	return (0);
}

int	validate_var_name(char *str)
{
	char	*key;
	size_t	split_index;
	int		res;
	char	*concat_sign;

	if (!str || is_bad_env_name_start(str[0]))
		return (1);
	split_index = 0;
	if (is_concate(str))
	{
		concat_sign = ft_strnstr(str, "+=", ft_strlen(str));
		split_index = concat_sign - str;
	}
	else if (ft_strchr(str, '='))
		split_index = (size_t)ft_strchr(str, '=') - (size_t)str;
	if (split_index)
		key = ft_substr(str, 0, split_index);
	else
		key = ft_strdup(str);
	res = ft_is_alphanum_underscore(key + 1);
	free(key);
	return (res);
}

int	is_bad_env_name_start(char c)
{
	return (!ft_isalpha(c) && (c != '_'));
}

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
				ft_printf("declare -x %s=\"%s\"\n", ptr->key, ptr->value);
			else
				ft_printf("declare -x %s=\"\"\n", ptr->key);
		}
		else
			ft_printf("declare -x %s\n", ptr->key);
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
