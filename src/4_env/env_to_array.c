/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 14:50:10 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:47:52 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

void	free_envp(char **envp)
{
	char	**temp;

	if (!envp)
		return ;
	temp = envp;
	while (*envp)
	{
		free(*envp);
		envp++;
	}
	free(temp);
}

static int	count_env_nodes(t_env *env)
{
	int		size;
	t_env	*next_env;

	next_env = env;
	size = 0;
	while (next_env)
	{
		next_env = next_env->next;
		size++;
	}
	return (size);
}

static char	*concatenate_envp_value(char *key, char *value)
{
	char	*k_new_value;
	char	*v_new_value;

	k_new_value = ft_strjoin(key, "=");
	if (!k_new_value)
		return (NULL);
	if (!value)
		v_new_value = ft_strjoin(k_new_value, "");
	else
		v_new_value = ft_strjoin(k_new_value, value);
	free(k_new_value);
	return (v_new_value);
}

char	**env_to_array(t_env *env)
{
	int		size;
	char	**envp;
	char	*new_value;
	int		ind;

	size = count_env_nodes(env);
	ind = 0;
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (envp == NULL)
		return (NULL);
	while (env)
	{
		new_value = concatenate_envp_value(env->key, env->value);
		envp[ind++] = new_value;
		env = env->next;
	}
	envp[ind] = NULL;
	return (envp);
}
