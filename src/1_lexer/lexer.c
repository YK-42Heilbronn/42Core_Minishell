/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:04 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:14:57 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	handle_pipe(t_token **tokens, const char *line, int i)
{
	(void)line;
	if (!append_token_dup(tokens, TOK_PIPE, "|"))
		return (-1);
	return (i + 1);
}

static int	handle_redir_inout(t_token **tokens, const char *line, int i)
{
	if (line[i] == '<')
	{
		if (append_token_dup(tokens, TOK_REDIR_IN, "<"))
			return (i + 1);
		else
			return (-1);
	}
	if (line[i] == '>')
	{
		if (append_token_dup(tokens, TOK_REDIR_OUT, ">"))
			return (i + 1);
		else
			return (-1);
	}
	return (i);
}

static int	handle_redir(t_token **tokens, const char *line, int i)
{
	if (line[i] == '<' && line[i + 1] == '<')
	{
		if (append_token_dup(tokens, TOK_HEREDOC, "<<"))
			return (i + 2);
		else
			return (-1);
	}
	if (line[i] == '>' && line[i + 1] == '>')
	{
		if (append_token_dup(tokens, TOK_APPEND, ">>"))
			return (i + 2);
		else
			return (-1);
	}
	return (handle_redir_inout(tokens, line, i));
}

static int	handle_word(t_token **tokens, const char *line, int i)
{
	char	*value;
	int		len;

	len = word_len(line, i);
	value = extract_word(line, i, len);
	if (!append_token_owned(tokens, TOK_WORD, value))
		return (-1);
	return (i + len);
}

// returns a linked list of tokens or NULL on error
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
		if (line[i] == '|')
			i = handle_pipe(&tokens, line, i);
		else if (line[i] == '<' || line[i] == '>')
			i = handle_redir(&tokens, line, i);
		else
			i = handle_word(&tokens, line, i);
		if (i == -1)
			return (free_tokens(&tokens), NULL);
	}
	return (tokens);
}
