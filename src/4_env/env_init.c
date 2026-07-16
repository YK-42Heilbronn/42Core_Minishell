/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 12:27:07 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/16 21:33:16 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

// t_env	*env_new_node(const char *key, const char *value)
// {
// 	t_env	*new_env;

// 	new_env = (t_env *)malloc(sizeof(t_env) * 1);
// 	if (new_env == NULL)
// 		return (NULL);
// 	new_env->key = (char*)key;
// 	new_env->value = (char*)value;
// 	new_env->next = NULL;
// 	return (new_env);
// }

// t_env	*env_new_node(const char *key, const char *value)
// {
// 	t_env	*new_env;

// 	new_env = malloc(sizeof(t_env));
// 	if (!new_env)
// 		return (NULL);
// 	new_env->key = ft_strdup(key);
// 	new_env->value = ft_strdup(value ? value : "");
// 	if (!new_env->key || !new_env->value)
// 	{
// 		free(new_env->key);
// 		free(new_env->value);
// 		free(new_env);
// 		return (NULL);
// 	}
// 	new_env->next = NULL;
// 	return (new_env);
// }

t_env	*env_new_node(char *key, char *value)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
	{
		free(key);
		return (free(value), NULL);
	}
	new_env->key = key;
	new_env->value = value;
	new_env->next = NULL;
	return (new_env);
}

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

void	free_env(t_env **sh_env)
{
	t_env	*env;
	t_env	*next;

	if (!sh_env || !*sh_env)
		return ;
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
}

// t_env	*split_envp_value(char *env_value)
// {
// 	t_env	*new_env;
// 	char	*trimed_value;
// 	char	*key;
// 	char	*value;

// 	trimed_value = ft_strchr(env_value, '=');
// 	if (!trimed_value)
// 		return (NULL);
// 	key = ft_substr(env_value, 0, trimed_value - env_value);
// 	if (!key)
// 		return (NULL);
// 	if (*(trimed_value + 1) == '\0')
// 		value = ft_strdup("");
// 	else
// 		value = ft_strdup(trimed_value + 1);
// 	if (!value)
// 		return (free(key), key = NULL, NULL);
// 	new_env = env_new_node(key, value);
// 	free(key);
// 	key = NULL;
// 	free(value);
// 	value = NULL;
// 	if (!new_env)
// 		return (NULL);
// 	return (new_env);
// }

t_env   *split_envp_value(char *env_value)
{
    t_env   *new_env;
    char    *trimed_value;
    char    *key;
    char    *value;

    trimed_value = ft_strchr(env_value, '=');
    if (!trimed_value)
        return (NULL);
    key = ft_substr(env_value, 0, trimed_value - env_value);
    if (!key)
        return (NULL);
    if (*(trimed_value + 1) == '\0')
        value = ft_strdup("");
    else
        value = ft_strdup(trimed_value + 1);
    if (!value)
        return (free(key), NULL);
    new_env = env_new_node(key, value);
    if (!new_env)
        return (NULL);
    return (new_env);
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
