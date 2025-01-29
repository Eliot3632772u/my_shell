#include "../../includes/minishell.h"

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
	return (NULL);
}