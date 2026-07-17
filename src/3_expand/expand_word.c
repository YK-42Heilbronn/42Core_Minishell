/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:56:07 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 02:35:44 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static int	handle_dollar(char **result, const char *word, t_shell *shell,
		int *i)
{
	char	next;

	next = word[*i + 1];
	if (next == '?')
		return (append_exit_status(result, shell, i));
	if (is_valid_var_start(next))
		return (append_env_var(result, word, shell, i));
	return (append_char_at(result, word, i));
}

static int	is_quote_delimiter(int state, char c)
{
	if (c == '\'' && state != 2)
		return (1);
	if (c == '"' && state != 1)
		return (1);
	return (0);
}

static void	toggle_quote_state(int *state, char c)
{
	if (c == '\'' && *state != 2)
	{
		if (*state == 1)
			*state = 0;
		else
			*state = 1;
	}
	else if (c == '"' && *state != 1)
	{
		if (*state == 2)
			*state = 0;
		else
			*state = 2;
	}
}

// q_state: [single] | [double]
// q_state: [open, close]
// q_state: [-1, 1]  | [-2, 2]
static int	process_expand_step(char **result, const char *word,
				t_shell *shell, int *state_i)
{
	int	*state;
	int	*i;

	state = &state_i[0];
	i = &state_i[1];
	if (is_quote_delimiter(*state, word[*i]))
		return (toggle_quote_state(state, word[(*i)++]), 1);
	if (word[*i] == '$' && *state != 1)
		return (handle_dollar(result, word, shell, i));
	return (append_char_at(result, word, i));
}

char	*expand_word(const char *word, t_shell *shell, int q_state)
{
	char	*result;
	int		state_i[2];

	(void)q_state;
	if (!word)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	state_i[0] = 0;
	state_i[1] = 0;
	while (word[state_i[1]])
		if (!process_expand_step(&result, word, shell, state_i))
			return (free(result), NULL);
	return (result);
}
// char	*expand_word(const char *word, t_shell *shell, int q_state)
// {
// 	char	*result;
// 	int		i;
// 	int		state;

// 	(void)q_state;
// 	if (!word)
// 		return (ft_strdup(""));
// 	result = ft_strdup("");
// 	if (!result)
// 		return (NULL);
// 	state = 0;
// 	i = 0;
// 	while (word[i])
// 	{
// 		if (is_quote_delimiter(state, word[i]))
// 		{
// 			toggle_quote_state(&state, word[i]);
// 			i++;
// 			continue ;
// 		}
// 		if (word[i] == '$' && state != 1)
// 		{
// 			if (!handle_dollar(&result, word, shell, &i))
// 			{
// 				free(result);
// 				return (result = NULL, NULL);
// 			}
// 			continue ;
// 		}
// 		if (!append_char_at(&result, word, &i))
// 		{
// 			free(result);
// 			return (result = NULL, NULL);
// 		}
// 	}
// 	return (result);
// }
