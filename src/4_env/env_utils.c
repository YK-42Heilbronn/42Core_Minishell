/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 16:01:29 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/16 19:46:27 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

t_env	*env_get_node(t_env *env, char *key)
{
	t_env	*env_node;

	if (!env || !key)
		return (NULL);
	env_node = env;
	while (env_node)
	{
		if (env_node->key && ft_strlen(env_node->key) == ft_strlen(key)
			&& ft_strncmp(env_node->key, key, ft_strlen(key) + 1) == 0)
			return (env_node);
		env_node = env_node->next;
	}
	return (NULL);
}

/*
Behavior
Walk through the t_env linked list.
Compare env->key with key.
Return env->value if found.
Return NULL if not found.
*/
char	*env_get_value(t_env *env, char *key)
{
	t_env	*env_node;

	if (!env || !key)
		return (NULL);
	env_node = env;
	while (env_node)
	{
		if (env_node->key && ft_strlen(env_node->key) == ft_strlen(key)
			&& ft_strncmp(env_node->key, key, ft_strlen(key) + 1) == 0)
			return (env_node->value);
		env_node = env_node->next;
	}
	return (NULL);
}

/*
Behavior
search the shell env list for the given key
if the key already exists, replace its value
if the key does not exist, create a new t_env node and add it to the list
*/
int	env_set_value(t_env **env, char *key, char *value)
{
	t_env	*env_node;
	t_env	*new_env;
	char	*value_cpy;

	env_node = *env;
	if (!env || !*env || !key || !value)
		return (0);
	while (env_node)
	{
		if (env_node->key && ft_strlen(env_node->key) == ft_strlen(key)
			&& ft_strncmp(env_node->key, key, ft_strlen(key) + 1) == 0)
		{
			value_cpy = ft_strdup(value);
			if (value_cpy == NULL)
				return (0);
			free(env_node->value);
			env_node->value = value_cpy;
			return (1);
		}
		env_node = env_node->next;
	}
	new_env = env_new_node(key, value);
	if (new_env == NULL)
		return (0);
	env_add_back(env, new_env);
	return (1);
}

/*
Behavior
search the env list for the given key
unlink that node from the linked list
free its key, value, and the node itself
*/
// int		env_unset_value(t_env **env, const char *key)
// {
// 	t_env *env_node;
// 	t_env *prev_node;

// 	if (!env || !*env || !key)
// 		return (0);
// 	env_node = *env;
// 	prev_node = NULL;
// 	while (env_node)
// 	{
// 		if (env_node->key
// 			&& ft_strlen(env_node->key) == ft_strlen(key)
// 			&& ft_strncmp(env_node->key, key, ft_strlen(key)+1) == 0)
// 		{
// 			prev_node->next = env_node->next;
// 			free(env_node->key);
// 			free(env_node->value);
// 			free(env_node);
// 			return(1);
// 		}
// 		prev_node = env_node;
// 		env_node = env_node->next;
// 	}
// 	return (0);
// }

int	env_unset_value(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	if (!env || !*env || !key)
		return (0);
	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (cur->key && ft_strlen(cur->key) == ft_strlen(key)
			&& ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev == NULL)
				*env = cur->next;
			else
				prev->next = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
