/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:01:03 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 21:12:19 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Build t_cmd objects from tokens, detect syntax errors around pipes and redirections
//  parser output, environment data, and shell state
#ifndef COMMAND_H
# define COMMAND_H
// # include "lexer.h"

typedef struct s_token t_token;
typedef struct s_cmd   t_cmd;

typedef enum e_redir_type
{
    R_IN,
    R_OUT,
    R_APPEND,
    R_HEREDOC
}   t_redir_type;

// @Comment: Linkedlist
typedef struct s_redir
{
    t_redir_type     type;
    char             *file;
    struct s_redir   *next;
}   t_redir;

// @Comment: Linkedlist
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_shell
{
    t_env   *env;
    int     last_status;
    char    *line; //stores the current realine
    t_token *tokens; 
    t_cmd   *cmds;
    int     stdin_fd; //useful if you temporarily redirect and need to restore descriptors
    int     stdout_fd; //useful if you temporarily redirect and need to restore descriptors
    int     heredoc_fd; //convenient if you implement heredoc as a temporary input source
    int     exit_flag; //makes it easier to stop the loop from builtins or fatal errors
}   t_shell;

#endif
