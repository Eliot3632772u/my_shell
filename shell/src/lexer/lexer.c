#include "../../includes/minishell.h"

void	get_token(char *where, t_token *tok)
{
	tok->type = token_type(where);
	tok->len = token_len(tok->type, where);
}

bool	check_token(t_token *tok, char *cmd_line,
		int index_space[2], t_token **tokens)
{
	if ((int)tok->type < 0)
		return (tok_error(cmd_line[index_space[0]]), false);
	if (tok->type == WHITE_SPACE)
		return (true);
	if (tok->len == 1 && \
		(STR <= tok->type && tok->type <= DQSTR))
		return (unclosed_error(cmd_line[index_space[0]]), false);
	tok->value = ft_substr(cmd_line, index_space[0], tok->len);
	if (!tok->value)
		return (false);
	add_token(tokens,
		new_token(tok->type, tok->value, tok->len), \
		index_space[1]);
	return (true);
}

bool	lexer(char *command_line, t_token **tokens)
{
	int		index;
	bool	space;
	t_token	tok;

	index = 0;
	*tokens = NULL;
	space = true;
	tok.type = WHITE_SPACE;
	if (!command_line)
		return (false);
	while (command_line[index])
	{
		space = tok.type == WHITE_SPACE;
		get_token(command_line + index, &tok);
		if (!check_token(&tok, command_line,
				(int [2]){index, space}, tokens))
			return (free_tok_lst(*tokens), *tokens = NULL, false);
		index += tok.len;
	}
	ft_tokadd_back(tokens, new_token(NEW_LINE, ft_strdup("newline"), 7));
	return (true);
}