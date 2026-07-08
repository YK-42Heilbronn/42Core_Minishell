/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:04 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 19:23:41 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	handle_pipe(t_token **tokens, const char *line, int i)
{
	if (line[i] == '|')
	{
		append_new_token(tokens, TOK_PIPE, "|");
		return (i + 1);
	}
	return (i);
}

static int	handle_redir(t_token **tokens, const char *line, int i)
{
	if (line[i] == '<' && line[i + 1] == '<')
		return (append_new_token(tokens, TOK_HEREDOC, "<<"), i + 2);
	if (line[i] == '>' && line[i + 1] == '>')
		return (append_new_token(tokens, TOK_APPEND, ">>"), i + 2);
	if (line[i] == '<')
		return (append_new_token(tokens, TOK_REDIR_IN, "<"), i + 1);
	if (line[i] == '>')
		return (append_new_token(tokens, TOK_REDIR_OUT, ">"), i + 1);
	return (i);
}

static int	handle_word(t_token **tokens, const char *line, int i)
{
	char	*value;
	int		len;

	len = word_len(line, i);
	value = extract_word(line, i, len);
	append_new_token(tokens, TOK_WORD, value);
	return (i + len);
}

static int	handle_token(t_token **tokens, const char *line, int i)
{
	if (line[i] == '|')
		return (handle_pipe(tokens, line, i));
	if (line[i] == '<' || line[i] == '>')
		return (handle_redir(tokens, line, i));
	return (handle_word(tokens, line, i));
}

t_token	*lex_input(const char *line, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	(void)shell;
	if (!line)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && is_whitespace(line[i]))
			i++;
		if (!line[i])
			break ;
		i = handle_token(&tokens, line, i);
	}
	return (tokens);
}
