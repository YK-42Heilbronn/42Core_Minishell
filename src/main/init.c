/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:27 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 19:48:46 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "minishell.h"
#include "parser.h"

// void	init_shell(t_shell *shell, char **envp)
// {
// 	ft_memset(shell, 0, sizeof(t_shell));
// 	shell->last_status = 0;
// 	shell->line = NULL;
// 	shell->tokens = NULL;
// 	shell->cmds = NULL;
// 	shell->env = dup_env(envp);
// 	if (!shell->env)
// 	{
// 		shell->last_status = 1;
// 		return ;
// 	}
// 	lvl_up(shell);
// }

void	init_shell(t_shell *shell, char **envp)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->last_status = 0;
	shell->line = NULL;
	shell->tokens = NULL;
	shell->cmds = NULL;
	shell->env = NULL;
	env_init(shell, envp);
	if (!shell->env)
	{
		shell->last_status = 1;
		return ;
	}
	lvl_up(shell);
}

t_env	*dup_env(char **envp)
{
	t_env	*env;
	t_env	*node;
	int		i;

	env = NULL;
	if (!envp || !envp[0])
		return (NULL);
	i = 0;
	while (envp[i])
	{
		node = split_envp_value(envp[i]);
		if (!node)
		{
			free_env(&env);
			return (NULL);
		}
		env_add_back(&env, node);
		i++;
	}
	return (env);
}

void	lvl_up(t_shell *shell)
{
	t_env	*node;
	int		level;
	char	*new_value;

	node = env_get_node(shell->env, "SHLVL");
	if (!node)
	{
		node = env_new_node("SHLVL", "1");
		if (!node)
			return ;
		env_add_back(&shell->env, node);
		return ;
	}
	level = ft_atoi(node->value);
	level++;
	new_value = ft_itoa(level);
	if (!new_value)
		return ;
	free(node->value);
	node->value = new_value;
}
