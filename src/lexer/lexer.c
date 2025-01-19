#include "../../includes/minishell.h"

// Function to create a new token
// Parameters:
// - type: The type of the token (e.g., word, operator)
// - value: The string value of the token
// Returns: A pointer to the newly created token
static t_token *create_token(t_token_type type, char *value)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    
    token->type = type;
    token->value = value;
    token->next = NULL;
    return token;
}

// Function to add a new token to the end of the token list
// Parameters:
// - head: Pointer to the head of the token list
// - new_token: The new token to be added
// Returns: Nothing; updates the token list in place
static void add_token(t_token **head, t_token *new_token)
{
    t_token *current;

    if (!*head)
    {
        *head = new_token;
        return;
    }

    current = *head;
    while (current->next)
        current = current->next;

    current->next = new_token;
}

// Function to tokenize the input string into a linked list of tokens
// Parameters:
// - input: The input string to be tokenized
// Returns: A pointer to the head of the linked list of tokens
t_token *tokenize_input(char *input)
{
    t_token *head = NULL;
    char *start;
    int i = 0;

    while (input[i])
    {
        // Skip whitespace
        while (input[i] && ft_isspace(input[i]))
            i++;
        if (!input[i])
            break;

        // Handle operators
        if (ft_is_operator(input[i]))
        {
            t_token_type type;
            if (input[i] == '|')
                type = TOKEN_PIPE;
            else if (input[i] == '<')
                type = TOKEN_REDIRECT_IN;
            else if (input[i] == '>')
                type = TOKEN_REDIRECT_OUT;

            char *op = ft_substr(input, i, 1);
            add_token(&head, create_token(type, op));
            i++;
            continue;
        }

        // Handle words
        start = &input[i];
        while (input[i] && !ft_isspace(input[i]) && !ft_is_operator(input[i]))
            i++;

        char *word = ft_substr(start, 0, &input[i] - start);
        add_token(&head, create_token(TOKEN_WORD, word));
    }

    return head;
}
