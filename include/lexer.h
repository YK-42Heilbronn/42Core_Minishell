/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:01:17 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 19:38:10 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Split input into typed tokens, detect quote errors
#ifndef TOKEN_H
# define TOKEN_H
// # include "libs\libft\libft.h"
# include "libft.h"
# include "stdio.h"
# include <stdlib.h>

typedef struct s_shell	t_shell;

// Enumeration: [1, 2, 3, 4, 5, 6, 7]
typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_EOF
}						t_token_type;

// @Comment: Linkedlist
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	int					quoted_single;
	int					quoted_double;
	struct s_token		*next;
}						t_token;

// lexer.c
t_token					*lex_input(const char *line, t_shell *shell);

// token_create.c
t_token					*new_token(t_token_type type, char *value);
void					add_token(t_token **list, t_token *new_tok);

// lexer_utils.c
void					free_tokens(t_token *list);
void					append_new_token(t_token **tokens, t_token_type type,
							char *value);

// token_utils.c
int						is_operator_char(char c);
int						is_whitespace(char c);
int						word_len(const char *line, int i);
char					*extract_word(const char *line, int start, int len);

#endif
