/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_expander_tests.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 18:40:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/12 21:49:23 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expander.h"
#include "parser.h"

/* prototypes missing from header for now */
t_env	*env_new_node(const char *key, const char *value);
void	env_add_back(t_env **env, t_env *new_node);
t_env	*split_envp_value(char *env_value);
void	free_env(t_env **sh_env);
int		env_init(t_shell *shell, char **envp);
int		env_set_value(t_env **env, const char *key, const char *value);
int		env_unset_value(t_env **env, const char *key);
char	**env_to_array(t_env *env);
void	free_envp(char **envp);

static void	print_result(char *label, int ok)
{
	if (ok)
		printf("[OK] %s\n", label);
	else
		printf("[FAIL] %s\n", label);
}

static int	str_equal(char *a, char *b)
{
	if (!a && !b)
		return (1);
	if (!a || !b)
		return (0);
	return (strcmp(a, b) == 0);
}

static void	print_env_array(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		printf("envp[%d] = %s\n", i, envp[i]);
		i++;
	}
}

static void	test_env_init(t_shell *shell)
{
	char	*fake_envp[] = {
		"USER=ykonka",
		"HOME=/home/ykonka",
		"PATH=/usr/bin:/bin",
		NULL
	};

	printf("\n--- test_env_init ---\n");

	/*
	** current env_add_back needs first node already present
	** so we seed USER manually, then init the rest
	*/
	shell->env = split_envp_value("USER=ykonka");
	env_init(shell, &fake_envp[1]);

	print_result("env_get_value USER",
		str_equal(env_get_value(shell->env, "USER"), "ykonka"));
	print_result("env_get_value HOME",
		str_equal(env_get_value(shell->env, "HOME"), "/home/ykonka"));
	print_result("env_get_value PATH",
		str_equal(env_get_value(shell->env, "PATH"), "/usr/bin:/bin"));
}

static void	test_env_set_value(t_shell *shell)
{
	printf("\n--- test_env_set_value ---\n");

	print_result("update USER",
		env_set_value(&shell->env, "USER", "alice"));
	print_result("USER updated",
		str_equal(env_get_value(shell->env, "USER"), "alice"));

	print_result("add TEST",
		env_set_value(&shell->env, "TEST", "42"));
	print_result("TEST added",
		str_equal(env_get_value(shell->env, "TEST"), "42"));
}

static void	test_env_unset_value(t_shell *shell)
{
	printf("\n--- test_env_unset_value ---\n");

	/*
	** remove TEST only, not head node, because current unset
	** has a bug on head removal
	*/
	print_result("unset TEST",
		env_unset_value(&(shell->env), "TEST"));
	print_result("TEST removed",
		env_get_value(shell->env, "TEST") == NULL);
}

static void	test_env_to_array(t_shell *shell)
{
	char	**envp;

	printf("\n--- test_env_to_array ---\n");
	envp = env_to_array(shell->env);
	print_result("env_to_array not NULL", envp != NULL);
	if (envp)
		print_env_array(envp);
	free_envp(envp);
}

static void	test_expand_word(t_shell *shell)
{
	char	*res;

	printf("\n--- test_expand_word ---\n");

	shell->last_status = 127;

	res = expand_word("$USER", shell, 0, 0);
	print_result("expand $USER", str_equal(res, "alice"));
	free(res);

	res = expand_word("hello $HOME", shell, 0, 0);
	print_result("expand hello $HOME",
		str_equal(res, "hello /home/ykonka"));
	free(res);

	res = expand_word("$?", shell, 0, 0);
	print_result("expand $?", str_equal(res, "127"));
	free(res);

	res = expand_word("$NOT_FOUND", shell, 0, 0);
	print_result("expand missing var", str_equal(res, ""));
	free(res);

	res = expand_word("$USER", shell, 1, 0);
	print_result("single quote blocks expansion", str_equal(res, "$USER"));
	free(res);
}

int	main(void)
{
	t_shell	shell;

	memset(&shell, 0, sizeof(t_shell));

	test_env_init(&shell);
	test_env_set_value(&shell);
	test_env_unset_value(&shell);
	test_env_to_array(&shell);
	test_expand_word(&shell);

	free_env(&shell.env);
	return (0);
}


