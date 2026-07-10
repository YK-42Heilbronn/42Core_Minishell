/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:56:22 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/10 14:10:34 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"
#include "libft.h"
#include <stdlib.h>

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *));
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->argv[0] = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	cmd_add_arg(t_cmd *cmd, char *word)
{
	char	**new_argv;
	int		count;
	int		i;

	if (!cmd || !word)
		return (0);
	count = 0;
	while (cmd->argv[count])
		count++;
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return (0);
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = word;
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	return (1);
}

t_cmd	*parse_command(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd;

	(void)shell;
	if (!tokens || !*tokens)
		return (NULL);
	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	while (*tokens && (*tokens)->type != TOK_PIPE)
	{
		if ((*tokens)->type == TOK_WORD)
		{
			if (!cmd_add_arg(cmd, (*tokens)->value))
				return (NULL);
			*tokens = (*tokens)->next;
		}
		else if (is_redir_token((*tokens)->type))
		{
			if (!parse_redirection(cmd, tokens))
				return (NULL);
		}
		else
			break ;
	}
	return (cmd);
}
