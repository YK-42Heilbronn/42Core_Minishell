/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:00:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:03:27 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

/* central exit path: frees what we own and terminates the current
 * process. Correct whether called from the foreground shell (single
 * "exit" command -> whole shell dies) or from a forked pipeline child
 * running "exit" (-> only that child dies). */

void	shell_exit(t_shell *shell, int status)
{
	free_env(&shell->env);
	free_cmds(shell->cmds);
	if (shell->line)
		free(shell->line);
	rl_clear_history();
	exit(status & 255);
}

void	run_one_line(t_shell *shell, char *line)
{
	shell->line = line;
	if (shell->line && *(shell->line))
		add_history(shell->line);
	if (has_unclosed_quotes(line))
	{
		write_str(2, "minishell: syntax error: unclosed quotes\n");
		shell->last_status = 2;
		return (free(line), (void)(shell->line = NULL));
	}
	if (process_input(line, shell) == 0)
		execute_cmds(shell);
	free_cmds(shell->cmds);
	shell->cmds = NULL;
	free_tokens(&(shell->tokens));
	shell->tokens = NULL;
	free(line);
	shell->line = NULL;
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (!shell->exit_flag)
	{
		set_signals_interactive();
		line = readline("minishell$ ");
		if (!line)
		{
			write_str(1, "exit\n");
			shell_exit(shell, shell->last_status);
		}
		run_one_line(shell, line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.env = NULL;
	shell.last_status = 0;
	shell.line = NULL;
	shell.tokens = NULL;
	shell.cmds = NULL;
	shell.stdin_fd = dup(STDIN_FILENO);
	shell.stdout_fd = dup(STDOUT_FILENO);
	shell.heredoc_fd = -1;
	shell.exit_flag = 0;
	if (env_init(&shell, envp))
		return (1);
	shell_loop(&shell);
	free_env(&shell.env);
	close(shell.stdin_fd);
	close(shell.stdout_fd);
	return (shell.last_status);
}
