/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:05:28 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:05:29 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	process_export_argument(char *ptr, t_env **env)
{
	char	*key;
	t_env	*env_var;

	key = get_key(ptr);
	if (ft_strchr(ptr, '='))
		set_env_value(key, get_value(ptr), 1);
	else
	{
		env_var = search_in_env(*env, key);
		if (env_var)
			set_env_value(key, ft_strdup(env_var->value), env_var->equal_sign);
		else
			set_env_value(key, ft_strdup(""), 0);
	}
}

int	ft_export(int argc, char **argv, t_env **env)
{
	int		i;
	int		res;
	char	*ptr;

	if (!env)
		return (1);
	if (!argc)
		return (print_exports(*env));
	i = 0;
	res = 0;
	while (i < argc)
	{
		ptr = argv[i++];
		if (validate_arg(ptr))
		{
			res = 1;
			continue ;
		}
		if (handle_concate_case(ptr))
			continue ;
		process_export_argument(ptr, env);
	}
	return (res);
}

int	handle_concate_case(char *line)
{
	if (!line || !is_concate(line))
		return (0);
	concate_env(get_key(line), get_value(line));
	return (1);
}
