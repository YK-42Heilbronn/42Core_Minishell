/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 16:02:04 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:30:39 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

/*
Behavior
is_valid_var_start() returns 1 for alphabetic or _, else 0.
is_valid_var_char() returns 1 for alnum or _, else 0.
var_name_len() counts how long the variable name is.
extract_var_name() duplicates only the variable part.
str_append_char() and str_append_str() build the final expanded string safely.
*/
int	is_valid_var_start(char c)
{
	return (ft_isalpha((unsigned char)c) || c == '_');
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}

int	var_name_len(const char *s)
{
	int	len;

	if (!s || !is_valid_var_char(s[0]))
		return (0);
	len = 0;
	while (s[len] && is_valid_var_char(s[len]))
		len++;
	return (len);
}

char	*str_append_char(char *src, char c)
{
	char	*appended_str;
	size_t	len;

	if (!src)
	{
		appended_str = malloc(2);
		if (!appended_str)
			return (NULL);
		appended_str[0] = c;
		appended_str[1] = '\0';
		return (appended_str);
	}
	len = ft_strlen(src);
	appended_str = malloc(len + 2);
	if (!appended_str)
		return (NULL);
	ft_memcpy(appended_str, src, len);
	appended_str[len] = c;
	appended_str[len + 1] = '\0';
	free(src);
	src = NULL;
	return (appended_str);
}

char	*str_append_str(char *src, const char *suffix)
{
	char	*new_str;

	if (!suffix)
		suffix = "";
	if (!src)
		return (ft_strdup(suffix));
	new_str = ft_strjoin(src, suffix);
	if (!new_str)
		return (NULL);
	free(src);
	src = NULL;
	return (new_str);
}
