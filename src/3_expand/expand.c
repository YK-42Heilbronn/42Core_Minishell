/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 16:02:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/13 19:18:04 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

/*
Behavior::
If token == NULL, return 0 or 1 depending on your style; I recommend 0 only for real failure and 1 for no-op success.
If token->type != TOK_WORD, do nothing and return 1.
If token is TOK_WORD, call:
C::
new_value = expand_word(token->value, shell,
	token->quoted_single, token->quoted_double);
Free old token->value
Replace with new_value
Return 1 on success, 0 on failure
*/
// int	expand_one_token(t_token *token, t_shell *shell)
// {
// 	char *new_tok_value;

// 	if (token == NULL)
// 		return (1);
// 	if (token->type != TOK_WORD)
// 		return (1); // success
// 	else
// 	{
// 		new_tok_value = expand_word(token->value, shell, token->quoted_single, token->quoted_double);
// 		if (new_tok_value == NULL)
// 			return (0); // failure
// 		free(token->value);
// 		token->value = new_tok_value;
// 		return (1); // success
// 	}
// }

int expand_one_token(t_token *token, t_shell *shell)
{
    char *new_tok_value;

    if (token == NULL)
        return (1);
    if (token->type != TOK_WORD)
        return (1);
    if (token->value == NULL)
    {
        token->value = ft_strdup("");
        if (!token->value)
            return (0);
        return (1);
    }
    new_tok_value = expand_word(token->value, shell,
            token->quoted_single, token->quoted_double);
    if (new_tok_value == NULL)
        return (0);
    free(token->value);
    token->value = new_tok_value;
    return (1);
}

/*
Behavior
Walk the whole linked list.
Call expand_one_token() on each node.
Stop and return 0 if any token expansion fails.
Return 1 when the whole list is expanded successfully.
*/
// tokens = shell->tokens [head of the token]
int	expand_tokens(t_token *tokens, t_shell *shell)
{
	while(tokens)
	{
		if (!expand_one_token(tokens, shell))
			return (0); // failure
		// printf("va:%s\n", tokens->value);
		tokens = tokens->next;
	}
	return (1); // success
}
