/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_expander_tests_stage1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 18:05:07 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/15 01:22:01 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <stdio.h>

static void	test_expand_word(t_shell *shell, char *input, int sq, int dq)
{
	char	*out;

	out = expand_word(input, shell, sq, dq);
	printf("in=[%s] sq=%d dq=%d out=[%s]\n", input, sq, dq, out ? out : "NULL");
	free(out);
}

static void	test_one_token(t_shell *shell, char *value, int type, int sq,
		int dq)
{
	t_token	token;

	token.type = type;
	token.value = ft_strdup(value);
	token.s_quote = sq;
	token.d_quote = dq;
	token.next = NULL;
	if (expand_one_token(&token, shell))
		printf("token ok: [%s]\n", token.value);
	else
		printf("token fail\n");
	free(token.value);
}

static t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

static void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*tmp;

	if (!env || !new_node)
		return ;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

int	main(void)
{
	t_shell	shell;
	char	*result;

	shell.env = NULL;
	shell.last_status = 42;
	shell.line = NULL;
	shell.tokens = NULL;
	shell.cmds = NULL;
	shell.stdin_fd = 0;
	shell.stdout_fd = 1;
	shell.heredoc_fd = -1;
	shell.exit_flag = 0;
	add_env_node(&shell.env, new_env_node("USER", "ykonka"));
	add_env_node(&shell.env, new_env_node("HOME", "/home/ykonka"));
	add_env_node(&shell.env, new_env_node("TEST", "42"));
	result = expand_word("hello_$USER", &shell, 0, 0);
	printf("result = [%s]\n", result);
	free(result);
	result = expand_word("$?", &shell, 0, 0);
	printf("status = [%s]\n", result);
	free(result);
	test_expand_word(&shell, "hello", 0, 0);
	test_expand_word(&shell, "$?", 0, 0);
	test_expand_word(&shell, "$USER", 0, 0);
	test_expand_word(&shell, "$USER", 1, 0);
	test_expand_word(&shell, "abc$USER", 0, 0);
	test_expand_word(&shell, "$NOT_SET", 0, 0);
	test_expand_word(&shell, "$", 0, 0);
	test_one_token(&shell, "$USER", TOK_WORD, 0, 0);
	test_one_token(&shell, "$USER", TOK_WORD, 1, 0);
	test_one_token(&shell, "|", TOK_PIPE, 0, 0);
	return (0);
}
