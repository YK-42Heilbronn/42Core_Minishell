/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:01:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 21:48:04 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

int	open_redir_fd(t_shell *shell, t_redir *r)
{
	if (r->type == R_HEREDOC)
		return (shell->heredoc_fd);
	if (r->type == R_IN)
		return (open(r->file, O_RDONLY));
	if (r->type == R_OUT)
		return (open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (r->type == R_APPEND)
		return (open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

int	apply_input_redir(t_redir *r, int fd)
{
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		if (r->type != R_HEREDOC)
			close(fd);
		return (1);
	}
	if (r->type != R_HEREDOC)
		close(fd);
	return (0);
}

int	apply_output_redir(int fd)
{
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_one_redir(t_shell *shell, t_redir *r)
{
	int	fd;

	fd = open_redir_fd(shell, r);
	if (fd < 0)
	{
		perror(r->file);
		return (1);
	}
	if (r->type == R_HEREDOC || r->type == R_IN)
		return (apply_input_redir(r, fd));
	return (apply_output_redir(fd));
}

int	apply_redirs(t_shell *shell, t_cmd *cmd, int is_child)
{
	t_redir	*r;

	(void)is_child;
	if (!shell || !cmd)
		return (1);
	r = cmd->redirs;
	while (r)
	{
		if (apply_one_redir(shell, r))
			return (1);
		r = r->next;
	}
	return (0);
}
