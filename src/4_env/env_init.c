/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 12:27:07 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/12 21:01:29 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

t_env	*env_new_node(const char *key, const char *value)
{
	t_env	*new_env;

	new_env = (t_env *)malloc(sizeof(t_env) * 1);
	if (new_env == NULL)
		return (NULL);
	new_env->key = (char*)key;
	new_env->value = (char*)value;
	new_env->next = NULL;
	return (new_env);
}

// void	env_add_back(t_env **env, t_env *new_node)
// {
// 	t_env	*next_env;

// 	next_env = *env;
// 	while (next_env->next)
// 	{
// 		next_env = next_env->next;
// 	}
// 	next_env->next = new_node;
// }

void	env_add_back(t_env **env, t_env *new_node)
{
	t_env	*cur;

	if (!env || !new_node)
		return ;
	if (*env == NULL)
	{
		*env = new_node;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}

t_env	*split_envp_value(char *env_value)
{
	t_env	*new_env;
	char	*trimed_value;
	char	*key;
	char	*value;

	trimed_value = ft_strchr(env_value, '=');
	if (!trimed_value)
		return (NULL);
	key = ft_substr(env_value, 0, trimed_value-env_value);
	if (!key)
		return (NULL);
	value = ft_strdup(trimed_value+1);
	if (!value)
		return (free(key), NULL);
	new_env = env_new_node(key, value);
	if (!new_env)
		return (free(key), free(value), NULL);
	return (new_env);
}

void free_env(t_env **sh_env)
{
	t_env *env;
	t_env *next;

	if (!sh_env || !*sh_env)
		return;
	env = *sh_env;
	while (env)
	{
		next = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next;
	}
	*sh_env = NULL;
	// free(sh_env);
}

// env copy from envp into shell->env
int	env_init(t_shell *shell, char **envp)
{
	t_env	*new_env;

	if (!shell || !envp)
		return (1);
	while (*envp)
	{
		new_env = split_envp_value(*envp);
		if (!new_env)
			return (free_env(&(shell->env)), 1);
		env_add_back(&(shell->env), new_env);
		envp++;
	}
	return (0);
}
