/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:01:17 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 12:54:26 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Split input into typed tokens, detect quote errors
#ifndef TOKEN_H
# define TOKEN_H

typedef enum e_token_type
{
    TOK_WORD,
    TOK_PIPE,
    TOK_REDIR_IN,
    TOK_REDIR_OUT,
    TOK_APPEND,
    TOK_HEREDOC,
    TOK_EOF
}   t_token_type;

// @Comment: Linkedlist
typedef struct s_token
{
    t_token_type    type;
    char            *value;
    int             quoted_single;
    int             quoted_double;
    struct s_token  *next;
}   t_token;

#endif
