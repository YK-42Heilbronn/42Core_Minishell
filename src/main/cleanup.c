/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:17 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/16 00:24:42 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->file);
		free(redirs);
		redirs = next;
	}
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->argv)
			free(cmd->argv);
		free_redirs(cmd->redirs);
		free(cmd);
		cmd = next;
	}
}

void	cleanup_command(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->tokens)
	{
		free_tokens(&(shell->tokens));
		shell->tokens = NULL;
	}
	if (shell->cmds)
	{
		free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	if (shell->heredoc_fd >= 0)
	{
		close(shell->heredoc_fd);
		shell->heredoc_fd = -1;
	}
}
