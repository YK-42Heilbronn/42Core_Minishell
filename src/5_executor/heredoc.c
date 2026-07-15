/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 22:00:02 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/16 00:08:44 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

/* walks every command's redir list and collects every heredoc BEFORE any
 * command in the pipeline runs (this is what bash does too: it lets you
 * ctrl-C out of the whole pipeline while still typing heredoc input, and
 * it means the heredoc reads from the real terminal, never from a pipe
 * belonging to a previous command in the pipeline). */
int	handle_heredocs(t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = shell->cmds;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == R_HEREDOC && read_heredoc(redir, shell) == -1)
				return (-1);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

/* forks a small helper process that reads lines from the terminal (via
 * readline, so history/line-editing still work) until it sees the
 * delimiter or EOF, writing each (possibly expanded) line into a pipe.
 * The read end of that pipe becomes redir->fd for later use in
 * apply_redirections(). Reading in a child avoids blocking on the pipe's
 * fixed buffer size for large heredocs. */
int	read_heredoc(t_redir *redir, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), -1);
	if (pid == 0)
		heredoc_writer(pipefd, redir, shell);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write_str(1, "\n");
		shell->last_status = 130;
		close(pipefd[0]);
		return (-1);
	}
	redir->fd = pipefd[0];
	return (0);
}

int	append_dollar(char **result, const char *line, t_shell *shell,
		int *i)
{
	char	*name;
	char	*value;
	int		len;

	if (line[*i + 1] == '?')
	{
		value = expand_exit_status(shell);
		if (!value)
			return (0);
		*result = str_append_str(*result, value);
		free(value);
		*i += 2;
		return (*result != NULL);
	}
	if (!is_valid_var_start(line[*i + 1]))
	{
		*result = str_append_char(*result, '$');
		(*i)++;
		return (*result != NULL);
	}
	len = var_name_len(&line[*i + 1]);
	name = extract_var_name(&line[*i + 1], len);
	if (!name)
		return (0);
	value = env_get_value(shell->env, name);
	free(name);
	*result = str_append_str(*result, value);
	*i += len + 1;
	return (*result != NULL);
}

char	*expand_heredoc_line(const char *line, t_shell *shell)
{
	char	*result;
	int		i;

	if (!line)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			if (!append_dollar(&result, line, shell, &i))
				return (free(result), NULL);
			continue ;
		}
		result = str_append_char(result, line[i]);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

char	*choose_expand(char *line, t_redir *redir, t_shell *shell)
{
	if (redir->quoted)
		return (ft_strdup(line));
	return (expand_heredoc_line(line, shell));
}

void	write_heredoc_line(int fd, char *expanded)
{
	if (!expanded)
		return ;
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

int	is_delimiter(char *line, char *delim)
{
	return (ft_strlen(line) == ft_strlen(delim)
		&& ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0);
}

/* runs only in the forked child created by read_heredoc(). Never returns:
 * exits 0 on delimiter/EOF, or dies from SIGINT (default disposition, see
 * set_signals_heredoc) which the parent detects via WIFSIGNALED. */
void	heredoc_writer(int *pipefd, t_redir *redir, t_shell *shell)
{
	char	*line;
	char	*expanded;

	close(pipefd[0]);
	set_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write_str(2, "minishell: warning: here-document delimited by "
				"end-of-file (wanted `");
			write_str(2, redir->file);
			write_str(2, "')\n");
			break ;
		}
		if (is_delimiter(line, redir->file))
			return (free(line), close(pipefd[1]), exit(0));
		expanded = choose_expand(line, redir, shell);
		free(line);
		write_heredoc_line(pipefd[1], expanded);
	}
	close(pipefd[1]);
	exit(0);
}
