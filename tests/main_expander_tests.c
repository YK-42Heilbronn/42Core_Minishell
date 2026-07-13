/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_expander_tests.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 18:40:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/13 05:09:20 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expander.h"
#include "parser.h"
#include "lexer.h"

static int	g_total = 0;
static int	g_pass = 0;

static void	test_result(const char *name, int ok)
{
	g_total++;
	if (ok)
	{
		g_pass++;
		printf("[OK]   %s\n", name);
	}
	else
		printf("[FAIL] %s\n", name);
}

static int	same_str(const char *a, const char *b)
{
	if (!a && !b)
		return (1);
	if (!a || !b)
		return (0);
	printf("a::%s||b::%s\n", a, b);
	return (strcmp(a, b) == 0);
}

/* ---------------- MOCK ENV ---------------- */

static t_env	*mock_env_new(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = strdup(key);
	node->value = strdup(value);
	node->next = NULL;
	return (node);
}

static void	mock_env_add_back(t_env **env, t_env *new_node)
{
	t_env	*cur;

	if (!env || !new_node)
		return ;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}

// char	*env_get_value(t_env *env, const char *key)
// {
// 	while (env)
// 	{
// 		if (env->key && strcmp(env->key, key) == 0)
// 			return (env->value);
// 		env = env->next;
// 	}
// 	return (NULL);
// }

static void	mock_free_env(t_env **env)
{
	t_env	*next;

	while (env && *env)
	{
		next = (*env)->next;
		free((*env)->key);
		free((*env)->value);
		free(*env);
		*env = next;
	}
}

/* ---------------- MOCK TOKENS ---------------- */

static t_token	*mock_token_new(t_token_type type, const char *value,
					int sq, int dq)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = strdup(value);
	tok->quoted_single = sq;
	tok->quoted_double = dq;
	tok->next = NULL;
	return (tok);
}

static void	mock_free_tokens(t_token *tok)
{
	t_token	*next;

	while (tok)
	{
		next = tok->next;
		free(tok->value);
		free(tok);
		tok = next;
	}
}

/* ---------------- TESTS ---------------- */

static void	test_is_valid_var_start(void)
{
	test_result("is_valid_var_start alpha", is_valid_var_start('A') == 1);
	test_result("is_valid_var_start underscore", is_valid_var_start('_') == 1);
	test_result("is_valid_var_start digit", is_valid_var_start('1') == 0);
	test_result("is_valid_var_start dollar", is_valid_var_start('$') == 0);
}

static void	test_is_valid_var_char(void)
{
	test_result("is_valid_var_char alpha", is_valid_var_char('Z') == 1);
	test_result("is_valid_var_char digit", is_valid_var_char('9') == 1);
	test_result("is_valid_var_char underscore", is_valid_var_char('_') == 1);
	test_result("is_valid_var_char dash", is_valid_var_char('-') == 0);
}

static void	test_var_name_len(void)
{
	test_result("var_name_len USER", var_name_len("USER") == 4);
	test_result("var_name_len USER123", var_name_len("USER123") == 7);
	test_result("var_name_len _ABC9+", var_name_len("_ABC9+") == 5);
	test_result("var_name_len empty", var_name_len("") == 0);
	test_result("var_name_len NULL", var_name_len(NULL) == 0);

	/*
	** This matches your current implementation:
	** first char is checked with is_valid_var_char(), not valid_var_start().
	*/
	test_result("var_name_len current behavior 1ABC", var_name_len("1ABC") == 4);
}

static void	test_extract_var_name(void)
{
	char	*name;

	name = extract_var_name("HOME=abc", 4);
	test_result("extract_var_name HOME", same_str(name, "HOME"));
	free(name);

	name = extract_var_name("_VAR123", 7);
	test_result("extract_var_name _VAR123", same_str(name, "_VAR123"));
	free(name);

	name = extract_var_name(NULL, 4);
	test_result("extract_var_name NULL", name == NULL);

	name = extract_var_name("ABC", 0);
	test_result("extract_var_name len 0", name == NULL);
}

static void	test_str_append_char(void)
{
	char	*s;

	s = str_append_char(NULL, 'A');
	test_result("str_append_char NULL", same_str(s, "A"));
	free(s);

	s = strdup("AB");
	s = str_append_char(s, 'C');
	test_result("str_append_char existing", same_str(s, "ABC"));
	free(s);
}

static void	test_str_append_str(void)
{
	char	*s;

	s = str_append_str(NULL, "hello");
	test_result("str_append_str NULL", same_str(s, "hello"));
	free(s);

	s = strdup("hello ");
	s = str_append_str(s, "world");
	test_result("str_append_str existing", same_str(s, "hello world"));
	free(s);

	s = strdup("abc");
	s = str_append_str(s, NULL);
	test_result("str_append_str NULL suffix", same_str(s, "abc"));
	free(s);
}

static void	test_expand_exit_status(void)
{
	t_shell	shell;
	char	*status;

	memset(&shell, 0, sizeof(shell));
	shell.last_status = 42;
	status = expand_exit_status(&shell);
	test_result("expand_exit_status 42", same_str(status, "42"));
	free(status);

	status = expand_exit_status(NULL);
	test_result("expand_exit_status NULL shell", status == NULL);
}

