/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-13 09:18:38 by irabhi            #+#    #+#             */
/*   Updated: 2025-01-13 09:18:38 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_env(char *env)
{
	int		i;
	char	**res;
	char	*str;
	char	*str2;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	str = ft_substr(env, 0, i);
	str2 = ft_substr(env, i + 1, ft_strlen(env));
	if (!str || !str2)
		return (NULL);
	res = malloc(sizeof(char *) * 2);
	if (!res)
	{
		free(str);
		free(str2);
	}
	res[0] = str;
	res[1] = str2;
	return (res);
}

void	init_env(t_env **env_struct, char **env)
{
	char	**env_vars;
	int		i;

	i = 0;
	while (env[i])
	{
		env_vars = split_env(env[i]);
		if (!env_vars)
			free_exit(env_struct, env_vars, 1);
		if (push_front(env_struct, env_vars[0], env_vars[1]))
			free_exit(env_struct, env_vars, 1);
		free_arr(env_vars);
		i++;
	}
}
//char	*get_prompt()
//{
//	char	*tmp;
//	char	*cwd;

//	cwd = getcwd(NULL, 0);
//	tmp = cwd;
//	cwd = ft_strjoin("\033[36m-> ", cwd);
//	free(tmp);
//	tmp = cwd;
//	cwd = ft_strjoin(cwd, " \033[0m");
//	free(tmp);
//	return (cwd);
//}

void	init_shell()
{
	char	*line;
	t_token	*token;
	//char	*prompt;

	while (1337)
	{
		//prompt = get_prompt();
		line = readline("msh --> ");
		if (!line)
		{
			// handel readline fail
		}
		if (line && line[0])
			add_history(line);
		if (lex(&token, line))
		{
			// handel lexing failure
		}
		//free(prompt);
		free(line);
	}
}

int	main(int ac, char **av, char **nv)
{
	t_env	*env;

	env = NULL;

	((void)ac, (void)av);
	init_env(&env, nv);
	init_shell();
}