/*
#include "../../includes/minishell.h"

void print_tokens(t_token *tokens)
{
    printf("Tokens:\n");
    while (tokens)
    {
        printf("Type: %d, Value: '%s'\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
    printf("\n");
}

void print_command(t_command *cmd)
{
    int i;
    printf("Command:\n");
    while (cmd)
    {
        printf("Arguments: ");
        i = 0;
        while (cmd->args && cmd->args[i])
        {
            printf("'%s' ", cmd->args[i]);
            i++;
        }
        printf("\n");
        if (cmd->next)
            printf("Pipe to next command\n");
        cmd = cmd->next;
    }
}

int main()
{
    // Test case 1: Simple command
    char *input1 = "ls -l";
    printf("Input: %s\n", input1);
    t_token *tokens1 = tokenize_input(input1);
    print_tokens(tokens1);
    t_command *cmd1 = parse_tokens(tokens1);
    print_command(cmd1);
    
    // Test case 2: Pipeline
    char *input2 = "ls -l | grep test | wc -l";
    printf("\nInput: %s\n", input2);
    t_token *tokens2 = tokenize_input(input2);
    print_tokens(tokens2);
    t_command *cmd2 = parse_tokens(tokens2);
    print_command(cmd2);

    // Cleanup
    free_tokens(tokens1);
    free_tokens(tokens2);
    // Need to implement command cleanup
    
    return 0;
}
*/
// run it like this from this path : gcc test.c parser.c ../lexer/lexer.c ../../libft/libft.a