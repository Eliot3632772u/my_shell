#include "../../includes/minishell.h"

t_ast   *parse_sub(t_token **tok, t_shell *shell)
{
    t_ast   *subshell;

    subshell = NULL;
    if (!(*tok)->next || (*tok)->next->type == C_P || (*tok)->next->type == T_AND || (*tok)->next->type == T_OR || (*tok)->next->type == PIPE)
        return (NULL);
    free_tok_node(tok);
    if ((*tok) && (*tok)->type != C_P)
    {
        subshell = new_ast(ast_sub, shell);
        if (subshell == NULL)
            return (NULL);
        subshell->left = parse_logical(tok, shell);
    }
    if (!(*tok) || (*tok)->type != C_P)
    {
        shell->error = UNEXPECTED_TOKEN;
        return (NULL);
    }
    free_tok_node(tok);
    return (subshell);
}

t_ast	*parse_cmd(t_token **tok, t_shell *shell)
{
    t_token *tmp;
    t_ast   *ast;

    if ((*tok))
        printf("cmd: %s\n", (*tok)->value);
    if ((*tok) && (*tok)->type == O_P)
        return (parse_sub(tok, shell));
    if (!(*tok) || ((*tok)->type == C_P || (*tok)->type == T_AND || (*tok)->type == T_OR || (*tok)->type == PIPE))
        return (NULL);
    tmp = *tok;
    while (*tok && ((*tok)->type == WORD || (*tok)->type == WORD_VAR || (*tok)->type == VARIABL || (*tok)->type == WILD))
        (*tok) = (*tok)->next;
    ast = new_cmd(tmp, shell);
    return (ast);
}

t_ast   *parse_redi(t_token **tok, t_shell *shell)
{
    t_ast	*ast;

	ast = parse_cmd(tok, shell);
    if (!ast || shell->error)
        return (ast);
    while (*tok && ((*tok)->type == HEREDOC || (*tok)->type == APPEND || (*tok)->type == REDIRECT_IN || (*tok)->type == REDIRECT_OUT))
    {
        if (!(*tok)->next || ((*tok)->next->type != WORD && (*tok)->next->type != WORD_VAR && (*tok)->next->type != VARIABL && (*tok)->next->type != WILD))
        {
            free_tok_node(tok);
            shell->error = UNEXPECTED_TOKEN;
            return (ast);
        }
        if (add_redic(&(ast->redc), tok, shell))
            return (ast);
        if (*tok && ((*tok)->type == WORD || (*tok)->type == WORD_VAR || (*tok)->type == VARIABL || (*tok)->type == WILD))
            add_arg(&(ast->tok_args), tok); // todo , need to add args only if there is a cmd // echo hello > file | > wc hello2 --> zsh: command not found: hello2
    }
    return (ast); 
}

t_ast   *parse_pipe(t_token **tok, t_shell *shell)
{
    t_ast	*ast;
    t_ast   *root;

	ast = parse_redi(tok, shell);
	if (!ast || shell->error)
		return (ast);
    if ((*tok) && (*tok)->type == PIPE)
    {
        if (!(*tok)->next || (*tok)->next->type == PIPE || (*tok)->next->type == T_AND || (*tok)->next->type == T_OR) // this check gets repeated
        {
            free_tok_node(tok);
            shell->error = UNEXPECTED_TOKEN;
            return (ast);
        }
        free_tok_node(tok);
        root = new_ast(ast_pip, shell);
        if (!root)
            return (ast);
        root->left = ast;
        root->right = parse_logical(tok, shell);
        return (root);
    }
    return (ast);
}


t_ast   *parse_logical(t_token **tok, t_shell *shell)
{
    t_ast	    *ast;
    t_ast       *root;
    t_ast_type  type;

	ast = parse_pipe(tok, shell);
	if (!ast || shell->error)
		return (ast);
    if ((*tok) && ((*tok)->type == T_AND || (*tok)->type == T_OR))
    {   
        type = ast_and;
        if ((*tok)->type == T_OR)
            type = ast_or;
        if (!(*tok)->next || (*tok)->next->type == PIPE || (*tok)->next->type == T_AND || (*tok)->next->type == T_OR) // this check gets repeated
            return ((shell->error = UNEXPECTED_TOKEN), (ast));
        free_tok_node(tok);
        root = new_ast(type, shell);
        if (root == NULL)
            return (ast);
        root->left = ast;
        root->right = parse_logical(tok, shell);
        return (root);
    }
    return (ast);
}

