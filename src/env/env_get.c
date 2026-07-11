/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 16:01:29 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/11 16:22:49 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

/*
Behavior
Walk through the t_env linked list.
Compare env->key with key.
Return env->value if found.
Return NULL if not found.
*/
char	*env_get_value(t_env *env, const char *key)
{
	t_env *env_node;

	if (env == NULL || key == NULL)
		return (NULL);
	env_node = env;
	while (env_node)
	{
		if(env_node->key
		   && ft_strlen(env_node->key) == ft_strlen(key)
		   && ft_strncmp(env_node->key, key, ft_strlen(key)) == 0)
		   return (env_node->value);
		env_node = env_node->next;
	}
	return (NULL);
}

// char	*env_get_value(t_env *env, const char *key)
// {
// 	t_env	*env_node;

// 	if (!env || !key)
// 		return (NULL);
// 	env_node = env;
// 	while (env_node)
// 	{
// 		if (env_node->key
// 			&& ft_strlen(env_node->key) == ft_strlen(key)
// 			&& ft_strncmp(env_node->key, key, ft_strlen(key)) == 0)
// 			return (env_node->value);
// 		env_node = env_node->next;
// 	}
// 	return (NULL);
// }
