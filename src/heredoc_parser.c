/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:48:12 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/23 20:48:38 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *process_heredoc_delimiter(const char *raw_delimiter)
{
    char *result;
    int quoted = 0;
    
    // Check if delimiter ends with quotes
    size_t len = strlen(raw_delimiter);
    if (len > 0 && (raw_delimiter[len-1] == '"' || raw_delimiter[len-1] == '\''))
        quoted = 1;
        
    if (!quoted)
        return strdup(raw_delimiter); // Return exact copy for unquoted delimiter
        
    // For quoted delimiter, process $ signs
    char *temp = malloc(len + 1);
    int j = 0;
    
    // Process $ signs in pairs
    for (int i = 0; i < len - 1; i++) {  // -1 to exclude closing quote
        if (raw_delimiter[i] == '$') {
            if (i + 1 < len - 1 && raw_delimiter[i + 1] == '$') {
                // Keep paired $$ signs
                temp[j++] = '$';
                temp[j++] = '$';
                i++; // Skip next $
            }
            // Ignore unpaired $ signs
        } else {
            temp[j++] = raw_delimiter[i];
        }
    }
    temp[j] = '\0';
    
    // Remove quotes
    result = remove_quotes(temp);
    free(temp);
    
    return result;
}