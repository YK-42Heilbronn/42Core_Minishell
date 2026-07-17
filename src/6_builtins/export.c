/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:26:11 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:02:33 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

static int	export_one(char *arg, t_shell *shell)
{
	char	*eq;
	char	*key;

	eq = ft_strchr(arg, '=');
	if (eq)
		key = ft_substr(arg, 0, eq - arg);
	else
		key = ft_strdup(arg);
	if (!key || !is_valid_identifier(key))
	{
		write_str(2, "minishell: export: `");
		write_str(2, arg);
		write_str(2, "': not a valid identifier\n");
		return (free(key), -1);
	}
	if (eq)
		env_set_value(&shell->env, key, eq + 1);
	else if (!env_get_node(shell->env, key))
		env_set_value(&shell->env, key, "");
	return (free(key), 0);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;
	int	ret;

	if (!argv[1])
		return (print_export_list(shell), 0);
	ret = 0;
	i = 1;
	while (argv[i])
	{
		if (export_one(argv[i], shell) == -1)
			ret = 1;
		i++;
	}
	return (ret);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!is_valid_var_start(str[0]))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!is_valid_var_char(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* NOTE: real bash prints this list alphabetically sorted; not required
 * by the subject text, kept simple here (insertion order). Easy to add
 * a small sort pass later if you want the closer bash look. */

void	print_export_list(t_shell *shell)
{
	t_env	*cur;

	cur = shell->env;
	while (cur)
	{
		printf("declare -x %s=\"%s\"\n", cur->key, cur->value);
		cur = cur->next;
	}
}
