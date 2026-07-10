/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 14:51:26 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/09 15:50:01 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int has_unclosed_quotes(const char *line)
{
	int quotes;

	quotes = 0;
	while (*line)
	{
		if (*line == '"' || *line == '\'')
			quotes++;
	}
	if (quotes%2 == 0)
		return (0);
	else
		return (1);
}

// Rules
// Implement these exact checks in syntax_check_tokens():
// If tokens == NULL, return success; empty input is not a syntax error.
// For your current lexer design, the token after a redirection must be TOK_WORD.
// Practical meaning

// So these should be invalid:
// | ls
// ls |
// ls || wc
// echo >
// echo <
// echo >>
// cat <<
// echo > | wc
// echo > > out
// cat < << eof

// And these should be valid for syntax even if execution later fails:
// > out.txt echo hi if your project chooses to allow leading redirections like bash does.
// cat < infile
// echo hi > out.txt
// cat << EOF
// ls | wc

// A redirection token (<, >, <<, >>) must have a next token.
// A redirection token cannot be followed by a PIPE.
// A redirection token cannot be followed by another redirection token.
int redir_checks(t_token *tokens)
{
	t_token *next_tok;

	next_tok = tokens->next;  // assume that current token type is redir
	if (next_tok == NULL)
		return (0); // error
	if (next_tok->type == TOK_PIPE)
		return (0);
	if (next_tok->type == TOK_REDIR_IN
	|| next_tok->type == TOK_REDIR_OUT
	|| next_tok->type == TOK_APPEND
	|| next_tok->type == TOK_HEREDOC)
		return (0) // error
	return (1);  // accept
}

// A PIPE cannot be followed by another PIPE.
int pipe_checks(t_token *tokens)
{
	t_token *next_tok;

	next_tok = tokens;
	if (next_tok->type == TOK_PIPE)
	{
		next_tok = next_tok->next;
		if (next_tok->type == TOK_PIPE)
			return (0); // error
		else
			return (1); // accept
	}
}

// A PIPE cannot be the first token.
// A PIPE cannot be the last token.
int validate_end_nodes(t_token *tokens)
{
	t_token *next_tok;

	next_tok = tokens;
	if (next_tok->type == TOK_PIPE)
		return (1); // first=pipe
	while (next_tok->next)
		next_tok = next_tok->next;
	if (next_tok->type == TOK_PIPE)
		return (1); // last=pipe
	return (0);
}

int syntax_check_tokens(t_token *tokens)
{
	t_token *next_token;

	next_token = tokens;
	if (tokens == NULL)
		return (1);
	else
	{
		if (validate_end_nodes(tokens)) // 1: pipe token at end nodes
			return (0); // syntax check failed
		while (next_token->next)
		{
			if (next_token->type == TOK_PIPE)
			{
				if (pipe_checks(next_token))
				{
					next_token = next_token->next;
					// continue;
				}
				else
					return (0); // error
			} else if (next_token->type == TOK_REDIR_IN
				|| next_token->type == TOK_REDIR_OUT
				|| next_token->type == TOK_APPEND
				|| next_token->type == TOK_HEREDOC)
			{
				if (redir_checks(next_token))
				{
					next_token = next_token->next;
					// continue;
				}
				else
					return (0); // error
			}else  // if (next_token->type == TOK_WORD)
			{
				next_token = next_token->next;
				// continue;
			}
		}
		return (1);
	}
}

t_cmd	*parse_tokens(t_token *tokens, t_shell *shell)
{
	
}
