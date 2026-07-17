/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:01:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 02:59:55 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

void	run_redir_only(t_cmd *cmd, t_shell *shell)
{
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (apply_redirections(cmd, shell) == -1)
		shell->last_status = 1;
	else
		shell->last_status = 0;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

int	open_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		print_exec_error(redir->file, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	return (close(fd), 0);
}

int	open_output_redir(t_redir *redir, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		print_exec_error(redir->file, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), -1);
	return (close(fd), 0);
}

/* redir->fd was already filled in by handle_heredocs() before the
 * pipeline/command started (see exec_heredoc.c). */

int	apply_heredoc_redir(t_redir *redir)
{
	if (redir->fd == -1)
		return (-1);
	if (dup2(redir->fd, STDIN_FILENO) == -1)
		return (close(redir->fd), -1);
	close(redir->fd);
	redir->fd = -1;
	return (0);
}

/* redirections are applied in the order they were parsed, so later ones
 * on the same fd naturally win, matching bash (`cmd < a < b` uses b). */

int	apply_redirections(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;

	(void)shell;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == R_IN && open_input_redir(redir) == -1)
			return (-1);
		if (redir->type == R_OUT && open_output_redir(redir, 0) == -1)
			return (-1);
		if (redir->type == R_APPEND && open_output_redir(redir, 1) == -1)
			return (-1);
		if (redir->type == R_HEREDOC && apply_heredoc_redir(redir) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
