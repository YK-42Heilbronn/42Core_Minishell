/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:56:07 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/11 17:56:31 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static int	append_char_at(char **result, const char *word, int *i)
{
	*result = str_append_char(*result, word[*i]);
	if (!*result)
		return (0);
	(*i)++;
	return (1);
}

static int	append_exit_status(char **result, t_shell *shell, int *i)
{
	char	*status;

	status = expand_exit_status(shell);
	if (!status)
		return (0);
	*result = str_append_str(*result, status);
	free(status);
	if (!*result)
		return (0);
	*i += 2;
	return (1);
}

static int	append_env_var(char **result, const char *word, t_shell *shell, int *i)
{
	char	*name;
	char	*value;
	int		len;

	len = var_name_len(&word[*i + 1]);
	name = extract_var_name(&word[*i + 1], len);
	if (!name)
		return (0);
	value = env_get_value(shell->env, name);
	free(name);
	if (value)
		*result = str_append_str(*result, value);
	else
		*result = str_append_str(*result, "");
	if (!*result)
		return (0);
	*i += len + 1;
	return (1);
}

static int	handle_dollar(char **result, const char *word, t_shell *shell, int *i)
{
	if (word[*i + 1] == '?')
		return (append_exit_status(result, shell, i));
	if (is_valid_var_start(word[*i + 1]))
		return (append_env_var(result, word, shell, i));
	return (append_char_at(result, word, i));
}

char	*expand_word(const char *word, t_shell *shell, int sq, int dq)
{
	char	*result;
	int		i;

	(void)dq;
	if (!word)
		return (NULL);
	if (sq)
		return (ft_strdup(word));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (word[i] == '$' && !handle_dollar(&result, word, shell, &i))
			return (NULL);
		else if (word[i] != '$' && !append_char_at(&result, word, &i))
			return (NULL);
	}
	return (result);
}
