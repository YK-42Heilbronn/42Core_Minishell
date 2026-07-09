/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:01 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/09 16:36:12 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// single = 0 // not_declared
// double = 0 // not_declared
t_token	*new_token(t_token_type type, char *value)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = value;
	node->quoted_single = 0;
	node->quoted_double = 0;
	node->next = NULL;
	return (node);
}

void	add_token(t_token **tokens, t_token *new_tok)
{
	t_token	*tmp;

	if (!tokens || !new_tok)
		return ;
	if (!*tokens)
	{
		*tokens = new_tok;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_tok;
}
