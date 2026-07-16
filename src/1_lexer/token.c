/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:14:01 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/16 03:48:23 by ykonka           ###   ########.fr       */
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
	node->quoted = 0;
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

void	free_tokens(t_token **list)
{
	t_token	*next;

	while (*list)
	{
		next = (*list)->next;
		free((*list)->value);
		free(*list);
		*list = next;
	}
	*list = NULL;
}

void update_quotes_state(t_token *token)
{
	char *v_first;
	char *v_last;

	v_first = token->value;
	v_last = token->value;
	while (*v_last)
		v_last++;
	v_last--;
	if (*v_first == '\'' && *v_last == '\'')
		token->quoted = 1;
	else if (*v_first == '"' && *v_last == '"')
		token->quoted = 2;
}
