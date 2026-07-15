/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:43:20 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:48:00 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	builtin_env(t_shell *shell)
{
	t_env	*cur;

	cur = shell->env;
	while (cur)
	{
		if (cur->value)
		{
			write_str(1, cur->key);
			write_str(1, "=");
			write_str(1, cur->value);
			write_str(1, "\n");
		}
		cur = cur->next;
	}
	return (0);
}

// int	builtin_env(t_shell *shell)
// {
// 	t_env	*env;

// 	env = shell->env;
// 	while (env)
// 	{
// 		if (env->value)
// 		{
// 			write(1, env->key, ft_strlen(env->key));
// 			write(1, "=", 1);
// 			write(1, env->value, ft_strlen(env->value));
// 			write(1, "\n", 1);
// 		}
// 		env = env->next;
// 	}
// 	return (0);
// }
