#include "../../includes/minishell.h"

t_ast   *parse_sub(t_token **tok)
{
    t_ast   *subshell;

    subshell = NULL;
    (*tok) = (*tok)->next;
    if (!(*tok) || (*tok)->type == C_P || (*tok)->type == T_AND || (*tok)->type == T_OR || (*tok)->type == PIPE)
        return (unexpec_tok(tok, subshell));
    if ((*tok) && (*tok)->type != C_P)
    {
        subshell = new_ast(ast_sub);
        if (subshell == NULL)
        {
            // handle malloc failure;
            return (NULL);
        }
        subshell->left = parse_logical(tok);
    }
    if (!(*tok) || (*tok)->type != C_P)
        return (unexpec_tok(tok, subshell));
    return (subshell);
}

t_ast	*parse_cmd(t_token **tok)
{
    t_token *tmp;
    t_ast   *ast;

    if ((*tok) && (*tok)->type == O_P)
        return (parse_sub(tok));
    if (!(*tok) || ((*tok)->type != WORD && (*tok)->type != WORD_VAR && (*tok)->type != VARIABL && (*tok)->type != WILD))
        return (NULL);
    tmp = *tok;
    while (*tok && ((*tok)->type == WORD || (*tok)->type == WORD_VAR || (*tok)->type == VARIABL || (*tok)->type == WILD))
        (*tok) = (*tok)->next;
    ast = new_cmd(tmp);
    return (ast);
}

t_ast   *parse_redi(t_token **tok)
{
    t_ast	*ast;

	ast = parse_cmd(tok);
    if (!ast)
        return (ast);
    while (*tok && ((*tok)->type == HEREDOC || (*tok)->type == APPEND || (*tok)->type == REDIRECT_IN || (*tok)->type == REDIRECT_OUT))
    {
        (*tok) = (*tok)->next;
        if (!(*tok) || ((*tok)->type != WORD && (*tok)->type != WORD_VAR && (*tok)->type != VARIABL && (*tok)->type != WILD))
            return (unexpec_tok(tok, ast));
        if (add_redic(&(ast->redc), tok))
        {
            // handle if error coming from memory fail or unexpected token
            return (NULL);
        }
        if (*tok && ((*tok)->type == WORD || (*tok)->type == WORD_VAR || (*tok)->type == VARIABL || (*tok)->type == WILD))
            add_arg(&(ast->tok_args), tok); // todo , need to add args only if there is a cmd // echo hello > file | > wc hello2 --> zsh: command not found: hello2
    }
    return (ast); 
}

t_ast   *parse_pipe(t_token **tok)
{
    t_ast	*ast;
    t_ast   *root;

	ast = parse_redi(tok);
	if (!ast)
		return (NULL);
    if ((*tok)->type == PIPE)
    {
        (*tok) = (*tok)->next;
        if (!(*tok) || (*tok)->type == PIPE || (*tok)->type == T_AND || (*tok)->type == T_OR) // this check gets repeated
            return (unexpec_tok(tok, ast));
        root = new_ast(ast_pip);
        if (!root)
        {
            //handle malloc fail
            return (NULL);
        }
        root->left = ast;
        root->right = parse_logical(tok);
        return (root);
    }
    return (ast);
}


t_ast   *parse_logical(t_token **tok)
{
    t_ast	*ast;
    t_ast   *root;
    t_ast_type  *type;

	ast = parse_pipe(tok);
	if (!ast)
		return (NULL);
    if ((*tok) && ((*tok)->type == T_AND || (*tok)->type == T_OR))
    {   
        type = ast_and;
        if ((*tok)->type == T_OR)
            type = ast_or;
        (*tok) = (*tok)->next;
        if (!(*tok) || (*tok)->type == PIPE || (*tok)->type == T_AND || (*tok)->type == T_OR) // this check gets repeated
            return (unexpec_tok(tok, ast));
        root = new_ast(type);
        if (root == NULL)
        {
            // handle failure;
            return (NULL);
        }
        root->left = ast;
        root->right = parse_pipe(tok);
        return (root);
    }
    return (ast);
}

t_ast    *unexpec_tok(t_token **tok, t_ast *ast)
{
    ft_putstr_fd("minishell: syntax error near unexpected token ", STD_ERR);
    if (*tok)
        ft_putendl_fd((*tok)->value, STD_ERR);
    else
        ft_putendl_fd("\n", STD_ERR);
    free_ast(ast);
    return (NULL);
}

void    free_redi(t_redirect *red)
{
    t_redirect  *tmp;

    while (red)
    {
        tmp = red->next;
        free_tokens(red->tok_file);
        free(red);
        red = tmp;
    }
}

t_ast    *free_ast(t_ast *ast)
{
    if(!ast)
        return (NULL);
    free_ast(ast->left);
    free_ast(ast->right);
    free_redi(ast->redc);
    free_tokens(ast->tok_args);
    free(ast);
}
