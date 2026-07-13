/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 16:02:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/11 18:03:01 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

// /*
// Behavior::
// If sq == 1, return ft_strdup(word) with no expansion.
// If not single-quoted:
// 	copy normal chars as-is
// 	when seeing $?, replace with expand_exit_status(shell)
// 	when seeing $ followed by valid var start, extract var name and replace using env_get_value(shell->env, key)
// 	if variable does not exist, append empty string
// 	if $ is followed by invalid char or end of string, copy $ literally
// dq does not block expansion; it only matters because double quotes allow $ expansion while single quotes do not
// */
// // sq = single quoted, dq = double quoted
// // all the shell variables(session_custom+user+system) are added to ENV, can be extracted from the env_linked_list
// // expansion only starts if there is an $ sign
// // expansion rules:
// // single quotes: no expansion
// // double quotes: expansion takes place
// char	*expand_word(const char *word, t_shell *shell, int sq, int dq)
// {
// 	char *ext_word;

// 	if (sq == 1)
// 		return (ft_strdup(word));
// 	if (dq == 1)
// 	{
// 		if (*word == '$')
// 		{
// 			word++;
// 			if (*word == '?')
// 				return (expand_exit_status(shell));
// 			else
// 			{
// 				if (is_valid_var_start(*word))
// 				{
// 					ext_word = extract_var_name(word, ft_strlen(word));
// 					ext_word = env_get_value(shell, ext_word);
// 					if (ext_word == NULL)
// 						str_append_str(ext_word, "");  // append empty string
// 					else
// 					{
// 						if (!is_valid_var_char(*word))
// 							ext_word = "$";
// 					}
// 						str_append_str(word, ext_word); // append expanded string
// 				}
// 				// else{
// 				// }
// 			}
// 		}
// 	}
// }

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
int	expand_one_token(t_token *token, t_shell *shell)
{
	char *new_tok_value;

	if (token == NULL)
		return (1);
	if (token->type != TOK_WORD)
		return (1); // success
	else
	{
		new_tok_value = expand_word(token->value, shell, token->quoted_single, token->quoted_double);
		if (new_tok_value == NULL)
			return (0); // failure
		free(token->value);
		token->value = new_tok_value;
		return (1); // success
	}
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
		tokens = tokens->next;
	}
	return (1); // success
}
