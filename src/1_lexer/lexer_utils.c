/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:16:29 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:02:37 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	append_token_dup(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_tok;
	char	*copy;

	copy = ft_strdup(value);
	if (!copy)
		return (0);
	new_tok = new_token(type, copy);
	if (!new_tok)
	{
		free(copy);
		return (0);
	}
	add_token(tokens, new_tok);
	return (1);
}

int	append_token_owned(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_tok;

	if (!value)
		return (0);
	new_tok = new_token(type, value);
	if (!new_tok)
	{
		free(value);
		return (0);
	}
	update_quotes_state(new_tok);
	add_token(tokens, new_tok);
	return (1);
}
