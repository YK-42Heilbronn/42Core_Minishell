/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:01:17 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:50:15 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "libft.h"
# include "stdio.h"
# include <stdlib.h>

typedef struct s_shell	t_shell;
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
// quoted: {'no': 0, 'single': 1, 'double': 2}
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	int					quoted;
	struct s_token		*next;
}						t_token;

// lexer.c
t_token					*lex_input(const char *line, t_shell *shell);

// token.c
t_token					*new_token(t_token_type type, char *value);
void					add_token(t_token **list, t_token *new_tok);
void					update_quotes_state(t_token *token);

// lexer_utils.c
void					free_tokens(t_token **list);
int						append_token_dup(t_token **tokens, t_token_type type,
							char *value);
int						append_token_owned(t_token **tokens, t_token_type type,
							char *value);

// token_utils.c
int						is_operator_char(char c);
int						is_whitespace(char c);
int						word_len(const char *line, int i);
char					*extract_word(const char *line, int start, int len);

#endif
