#include "../../includes/minishell.h"

bool valid_env_char(char c)
{
    return (ft_isalpha(c) || ft_isdigit(c) || ft_strchr("?_", c));
}

bool check_expanding(char *str, t_token_type type)
{
    char *wild_pos;
    char *dollar_pos;

    wild_pos = NULL;
    if (type != DQSTR)
        wild_pos = ft_strchr(str, '*');
    dollar_pos = ft_strchr(str, '$');
    if (!wild_pos && !dollar_pos)
        return (false);
    if (dollar_pos && valid_env_char(dollar_pos[1]))
        return (true);
    if (wild_pos)
        return (true);
    return (false);
}

void free_tok_lst(t_token *tok)
{
    t_token *tmp;

    if (!tok)
        return;
    while (tok)
    {
        free(tok->value);
        free_tok_lst(tok->nospace_next);
        tmp = tok;
        tok = tok->next;
        free(tmp);
    }
}
