/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 16:02:27 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:39:36 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

char	*extract_var_name(const char *s, int len)
{
	char	*dest;

	if (!s || len <= 0)
		return (NULL);
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s, len);
	dest[len] = '\0';
	return (dest);
}

/*
Behavior
Convert shell->last_status to a newly allocated string with ft_itoa.
Return that string.
Return NULL on allocation failure
*/
char	*expand_exit_status(t_shell *shell)
{
	if (!shell)
		return (NULL);
	return (ft_itoa(shell->last_status));
}

int	append_char_at(char **result, const char *word, int *i)
{
	*result = str_append_char(*result, word[*i]);
	if (!*result)
		return (0);
	(*i)++;
	return (1);
}

int	append_exit_status(char **result, t_shell *shell, int *i)
{
	char	*status;

	status = expand_exit_status(shell);
	if (!status)
		return (*result = NULL, 0);
	*result = str_append_str(*result, status);
	free(status);
	status = NULL;
	if (!*result)
		return (0);
	*i += 2;
	return (1);
}

int	append_env_var(char **result, const char *word, t_shell *shell, int *i)
{
	char	*name;
	char	*value;
	int		len;

	len = var_name_len(&word[*i + 1]);
	if (len <= 0)
		return (append_char_at(result, word, i));
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
