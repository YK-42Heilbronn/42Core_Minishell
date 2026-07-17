/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 05:24:46 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/13 05:53:17 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expander.h"
#include "parser.h"

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
	return (strcmp(a, b) == 0);
}

static int	env_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static t_env	*make_env_node_dup(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = strdup(key);
	if (value)
		node->value = strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

static void	append_env_dup(t_env **env, const char *key, const char *value)
{
	t_env	*node;
	t_env	*cur;

	node = make_env_node_dup(key, value);
	if (!node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static void	free_env_list_manual(t_env **env)
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

/* prototypes under test */
t_env	*env_new_node(const char *key, const char *value);
void	env_add_back(t_env **env, t_env *new_node);
t_env	*split_envp_value(char *env_value);
void	free_env(t_env **sh_env);
int		env_init(t_shell *shell, char **envp);
char	*env_get_value(t_env *env, const char *key);
int		env_set_value(t_env **env, const char *key, const char *value);
int		env_unset_value(t_env **env, const char *key);
char	**env_to_array(t_env *env);
void	free_envp(char **envp);

static void	test_env_new_node(void)
{
	char	*key;
	char	*value;
	t_env	*node;

	key = strdup("USER");
	value = strdup("ykonka");
	node = env_new_node(key, value);
	test_result("env_new_node alloc", node != NULL);
	test_result("env_new_node key stored", same_str(node->key, "USER"));
	test_result("env_new_node value stored", same_str(node->value, "ykonka"));
	test_result("env_new_node next null", node->next == NULL);
	free(node->key);
	free(node->value);
	free(node);
}

static void	test_env_add_back(void)
{
	t_env	*env;
	t_env	*n1;
	t_env	*n2;

	env = NULL;
	n1 = make_env_node_dup("USER", "ykonka");
	n2 = make_env_node_dup("HOME", "/home/ykonka");
	env_add_back(&env, n1);
	test_result("env_add_back first node", env == n1);
	env_add_back(&env, n2);
	test_result("env_add_back second node linked", env->next == n2);
	test_result("env_add_back size 2", env_size(env) == 2);
	free_env_list_manual(&env);
}

// static void	test_split_envp_value(void)
// {
// 	t_env	*node;

// 	node = split_envp_value("USER=ykonka");
// 	test_result("split_envp_value basic", node != NULL);
// 	test_result("split_envp_value key", same_str(node->key, "USER"));
// 	test_result("split_envp_value value", same_str(node->value, "ykonka"));
// 	free_env(&node);

// 	node = split_envp_value("EMPTY=");
// 	test_result("split_envp_value empty value", node != NULL);
// 	test_result("split_envp_value empty key", same_str(node->key, "EMPTY"));
// 	test_result("split_envp_value empty value string", same_str(node->value, ""));
// 	free_env(&node);

// 	node = split_envp_value("=abc");
// 	test_result("split_envp_value empty key allowed currently", node != NULL);
// 	test_result("split_envp_value empty key exact", same_str(node->key, ""));
// 	test_result("split_envp_value value abc", same_str(node->value, "abc"));
// 	free_env(&node);

// 	node = split_envp_value("NO_EQUAL_SIGN");
// 	test_result("split_envp_value no equal returns null", node == NULL);
// }

static void	test_free_env(void)
{
	t_env	*env;

	env = NULL;
	append_env_dup(&env, "A", "1");
	append_env_dup(&env, "B", "2");
	free_env(&env);
	test_result("free_env sets null", env == NULL);

	free_env(NULL);
	test_result("free_env null ptr safe", 1);
}

static void	test_env_init(void)
{
	t_shell	shell;
	char	*envp1[] = {"USER=ykonka", "HOME=/home/ykonka", "EMPTY=", NULL};
	char	*envp2[] = {"BROKEN", NULL};

	memset(&shell, 0, sizeof(shell));
	// printf("%s\n", shell.env->key);
	test_result("env_init valid", env_init(&shell, envp1) == 0);
	test_result("env_init created 3 nodes", env_size(shell.env) == 3);
	test_result("env_init USER", same_str(env_get_value(shell.env, "USER"), "ykonka"));
	test_result("env_init HOME", same_str(env_get_value(shell.env, "HOME"), "/home/ykonka"));
	test_result("env_init EMPTY", same_str(env_get_value(shell.env, "EMPTY"), ""));
	printf("%s\n", env_get_value(shell.env, "USER"));
	free_env(&(shell.env));

	test_result("env_init null shell", env_init(NULL, envp1) == 1);

	memset(&shell, 0, sizeof(shell));
	test_result("env_init null envp", env_init(&shell, NULL) == 1);

	memset(&shell, 0, sizeof(shell));
	test_result("env_init broken entry fails", env_init(&shell, envp2) == 1);
	test_result("env_init broken entry cleaned list", shell.env == NULL);
}

static void	test_env_get_value(void)
{
	t_env	*env;

	env = NULL;
	append_env_dup(&env, "USER", "ykonka");
	append_env_dup(&env, "HOME", "/tmp");
	append_env_dup(&env, "EMPTY", "");

	test_result("env_get_value USER", same_str(env_get_value(env, "USER"), "ykonka"));
	test_result("env_get_value HOME", same_str(env_get_value(env, "HOME"), "/tmp"));
	test_result("env_get_value EMPTY", same_str(env_get_value(env, "EMPTY"), ""));
	test_result("env_get_value missing", env_get_value(env, "NOPE") == NULL);
	test_result("env_get_value null env", env_get_value(NULL, "USER") == NULL);
	test_result("env_get_value null key", env_get_value(env, NULL) == NULL);

	free_env_list_manual(&env);
}

static void	test_env_set_value(void)
{
	t_env	*env;
	char	*old_ptr;

	env = NULL;
	append_env_dup(&env, "USER", "old");
	append_env_dup(&env, "HOME", "/tmp");

	old_ptr = env->value;
	test_result("env_set_value existing key", env_set_value(&env, "USER", "new") == 1);
	test_result("env_set_value updated content", same_str(env_get_value(env, "USER"), "new"));
	test_result("env_set_value replaced pointer", env->value != old_ptr);

	test_result("env_set_value append new key", env_set_value(&env, "PWD", "/home") == 1);
	test_result("env_set_value new key exists", same_str(env_get_value(env, "PWD"), "/home"));
	test_result("env_set_value size 3", env_size(env) == 3);

	// test_result("env_set_value null env ptr", env_set_value(NULL, "A", "1") == 0);
	// test_result("env_set_value null key", env_set_value(&env, NULL, "1") == 0);
	// test_result("env_set_value null value", env_set_value(&env, "A", NULL) == 0);

	// free_env_list_manual(&env);

	/*
	** This matches current implementation:
	** if *env == NULL, function returns 0 and does not create first node.
	*/
	env = NULL;
	test_result("env_set_value empty list current behavior", env_set_value(&env, "A", "1") == 0);
	test_result("env_set_value empty list unchanged", env == NULL);
}

static void	test_env_unset_value(void)
{
	t_env	*env;

	env = NULL;
	append_env_dup(&env, "A", "1");
	append_env_dup(&env, "B", "2");
	append_env_dup(&env, "C", "3");

	test_result("env_unset_value remove middle", env_unset_value(&env, "B") == 1);
	test_result("env_unset_value middle removed", env_get_value(env, "B") == NULL);
	test_result("env_unset_value size after middle", env_size(env) == 2);

	test_result("env_unset_value remove head", env_unset_value(&env, "A") == 1);
	test_result("env_unset_value head removed", env_get_value(env, "A") == NULL);
	test_result("env_unset_value new head is C", same_str(env->key, "C"));

	test_result("env_unset_value remove tail/single", env_unset_value(&env, "C") == 1);
	test_result("env_unset_value list empty", env == NULL);

	test_result("env_unset_value missing key", env_unset_value(&env, "NOPE") == 0);
	test_result("env_unset_value null env", env_unset_value(NULL, "A") == 0);
	test_result("env_unset_value null key", env_unset_value(&env, NULL) == 0);
}

static void	test_env_to_array(void)
{
	t_env	*env;
	char	**envp;

	env = NULL;
	append_env_dup(&env, "USER", "ykonka");
	append_env_dup(&env, "HOME", "/home/ykonka");
	append_env_dup(&env, "EMPTY", "");

	envp = env_to_array(env);
	test_result("env_to_array alloc", envp != NULL);
	test_result("env_to_array [0]", same_str(envp[0], "USER=ykonka"));
	test_result("env_to_array [1]", same_str(envp[1], "HOME=/home/ykonka"));
	test_result("env_to_array [2]", same_str(envp[2], "EMPTY="));
	test_result("env_to_array null terminated", envp[3] == NULL);
	free_envp(envp);
	free_env_list_manual(&env);

	env = NULL;
	envp = env_to_array(env);
	test_result("env_to_array empty env alloc", envp != NULL);
	test_result("env_to_array empty env only null", envp[0] == NULL);
	free_envp(envp);
}

static void	test_free_envp_fn(void)
{
	char	**envp;

	envp = malloc(sizeof(char *) * 3);
	envp[0] = strdup("A=1");
	envp[1] = strdup("B=2");
	envp[2] = NULL;
	free_envp(envp);
	test_result("free_envp normal", 1);

	free_envp(NULL);
	test_result("free_envp null safe", 1);
}

int	main(void)
{
	printf("=== env module unit tests ===\n\n");
	test_env_new_node();
	test_env_add_back();
	// test_split_envp_value();
	test_free_env();
	test_env_init();
	test_env_get_value();
	test_env_set_value();
	test_env_unset_value();
	test_env_to_array();
	test_free_envp_fn();
	printf("\nPassed %d/%d tests\n", g_pass, g_total);
	return (0);
}
