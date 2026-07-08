/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:01:03 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 16:02:04 by ykonka           ###   ########.fr       */
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
    t_token *tokens;
    t_cmd   *cmds;
}   t_shell;

#endif
