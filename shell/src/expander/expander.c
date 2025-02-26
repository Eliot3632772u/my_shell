#include "../../includes/minishell.h"

char *ft_chr(char *str, char c)
{
    return (ft_strchr(str, c));
}

char *expand_env(char *to_expand, bool in_quote, bool ignore_env)
{
    char *ptr;
    char *chunk;
    t_strbuilder *sb;
    size_t offset;

    sb = stringbuilder();
    ptr = to_expand;
    while (*ptr)
    {
        if (!handle_chunk(sb, &chunk, ptr, in_quote, ignore_env, &offset))
        {
            ptr += offset;
            continue;
        }
        (sb_append_free(sb, chunk), ptr += offset);
    }
    return (free(to_expand), to_expand = sb->str, free(sb), to_expand);
}

char *expand(t_token *tok, bool ignore_env)
{
    char *str;

    if (tok->type == DQSTR)
        str = ft_strtrim(tok->value, "\"");
    else if (tok->type == STR)
        str = ft_strtrim(tok->value, "'");
    else
        str = ft_strdup(tok->value);
    if (!tok->to_expand)
        return (str);
    if (!ignore_env && str)
        str = expand_env(str, tok->type == DQSTR, ignore_env);
    return (str);
}

void expand_nosp_arg(t_token *sub_tok, t_str **lst, bool ignore_env)
{
    char *to_join;
    int iter;

    iter = 0;
    while (sub_tok)
    {
        to_join = expand(sub_tok, ignore_env);
        if (ft_strchr(to_join, TROLL))
            handle_split_args(to_join, lst, iter, sub_tok, TROLL);
        else if (ft_strchr(to_join, ' ') && sub_tok->to_expand 
            && !sub_tok->nospace_next && !sub_tok->next)
            handle_split_args(to_join, lst, iter, sub_tok, ' ');
        else
            add_str_lst(to_join, lst, iter != 0, sub_tok);
        sub_tok = sub_tok->nospace_next;
        (free(to_join), iter++);
    }
}

char **expand_args(t_token *tok_lst)
{
    char **argv;
    t_str *argv_lst;

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
