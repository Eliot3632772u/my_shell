/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-15 12:27:24 by irabhi            #+#    #+#             */
/*   Updated: 2025-01-15 12:27:24 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_char(char **str, char c)
{
	char	*tmp;
	char	*tmp2;

	if (*str == NULL)
	{
		*str = malloc(sizeof(char) * 2);
		if (!(*str))
			return (1);
		*str[0] = c;
		*str[1] = 0;
		return (0); 
	}
	tmp = malloc(2);
	if (!tmp)
		return (1);
	tmp[0] = c;
	tmp[1] = '\0';
	tmp2 = ft_strjoin(*str, tmp);
	if (!tmp2)
		return (free(tmp), 1);
	free(*str);
	free(tmp);
	*str = tmp2;
	return (0);
}

int	join_env_var(char **str, char *env_value)
{
	char	*s;

	if (*str == NULL)
	{
		*str = ft_strdup(env_value);
		if (!(*str))
			return (1);
		return (0);
	}
	s = ft_strjoin(*str, env_value);
	if (!s)
		return (1);
	free(*str);
	*str = s;
}

char	*search_env(t_env *env, char *to_find)
{
	while (env)
	{
		if (ft_strcmp(env->name, to_find) == 0)
		{
			free(to_find);
			return (env->value);
		}
		env = env->next;
	}
	free(to_find);
	return (NULL);
}

int	expand_var(char **str, char *line, int *itt, t_env *env)
{
	char	*tmp;
	int		i;

	i = *itt + 1;
	while (line[i] && !is_spr(line[i]))
		i++;
	tmp = ft_substr(line, *itt + 1, i - (*itt + 1));
	*itt = i;
	if (!tmp)
		return (1);
	
	tmp = search_env(env, tmp);
	if (tmp)
	{
		if (join_env_var(str, env))
			return (1);
		return (0);
	}
	return (add_char(str, "$"));	
}

int	handel_quote(t_token **token, char *line, int *i, int *error, t_env *env)
{
	int		close_q;
	char	quote;
	int		itt;
	char	*str;

	close_q = 1;
	quote = line[*i];
	itt = *i + 1;
	str = NULL;
	while (line[itt] && line[itt] != quote)
	{
		if (line[itt] == '$' && quote == '"')
			expand_var(&str, line, &itt, env);
		else
		{
			if(add_char(&str, line[itt]))
				*error = 1;
		}	
	}
}