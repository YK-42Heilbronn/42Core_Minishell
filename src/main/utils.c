/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 00:02:15 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:48:43 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

/* every hardcoded write(fd, "literal", N) is one off-by-one bug waiting
 * to happen; route error/message output through this instead. */
void	write_str(int fd, const char *s)
{
	write(fd, s, ft_strlen(s));
}

/* stderr messages must go through write(), not printf (printf only
 * targets stdout and isn't safe to assume line-buffered on a pipe). */
void	print_exec_error(char *name, char *msg)
{
	write_str(2, "minishell: ");
	if (name)
		write_str(2, name);
	write_str(2, ": ");
	write_str(2, msg);
	write_str(2, "\n");
}

void	free_cmd(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*next_redir;
	int		i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd->argv && cmd->argv[i])
		free(cmd->argv[i++]);
	free(cmd->argv);
	redir = cmd->redirs;
	while (redir)
	{
		next_redir = redir->next;
		free(redir->file);
		if (redir->fd != -1)
			close(redir->fd);
		free(redir);
		redir = next_redir;
	}
	free(cmd);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;

	while (cmds)
	{
		next = cmds->next;
		free_cmd(cmds);
		cmds = next;
	}
}

/* sweeps up any stray heredoc-writer children that may still be around
 * (they normally finish on their own once the reading command is done). */
void	reap_zombie_children(void)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

void	set_status_from_wait(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write_str(2, "Quit (core dumped)\n");
		else if (WTERMSIG(status) == SIGINT)
			write_str(1, "\n");
	}
}

// int	cmd_count(t_cmd *cmds)
// {
// 	int	n;

// 	n = 0;
// 	while (cmds)
// 	{
// 		n++;
// 		cmds = cmds->next;
// 	}
// 	return (n);
// }
