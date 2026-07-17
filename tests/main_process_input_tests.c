
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
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

static int	count_cmds_local(t_cmd *cmds)
{
	int	n;

	n = 0;
	while (cmds)
	{
		n++;
		cmds = cmds->next;
	}
	return (n);
}

static int	count_redirs_by_type(t_redir *redirs, t_redir_type type)
{
	int	n;

	n = 0;
	while (redirs)
	{
		if (redirs->type == type)
			n++;
		redirs = redirs->next;
	}
	return (n);
}

static void	print_joined_argv(char **argv)
{
	int	i;

	if (!argv || !argv[0])
	{
		printf("(empty)");
		return ;
	}
	i = 0;
	while (argv[i])
	{
		if (i > 0)
			printf(",");
		printf("%s", argv[i]);
		i++;
	}
}

static void	print_cmds_summary(t_cmd *cmds)
{
	int	i;

	i = 0;
	while (cmds)
	{
		printf("CMD[%d].ARGV=", i);
		print_joined_argv(cmds->argv);
		printf("\n");
		printf("CMD[%d].IN=%d OUT=%d APP=%d HEREDOC=%d\n", i,
			count_redirs_by_type(cmds->redirs, R_IN),
			count_redirs_by_type(cmds->redirs, R_OUT),
			count_redirs_by_type(cmds->redirs, R_APPEND),
			count_redirs_by_type(cmds->redirs, R_HEREDOC));
		cmds = cmds->next;
		i++;
	}
}

static void	init_test_shell(t_shell *shell, char **envp)
{
	memset(shell, 0, sizeof(*shell));
	shell->last_status = 42;
	if (env_init(shell, envp) != 0)
	{
		fprintf(stderr, "env_init failed\n");
		exit(2);
	}
}

static void	cleanup_test_shell(t_shell *shell)
{
	if (shell->tokens)
		free_tokens(&(shell->tokens));
	if (shell->cmds)
		free_cmds(shell->cmds);
	if (shell->env)
		free_env(&shell->env);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		ret;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s \"input line\"\n", argv[0]);
		return (2);
	}
	init_test_shell(&shell, envp);
	ret = process_input(argv[1], &shell);
	printf("RET=%d\n", ret);
	if (ret == 0)
	{
		printf("CMDS=%d\n", count_cmds_local(shell.cmds));
		print_cmds_summary(shell.cmds);
		cleanup_test_shell(&shell);
		return (0);
	}
	cleanup_test_shell(&shell);
	return (1);
}
