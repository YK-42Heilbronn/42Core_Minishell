/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:04 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/16 21:30:38 by ykonka           ###   ########.fr       */
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

static int	handle_redir(t_token **tokens, const char *line, int i)
{
	if (line[i] == '<' && line[i + 1] == '<')
		return (append_token_dup(tokens, TOK_HEREDOC, "<<") ? i + 2 : -1);
	if (line[i] == '>' && line[i + 1] == '>')
		return (append_token_dup(tokens, TOK_APPEND, ">>") ? i + 2 : -1);
	if (line[i] == '<')
		return (append_token_dup(tokens, TOK_REDIR_IN, "<") ? i + 1 : -1);
	if (line[i] == '>')
		return (append_token_dup(tokens, TOK_REDIR_OUT, ">") ? i + 1 : -1);
	return (i);
}

static int	handle_word(t_token **tokens, const char *line, int i)
{
	char	*value;
	int		len;

	len = word_len(line, i);
	value = extract_word(line, i, len);
	// printf("value::%s\n", value);
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
	// printf("%s\n", line);
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
