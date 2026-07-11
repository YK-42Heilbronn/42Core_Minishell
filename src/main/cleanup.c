/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:17 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/11 16:42:45 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"

void free_redirs(t_redir *redirs)
{
    t_redir *next;
    
    while (redirs)
    {
        next = redits->next;
        free(redirs);
        redirs = next;
    }
}

void    free_cmds(t_cmd *cmd)
{
    t_cmd   *next;
    
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

void    cleanup_command(t_shell *shell)
{
    if (!shell)
        return ;
    if (shell->tokens)
    {
        free_tokens(shell->tokens);
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