static void	test_expand_word_basic(void)
{
	t_shell	shell;
	char	*res;

	memset(&shell, 0, sizeof(shell));
	mock_env_add_back(&(shell.env), mock_env_new("USER", "ykonka"));
	mock_env_add_back(&(shell.env), mock_env_new("HOME", "/home/ykonka"));
	mock_env_add_back(&(shell.env), mock_env_new("EMPTY", ""));
	shell.last_status = 127;

	res = expand_word("$USER", &shell, 0, 0);
	test_result("expand_word $USER", same_str(res, "ykonka"));
	free(res);

	res = expand_word("hello $USER", &shell, 0, 0);
	test_result("expand_word hello $USER", same_str(res, "hello ykonka"));
	free(res);

	res = expand_word("$HOME", &shell, 0, 0);
	test_result("expand_word $HOME", same_str(res, "/home/ykonka"));
	free(res);

	res = expand_word("$EMPTY", &shell, 0, 0);
	test_result("expand_word empty var", same_str(res, ""));
	free(res);

	res = expand_word("$NOPE", &shell, 0, 0);
	test_result("expand_word missing var", same_str(res, ""));
	free(res);

	res = expand_word("$?", &shell, 0, 0);
	test_result("expand_word $?", same_str(res, "127"));
	free(res);

	mock_free_env(&shell.env);
}

static void	test_expand_word_edge_cases(void)
{
	t_shell	shell;
	char	*res;

	memset(&shell, 0, sizeof(shell));
	mock_env_add_back(&shell.env, mock_env_new("USER", "ykonka"));
	shell.last_status = 5;

	res = expand_word(NULL, &shell, 0, 0);
	test_result("expand_word NULL", res == NULL);

	res = expand_word("$USER", &shell, 1, 0);
	test_result("expand_word single quotes block", same_str(res, "$USER"));
	free(res);

	res = expand_word("$USER", &shell, 0, 1);
	test_result("expand_word double quotes allow", same_str(res, "ykonka"));
	free(res);

	res = expand_word("$", &shell, 0, 0);
	test_result("expand_word single dollar", same_str(res, "$"));
	free(res);

	res = expand_word("abc$", &shell, 0, 0);
	test_result("expand_word trailing dollar", same_str(res, "abc$"));
	free(res);

	res = expand_word("$1", &shell, 0, 0);
	test_result("expand_word invalid after dollar digit", same_str(res, "$1"));
	free(res);

	res = expand_word("$$", &shell, 0, 0);
	test_result("expand_word double dollar literal", same_str(res, "$$"));
	free(res);

	res = expand_word("$-abc", &shell, 0, 0);
	test_result("expand_word invalid after dollar dash", same_str(res, "$-abc"));
	free(res);

	res = expand_word("$USER$?", &shell, 0, 0);
	test_result("expand_word adjacent expansions", same_str(res, "ykonka5"));
	free(res);

	/*
	** This matches your current implementation:
	** USERy is read as one variable name.
	*/
	res = expand_word("x$USERy", &shell, 0, 0);
	test_result("expand_word current behavior x$USERy", same_str(res, "x"));
	free(res);

	mock_free_env(&shell.env);
}

static void	test_expand_one_token(void)
{
	t_shell	shell;
	t_token	*tok;

	memset(&shell, 0, sizeof(shell));
	mock_env_add_back(&shell.env, mock_env_new("USER", "ykonka"));
	shell.last_status = 99;

	test_result("expand_one_token NULL token", expand_one_token(NULL, &shell) == 1);

	tok = mock_token_new(TOK_PIPE, "|", 0, 0);
	test_result("expand_one_token non-word success", expand_one_token(tok, &shell) == 1);
	test_result("expand_one_token non-word unchanged", same_str(tok->value, "|"));
	mock_free_tokens(tok);

	tok = mock_token_new(TOK_WORD, "$USER", 0, 0);
	test_result("expand_one_token word success", expand_one_token(tok, &shell) == 1);
	test_result("expand_one_token word expanded", same_str(tok->value, "ykonka"));
	mock_free_tokens(tok);

	tok = mock_token_new(TOK_WORD, "$USER", 1, 0);
	test_result("expand_one_token single quoted", expand_one_token(tok, &shell) == 1);
	test_result("expand_one_token single quoted unchanged",
		same_str(tok->value, "$USER"));
	mock_free_tokens(tok);

	mock_free_env(&shell.env);
}

static void	test_expand_tokens(void)
{
	t_shell	shell;
	t_token	*a;
	t_token	*b;
	t_token	*c;

	memset(&shell, 0, sizeof(shell));
	mock_env_add_back(&shell.env, mock_env_new("USER", "ykonka"));
	mock_env_add_back(&shell.env, mock_env_new("HOME", "/tmp"));
	shell.last_status = 21;

	test_result("expand_tokens NULL list", expand_tokens(NULL, &shell) == 1);

	a = mock_token_new(TOK_WORD, "$USER", 0, 0);
	b = mock_token_new(TOK_WORD, "$?", 0, 0);
	c = mock_token_new(TOK_PIPE, "|", 0, 0);
	a->next = b;
	b->next = c;

	test_result("expand_tokens list success", expand_tokens(a, &shell) == 1);
	test_result("expand_tokens token1", same_str(a->value, "ykonka"));
	test_result("expand_tokens token2", same_str(b->value, "21"));
	test_result("expand_tokens token3 unchanged", same_str(c->value, "|"));

	mock_free_tokens(a);
	mock_free_env(&shell.env);
}

int	main(void)
{
	printf("=== expand module unit tests ===\n\n");
	test_is_valid_var_start();
	test_is_valid_var_char();
	test_var_name_len();
	test_extract_var_name();
	test_str_append_char();
	test_str_append_str();
	test_expand_exit_status();
	test_expand_word_basic();
	test_expand_word_edge_cases();
	test_expand_one_token();
	test_expand_tokens();
	printf("\nPassed %d/%d tests\n", g_pass, g_total);
	return (0);
}


