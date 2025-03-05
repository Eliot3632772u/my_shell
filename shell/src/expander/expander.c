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

    if (!to_expand)
        return (NULL);
    sb = stringbuilder();
    if (!sb)
    {
        free(to_expand);
        return (NULL);
    }
    ptr = to_expand;
    while (ptr && *ptr)
    {
        if (!handle_chunk(sb, &chunk, ptr, in_quote, ignore_env, &offset))
        {
            ptr += offset;
            continue;
        }
        if (!chunk)
        {
            sb_free(sb);
            free(to_expand);
            return (NULL);
        }
        sb_append_free(sb, chunk);
        ptr += offset;
    }
    char *result = ft_strdup(sb->str);
    if (!result)
    {
        sb_free(sb);
        free(to_expand);
        return (NULL);
    }
    sb_free(sb);
    free(to_expand);
    return (result);
}

static bool is_dollar_quoted_str(const char *str)
{
    return (str[0] == '$' && (str[1] == '"' || str[1] == '\''));
}

static char *handle_dollar_quoted(char *str)
{
    char quote;
    char *content;
    char *expanded;

    quote = str[1];
    content = ft_substr(str, 2, ft_strlen(str) - 3);
    if (!content)
        return (NULL);
    if (quote == '"' && ft_strchr(content, '$'))
    {
        expanded = expand_env(content, true, false);
        return expanded;
    }
    return content;
}

static bool is_standalone_dollar_star(const char *str)
{
    return (str[0] == '$' && str[1] == '*' && str[2] == '\0');
}

static char *expand_tilde(char *str)
{
    char *home;
    char *result;

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

char *expand(t_token *tok, bool ignore_env)
{
    char *str;

    if (is_standalone_dollar_star(tok->value))
        return NULL;
    if (tok->type == WORD && is_dollar_quoted_str(tok->value))
        return handle_dollar_quoted(tok->value);
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

void expand_nosp_arg(t_token *sub_tok, t_str **lst, bool ignore_env)
{
    char *to_join;
    int iter;
    bool is_export_val;
    bool is_first_token;

    iter = 0;
    is_first_token = true;
    while (sub_tok)
    {
        is_export_val = sub_tok->value && ft_strchr(sub_tok->value, '=') && !sub_tok->nospace_next && !sub_tok->next;
        to_join = expand(sub_tok, ignore_env);
        if (!to_join)
            return;
        if (is_first_token && !is_export_val && sub_tok->to_expand && 
            ft_strchr(to_join, ' ') && sub_tok->type != DQSTR && sub_tok->type != STR)
        {
            handle_split_args(to_join, lst, iter, sub_tok, ' ');
        }
        else if (ft_strchr(to_join, TROLL))
            handle_split_args(to_join, lst, iter, sub_tok, (char)TROLL);
        else if (ft_strchr(to_join, ' ') && sub_tok->to_expand && 
            !sub_tok->nospace_next && !sub_tok->next && 
            !is_export_val && sub_tok->type != DQSTR && sub_tok->type != STR)
            handle_split_args(to_join, lst, iter, sub_tok, ' ');
        else
            add_str_lst(to_join, lst, iter != 0, sub_tok);
        is_first_token = false;
        sub_tok = sub_tok->nospace_next;
        iter++;
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
