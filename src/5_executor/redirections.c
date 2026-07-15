/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:01:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:47:39 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

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


// int	open_redir_fd(t_shell *shell, t_redir *r)
// {
// 	if (r->type == R_HEREDOC)
// 		return (shell->heredoc_fd);
// 	if (r->type == R_IN)
// 		return (open(r->file, O_RDONLY));
// 	if (r->type == R_OUT)
// 		return (open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
// 	if (r->type == R_APPEND)
// 		return (open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644));
// 	return (-1);
// }

// int	apply_input_redir(t_redir *r, int fd)
// {
// 	if (dup2(fd, STDIN_FILENO) < 0)
// 	{
// 		perror("dup2");
// 		if (r->type != R_HEREDOC)
// 			close(fd);
// 		return (1);
// 	}
// 	if (r->type != R_HEREDOC)
// 		close(fd);
// 	return (0);
// }

// int	apply_output_redir(int fd)
// {
// 	if (dup2(fd, STDOUT_FILENO) < 0)
// 	{
// 		perror("dup2");
// 		close(fd);
// 		return (1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	apply_one_redir(t_shell *shell, t_redir *r)
// {
// 	int	fd;

// 	fd = open_redir_fd(shell, r);
// 	if (fd < 0)
// 	{
// 		perror(r->file);
// 		return (1);
// 	}
// 	if (r->type == R_HEREDOC || r->type == R_IN)
// 		return (apply_input_redir(r, fd));
// 	return (apply_output_redir(fd));
// }

// int	apply_redirs(t_shell *shell, t_cmd *cmd, int is_child)
// {
// 	t_redir	*r;

// 	(void)is_child;
// 	if (!shell || !cmd)
// 		return (1);
// 	r = cmd->redirs;
// 	while (r)
// 	{
// 		if (apply_one_redir(shell, r))
// 			return (1);
// 		r = r->next;
// 	}
// 	return (0);
// }
