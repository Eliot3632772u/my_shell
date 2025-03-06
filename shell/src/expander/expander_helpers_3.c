/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers_3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 07:21:36 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/06 07:58:46 by yrafai           ###   ########.fr       */
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

void	should_split_token(t_split_args *args, \
	bool is_first_token, bool is_export_val)
{
	if (is_first_token && !is_export_val && args->sub_tok->to_expand
		&& ft_chr(args->to_join, ' ') && args->sub_tok->type != DQSTR
		&& args->sub_tok->type != STR)
	{
		args->split_char = ' ';
		handle_split_args(args);
	}
	else if (ft_strchr(args->to_join, TROLL))
	{
		args->split_char = (char)TROLL;
		handle_split_args(args);
	}
	else if (ft_strchr(args->to_join, ' ') && args->sub_tok->to_expand
		&& !args->sub_tok->nospace_next && !args->sub_tok->next
		&& !is_export_val && args->sub_tok->type != DQSTR
		&& args->sub_tok->type != STR)
	{
		args->split_char = ' ';
		handle_split_args(args);
	}
	else
	{
		add_str_lst(args->to_join, args->lst, args->iter != 0, args->sub_tok);
	}
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
		is_export_val = sub_tok->value && ft_strchr(sub_tok->value, '=')
			&& !sub_tok->nospace_next && !sub_tok->next;
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

char	**expand_args(t_token *tok_lst)
{
	char	**argv;
	t_str	*argv_lst;

	argv_lst = NULL;
	while (tok_lst)
	{
		expand_nosp_arg(tok_lst, &argv_lst, 0);
		tok_lst = tok_lst->next;
	}
	argv_lst = expand_wild_cards(argv_lst);
	argv = consume_argv(argv_lst);
	return (argv);
}
