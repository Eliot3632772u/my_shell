#include "../../includes/minishell.h"

int g_signal_received = 0;

static void init_shell(t_shell *shell, char **envp)
{
    shell->env = init_env(envp);
    shell->cmd = NULL;
    shell->input = NULL;
    shell->last_status = 0;
    shell->running = 1;
    shell->is_wild = 0;
    shell->env_array = NULL;
    shell->error = 0;
}

char    *f(t_token_type type)
{
    if (type == WORD)
        return (ft_strdup("WORD"));
    else if (type == WORD_VAR)
        return (ft_strdup("WORD_VAR"));
    else if (type == REDIRECT_IN)
        return (ft_strdup("REDIRECT_IN"));
    else if (type == REDIRECT_OUT)
        return (ft_strdup("REDIRECT_OUT"));
    else if (type == APPEND)
        return (ft_strdup("APPEND"));
    else if (type == HEREDOC)
        return (ft_strdup("HERDOC"));
    else if (type == T_AND)
        return (ft_strdup("TOKEN_AND"));
    else if (type == T_OR)
        return (ft_strdup("TOKEN_OR"));
    else if (type == WILD)
        return (ft_strdup("WILDCARD"));
    if (type == VARIABL)
        return (ft_strdup("VARIABLE"));
    if (type == O_P)
        return (ft_strdup("OPEN BRACKET"));
    if (type == C_P)
        return (ft_strdup("CLOSE BRACKET"));
    if (type == PIPE)
        return (ft_strdup("PIPE"));
    return (NULL);
}


void print_tokens(t_token *token)
{
    while (token)
    {
        printf("%s ", token->value);
        token = token->next;
    }
}

void print_redirects(t_redirect *red)
{
    while (red)
    {
        printf("Redirect type: %d, File: %s\n", red->type, red->tok_file ? red->tok_file->value : "(null)");
        red = red->next;
    }
}

void print_ast(t_ast *node, int depth)
{
    if (!node)
        return;
    
    for (int i = 0; i < depth; i++)
        printf("  ");
    
    printf("Node Type: %d\n", node->type);
    
    if (node->tok_args)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("Arguments: ");
        print_tokens(node->tok_args);
        printf("\n");
    }
    
    if (node->redc)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("Redirections:\n");
        print_redirects(node->redc);
    }
    
    if (node->left)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("Left:\n");
        print_ast(node->left, depth + 1);
    }
    
    if (node->right)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        printf("Right:\n");
        print_ast(node->right, depth + 1);
    }
}

int get_ast_depth(t_ast *node)
{
    if (!node)
        return 0;
    
    int left_depth = get_ast_depth(node->left);
    int right_depth = get_ast_depth(node->right);
    
    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

void    set_varss(t_ast *ast, t_shell *shell)
{
    if (ast == NULL)
        return ;

    set_varss(ast->left, shell);
    set_varss(ast->right, shell);
    ast->args = expand(ast->tok_args, shell);
    t_redirect *tmp;
    if (ast->redc)
    {
        tmp = ast->redc;
        while (tmp)
        {
            tmp->file = expand_redec(tmp->tok_file, shell);
            tmp = tmp->next;
        }
    }
}

void    print_vars(t_ast *ast)
{
    if (ast == NULL)
        return ;

    print_vars(ast->left);
    print_vars(ast->right);
    int i = 0;
    while (ast->args && ast->args[i])
    {
        printf("arg %d: %s ", i, ast->args[i]);
        i++;
    }
    printf("\n\n");
    while (ast->redc)
    {
        i = 0;
        while (ast->redc->file && ast->redc->file[i])
        {
            printf("redc %d:  %s  ", i, ast->redc->file[i]);
            i++;
        }
        printf("     ----    ");
        ast->redc = ast->redc->next;
    }
    printf("\n\n\n\n");
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    t_token *token;
    char *input;
    (void)argc;
    (void)argv;

    // Initialize shell
    init_shell(&shell, envp);
    //setup_signals();

    token = NULL;
    // Main shell loop
    while (shell.running)
    {
        input = readline("minishell$ ");
        if (!input)  // Handle Ctrl+D (EOF)
        {
            ft_printf("exit\n");
            break;
        }
        if (input[0] != '\0')
        {
            add_history(input);
            // Here the parser will be integrated
            // i'll implement basic command handling
            shell.input = input;
            lexer(&shell, &token, input);
            if (shell.error)
            {
                free_tokens(token);
                token = NULL;
                free(shell.input);
                shell.input = NULL;
                shell.error = 0;
                continue;
            }
            //while (token)
            //{
            //    char *s = f(token->type);
            //    printf("%s --> %s", token->value, s);
            //    if (token->concate)
            //        printf("🔗🔗🔗🔗");
            //    printf("\n");
            //    token = token->next;
            //}
            t_ast *ast = parser(&token, &shell);

            
            //if (!ast)
            //{
            //    printf("\n🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴\n");
            //    continue;
            //}
            //print_ast(ast, get_ast_depth(ast));
            //printf("\n\n\n");
            set_varss(ast, &shell);
            //print_vars(ast);
            shell.error = 0;  
            shell.is_wild = 0;
            ast = NULL;
            // Execute command (i'll implement this next)
            // execute_command(shell.cmd, &shell);
        }
    }

    //cleanup_shell(&shell);
    return (0/*shell.last_status*/);
}
//ladsf af laskf | sfklj (())LKSDJ LD