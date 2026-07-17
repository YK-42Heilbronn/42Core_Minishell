/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:36 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:21:07 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

int	print_multi_str_error(char *s1, char *s2)
{
	char	*join_s;

	join_s = ft_strjoin(s1, s2);
	print_syntax_error(join_s);
	free(join_s);
	return (1);
}

int	has_unclosed_quotes(const char *line)
{
	int	quote;

	quote = 0;
	while (*line)
	{
		if (!quote && (*line == '"' || *line == '\''))
			quote = *line;
		else if (quote && *line == quote)
			quote = 0;
		line++;
	}
	return (quote != 0);
}
