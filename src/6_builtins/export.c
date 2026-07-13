/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:26:11 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 15:21:39 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "minishell.h"
#include "parser.h"

// TODO: why export_one?

// Short explanation:
// export with no argc: print env in declare -x style or simple KEY=VALUE
// export KEY=VALUE: add/update enviroment
// ignore invalid identifiers

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!(ft_isalpha(s[0]) || s[0] == '-'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	export_error_print(const char *arg)
{
	write(2, "minishell: export: ", 20);
	write(2, arg, ft_strlen(arg));
	write(2, ": not a valid identifier\n", 26);
	return (1);
}

int	export_one(t_shell *shell, const char *arg)
{
	char	*eq;
	char	*key;
	char	*value;

	if (is_valid_identifier(arg))
		return (export_error_print(arg));
	eq = ft_strchr(arg, '=');
	if (!eq)
		return (env_set_value(&shell->env, arg, env_get_value(shell->env,
					arg)));
	key = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (1);
	}
	env_set_value(&shell->env, key, value);
	free(key);
	free(value);
	return (0);
}

int	export_env_print(t_shell *shell)
{
	t_env	*env;

	env = shell->env;
	while (env)
	{
		write(1, "declare -x ", 11);
		write(1, env->key, ft_strlen(env->key));
		if (env->value)
		{
			write(1, "=\"", 2);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 2);
		env = env->next;
	}
	return (0);
}

int	builtin_export(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	status;

	if (!shell || !cmd)
		return (1);
	if (!cmd->argv[1])
		return (export_env_print(shell));
	i = 1;
	status = 0;
	while (cmd->argv[i])
	{
		if (export_one(shell, cmd->argv[i]))
			status = 1;
		i++;
	}
	return (status);
}
