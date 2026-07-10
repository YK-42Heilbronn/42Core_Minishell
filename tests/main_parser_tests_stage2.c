/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser_tests_stage2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:19:25 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/10 14:30:00 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

// typedef struct s_cmd
// {
// 	char			**argv;
// 	t_redir			*redirs;
// 	struct s_cmd	*next;
// }	t_cmd;

static const char	*token_type_name(t_token_type type)
{
	if (type == TOK_WORD)
		return ("TOK_WORD");
	if (type == TOK_PIPE)
		return ("TOK_PIPE");
	if (type == TOK_REDIR_IN)
		return ("TOK_REDIR_IN");
	if (type == TOK_REDIR_OUT)
		return ("TOK_REDIR_OUT");
	if (type == TOK_APPEND)
		return ("TOK_APPEND");
	if (type == TOK_HEREDOC)
		return ("TOK_HEREDOC");
	return ("UNKNOWN");
}

static const char	*redir_type_name(t_redir_type type)
{
	if (type == R_IN)
		return ("R_IN");
	if (type == R_OUT)
		return ("R_OUT");
	if (type == R_APPEND)
		return ("R_APPEND");
	if (type == R_HEREDOC)
		return ("R_HEREDOC");
	return ("UNKNOWN");
}

static void	print_tokens(t_token *tokens)
{
	printf("TOKENS:\n");
	while (tokens)
	{
		printf("  [%s] value=\"%s\"\n",
			token_type_name(tokens->type), tokens->value);
		tokens = tokens->next;
	}
}

static void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("    redir: type=%s file=\"%s\"\n",
			redir_type_name(redir->type), redir->file);
		redir = redir->next;
	}
}

static void	print_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
	{
		printf("    argv: (null)\n");
		return ;
	}
	printf("    argv:");
	while (argv[i])
	{
		printf(" [%s]", argv[i]);
		i++;
	}
	printf("\n");
}

static void	print_cmds(t_cmd *cmds)
{
	int	i;

	i = 1;
	printf("PARSED COMMANDS:\n");
	while (cmds)
	{
		printf("  cmd #%d\n", i);
		print_argv(cmds->argv);
		print_redirs(cmds->redirs);
		cmds = cmds->next;
		i++;
	}
}

static void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir);
		redir = next;
	}
}

static void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;

	while (cmds)
	{
		next = cmds->next;
		if (cmds->argv)
			free(cmds->argv);
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = next;
	}
}

static void	run_test(const char *input)
{
	t_shell	shell;
	t_token	*tokens;
	t_cmd	*cmds;

	shell.env = NULL;
	shell.last_status = 0;
	shell.line = (char *)input;
	shell.tokens = NULL;
	shell.cmds = NULL;
	shell.stdin_fd = 0;
	shell.stdout_fd = 1;
	shell.heredoc_fd = -1;
	shell.exit_flag = 0;
	printf("========================================\n");
	printf("INPUT: %s\n", input);
	if (has_unclosed_quotes(input))
	{
		printf("QUOTE CHECK: FAIL (unclosed quotes)\n\n");
		return ;
	}
	printf("QUOTE CHECK: PASS\n");
	tokens = lex_input(input, &shell);
	if (!tokens)
	{
		printf("LEXER: FAIL\n\n");
		return ;
	}
	shell.tokens = tokens;
	print_tokens(tokens);
	if (syntax_check_tokens(tokens))
	{
		printf("SYNTAX CHECK: FAIL\n\n");
		free_tokens(tokens);
		return ;
	}
	printf("SYNTAX CHECK: PASS\n");
	cmds = parse_tokens(tokens, &shell);
	if (!cmds)
	{
		printf("PARSER: FAIL\n\n");
		free_tokens(tokens);
		return ;
	}
	printf("PARSER: PASS\n");
	print_cmds(cmds);
	printf("\n");
	free_cmds(cmds);
	free_tokens(tokens);
}

int	main(void)
{
	const char	*tests[] = {
		"ls",
		"echo hello world",
		"cat < infile",
		"echo hi > out.txt",
		"echo hi >> log.txt",
		"cat << EOF",
		"grep main < in.txt > out.txt",
		"echo hello | wc",
		"echo hello | wc -c",
		"cat < infile | grep abc > outfile",
		"| ls",
		"ls |",
		"ls || wc",
		"echo >",
		"echo <",
		"echo > | wc",
		"echo > > out",
		"cat < << eof",
		"echo \"hello",
		NULL
	};
	int			i;

	i = 0;
	while (tests[i])
	{
		run_test(tests[i]);
		i++;
	}
	return (0);
}

