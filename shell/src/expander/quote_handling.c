/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:21:13 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 10:22:04 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_tilde(char *str)
{
	char	*home;
	char	*result;

	if (!str || str[0] != '~')
		return (str);
	if (str[1] != '\0' && str[1] != '/')
		return (str);
	home = getenv("HOME");
	if (!home)
		return (str);
	if (str[1] == '\0')
		result = ft_strdup(home);
	else
		result = ft_strjoin(home, str + 1);
	free(str);
	return (result);
}

void	should_split_token(t_split_args *args, bool is_first_token,
	bool is_export_val)
{
	bool	should_split;

	(void)is_first_token;
	(void)is_export_val;
	should_split = (args->sub_tok->type == WORD
			&& args->sub_tok->to_expand
			&& (ft_strchr(args->to_join, ' ')
				|| ft_strchr(args->to_join, '\t')));
	if (should_split)
	{
		if (ft_strchr(args->to_join, '\t'))
			args->split_char = '\t';
		else
			args->split_char = ' ';
		handle_split_args(args);
	}
	else if (ft_strchr(args->to_join, HIDDEN_SEPARATOR))
	{
		args->split_char = (char)HIDDEN_SEPARATOR;
		handle_split_args(args);
	}
	else
		add_str_lst(args->to_join, args->lst, args->iter != 0, args->sub_tok);
}

void	expand_nosp_arg(t_token *sub_tok, t_str **lst, bool ignore_env)
{
	char			*to_join;
	int				iter;
	bool			is_export_val;
	bool			is_first_token;
	t_split_args	args;

	iter = 0;
	is_first_token = true;
	while (sub_tok)
	{
		is_export_val = (sub_tok->value && ft_strchr(sub_tok->value, '=')
				&& !sub_tok->nospace_next && !sub_tok->next);
		to_join = expand(sub_tok, ignore_env);
		if (!to_join)
			return ;
		args.to_join = to_join;
		args.lst = lst;
		args.iter = iter;
		args.sub_tok = sub_tok;
		should_split_token(&args, is_first_token, is_export_val);
		is_first_token = false;
		sub_tok = sub_tok->nospace_next;
		iter++;
	}
}

bool	handle_chunk(t_chunk_info *info)
{
	size_t	len;

	len = get_chunk_len(info->ptr, "$");
	*(info->chunk) = ft_substr(info->ptr, 0, len);
	*(info->offset) = len;
	if (!info->ignore_env && *(info->ptr) == '$' && len > 1)
	{
		if (!expand_environment_variable(info, len))
			return (false);
	}
	return (true);
}

char	*expand(t_token *tok, bool ignore_env)
{
	char	*str;

	if (is_standalone_dollar_star(tok->value))
		return (NULL);
	if (tok->type == WORD && is_dollar_quoted_str(tok->value))
		return (handle_dollar_quoted(tok->value));
	if (tok->type == DQSTR)
		str = ft_strtrim(tok->value, "\"");
	else if (tok->type == STR)
		str = ft_strtrim(tok->value, "'");
	else
		str = ft_strdup(tok->value);
	if (!str)
		return (NULL);
	if (tok->type != DQSTR && tok->type != STR)
		str = expand_tilde(str);
	if (ft_strcmp(str, "$?") == 0)
		tok->to_expand = true;
	if (!tok->to_expand || ignore_env)
		return (str);
	return (expand_env(str, tok->type == DQSTR, ignore_env));
}
