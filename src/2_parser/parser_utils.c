/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:36 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/15 20:39:04 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

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

// void free_cmds(t_cmd *cmd)
// {
// 	if (!cmd)
// 		return;
// 	if (cmd->argv)
// 	{
// 		for (int i = 0; cmd->argv[i]; i++)
// 			free(cmd->argv[i]);
// 		free(cmd->argv);
// 	}
// 	free_redirs(cmd->redirs);
// 	free(cmd);
// }

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
