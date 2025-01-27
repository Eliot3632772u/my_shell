#include "../../includes/minishell.h"

//// Function to create a new command structure
//// Returns: A pointer to the newly created t_command structure
//static t_command *create_command(void)
//{
//    t_command *cmd = malloc(sizeof(t_command));
//    if (!cmd)
//        return NULL;
    
//    cmd->args = NULL;
//    cmd->redirects = NULL;
//    cmd->pipe_read = -1;
//    cmd->pipe_write = -1;
//    cmd->next = NULL;
//    return cmd;
//}

//// Function to add an argument to a command's argument list
//// Parameters:
//// - cmd: Pointer to the command structure
//// - arg: The argument string to add
//static void add_argument(t_command *cmd, char *arg)
//{
//    int i = 0;
//    char **new_args;

//    if (cmd->args)
//        while (cmd->args[i])
//            i++;

//    new_args = malloc(sizeof(char *) * (i + 2));
//    if (!new_args)
//        return;

//    int j = 0;
//    while (j < i)
//    {
//        new_args[j] = cmd->args[j];
//        j++;
//    }

//    new_args[i] = ft_strdup(arg);
//    new_args[i + 1] = NULL;

//    free(cmd->args);
//    cmd->args = new_args;
//}

//// Function to parse a list of tokens into a list of commands
//// Parameters:
//// - tokens: Pointer to the head of the token list
//// Returns: A pointer to the head of the command list
//t_command *parse_tokens(t_token *tokens)
//{
//    t_command *cmd = create_command();
//    t_command *head = cmd;

//    while (tokens)
//    {
//        if (tokens->type == TOKEN_WORD)
//        {
//            add_argument(cmd, tokens->value);
//        }
//        else if (tokens->type == TOKEN_PIPE)
//        {
//            cmd->next = create_command();
//            cmd = cmd->next;
//        }
//        tokens = tokens->next;
//    }

//    return head;
//}

//// Function to free a list of tokens
//// Parameters:
//// - tokens: Pointer to the head of the token list to be freed
//void free_tokens(t_token *tokens)
//{
//    t_token *next;

//    while (tokens)
//    {
//        next = tokens->next;
//        free(tokens->value);
//        free(tokens);
//        tokens = next;
//    }
//}

t_ast   *new_cmd(t_token *token)
{
    t_ast   *tmp;
    t_token *list;

    tmp = malloc(sizeof(t_ast));
    if (!tmp == NULL)
        return (NULL);
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->redc = NULL;
    tmp->args = NULL;
    tmp->type = ast_cmd;
    tmp->tok_args = token;
    list = token;
    while (list && (list->type == WORD || list->type == WORD_VAR || list->type == VARIABL || list->type == WILD))
        list = list->next;
    list = list->prev;
    list->next->prev = NULL;
    list->next = NULL; 
    return (tmp);
}

t_ast   *parse_sub(t_token **tok)
{

}

t_ast	*parse_cmd(t_token **tok)
{
    t_token *tmp;
    t_ast   *ast;

    if ((*tok)->type == O_P)
        return (parse_sub(tok));
    tmp = *tok;
    while (*tok && ((*tok)->type == WORD || (*tok)->type == WORD_VAR || (*tok)->type == VARIABL || (*tok)->type == WILD))
        (*tok) = (*tok)->next;
    ast = new_cmd(tmp);
    return (ast);
}

t_redirect  *new_redic(t_token_type type)
{
    t_redirect  *new;

    new = malloc(sizeof(t_redirect));
    if (!new)
        return (NULL);
    new->type = type;
    new->file = NULL;
    new->tok_file = NULL;
    new->next = NULL;
    new->prev = NULL;
    return (new);
}

t_token *get_file_tokens(t_token **tok)
{
    t_token *tmp;

    (*tok) = (*tok)->next;
    tmp = (*tok);
    if (!(*tok) || ((*tok)->type != WORD && (*tok)->type != WORD_VAR && (*tok)->type != VARIABL && (*tok)->type != WILD))
    {
        // shell->error = unexpected token after >> // when there is no valid file or delemiter for herdoc
        return (NULL);
    }
    while ((*tok) && (*tok)->concate/*((*tok)->type != WORD && (*tok)->type != WORD_VAR && (*tok)->type != VARIABL && (*tok)->type != WILD)*/)
        (*tok) = (*tok)->next;
    (*tok)->prev->next = NULL;
    (*tok)->prev = NULL;
    (*tok) = (*tok)->next;
    return (tmp);
}

void    add_redic(t_redirect **redic, t_token **tok)
{
    t_redirect  *tmp;

    if (!(*redic))
    {
        *redic = new_redic((*tok)->type);
        if (!(*redic))
        {
            //handle malloc fail shell->error = ERR_MEMORY
            return ;
        }
        (*redic)->tok_file = get_file_tokens(tok);
        return ;
    }
    tmp = *redic;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_redic((*tok)->type);
    if (!(*redic))
    {
        //handle malloc fail shell->error = ERR_MEMORY
        return ;
    }
    tmp->next->tok_file = get_file_tokens(tok);
    return ;
}

t_ast   *parse_redi(t_token **tok)
{
    t_ast	*ast;

	ast = parse_cmd(tok);
    if (!ast)
        return (ast);
    while (*tok && ((*tok)->type == HEREDOC || (*tok)->type == APPEND || (*tok)->type == REDIRECT_IN || (*tok)->type == REDIRECT_OUT))
    {
        add_redic(&(ast->redc), tok);
        while (*tok && ((*tok)->type == WORD || (*tok)->type == WORD_VAR || (*tok)->type == VARIABL || (*tok)->type == WILD))
            add_arg(&(ast->tok_args), tok);
    }
}


t_ast   *parse_pipe(t_token **tok)
{
    t_ast	*ast;

	ast = parse_redir(tok);
	if (!ast)
		return (NULL);
}


t_ast   *parse_logical(t_token **tok)
{
    t_ast	*ast;

	ast = parse_pipe(tok);
	if (!ast)
		return (NULL);
}