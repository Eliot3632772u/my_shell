/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:05:28 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 12:49:22 by yrafai           ###   ########.fr       */
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

static int	handle_export_args(char *ptr, t_env **env)
{
	if (!ptr || !*ptr)
		return (2);
	if (validate_arg(ptr))
		return (1);
	if (handle_concate_case(ptr))
		return (0);
	process_export_argument(ptr, env);
	return (0);
}

int	validate_arg(char *arg)
{
	if (!arg)
		return (0);
	if (validate_var_name(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	return (0);
}

int	handle_concate_case(char *line)
{
	if (!line || !is_concate(line))
		return (0);
	concate_env(get_key(line), get_value(line));
	return (1);
}

int	ft_export(int argc, char **argv, t_env **env)
{
	int	i;
	int	res;
	int	status;

	if (!env)
		return (1);
	if (!argc)
		return (print_exports(*env));
	i = 0;
	res = 0;
	while (i < argc)
	{
		status = handle_export_args(argv[i++], env);
		if (status == 2)
			return (print_exports(*env));
		if (status == 1)
			res = 1;
	}
	return (res);
}
