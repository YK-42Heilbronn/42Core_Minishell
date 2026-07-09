/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:36 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/09 17:20:14 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

// int	has_unclosed_quotes(const char *line)
// {
// 	char	quote;

// 	quote = 0;
// 	while (*line)
// 	{
// 		if (!quote && (*line == '\'' || *line == '"'))
// 			quote = *line;
// 		else if (quote && *line == quote)
// 			quote = 0;
// 		line++;
// 	}
// 	return (quote != 0);
// }
