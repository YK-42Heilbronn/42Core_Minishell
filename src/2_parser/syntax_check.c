/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:56:06 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 00:32:26 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

int	print_syntax_error(char *token)
{
	printf("minishell: %s\n", token);
	return (1);
}

int	is_redir_token(t_token_type type)
{
	if (type == TOK_REDIR_IN || type == TOK_REDIR_OUT || type == TOK_APPEND
		|| type == TOK_HEREDOC)
		return (1);
	return (0);
}
int print_multi_str_error(char *s1, char *s2)
{
	char *join_s;

	join_s = ft_strjoin(s1, s2);
	print_syntax_error(join_s);
	free(join_s);
	return (1);
}
static int	redir_checks(t_token *token)
{
	t_token	*next;

	next = token->next;
	if (!next)
		return (print_syntax_error("syntax error"));
	if (next->type == TOK_PIPE)
		return (print_syntax_error("|"));
	if (is_redir_token(next->type))
		return (print_multi_str_error("syntax error ", next->value));
	if (next->type != TOK_WORD)
		return (print_multi_str_error("syntax error ", next->value));
	return (0);
}

static int	pipe_checks(t_token *token)
{
	t_token	*next;

	next = token->next;
	if (!next)
		return (print_syntax_error("syntax error"));
	if (next->type == TOK_PIPE)
		return (print_syntax_error("parse error near '|'"));
	return (0);
}

static int	validate_end_nodes(t_token *tokens)
{
	t_token	*last;

	if (!tokens)
		return (0);
	if (tokens->type == TOK_PIPE)
		return (print_syntax_error("parse error near '|'"));
	last = tokens;
	while (last->next)
		last = last->next;
	if (last->type == TOK_PIPE)
		return (print_syntax_error("parse error near '|'"));
	return (0);
}

int	syntax_check_tokens(t_token *tokens)
{
	t_token	*cur;

	if (!tokens)
		return (0);
	if (validate_end_nodes(tokens))
		return (1);
	cur = tokens;
	while (cur)
	{
		if (cur->type == TOK_PIPE && pipe_checks(cur))
			return (1);
		if (is_redir_token(cur->type) && redir_checks(cur))
			return (1);
		cur = cur->next;
	}
	return (0);
}
