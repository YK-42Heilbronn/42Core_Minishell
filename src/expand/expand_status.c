/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 16:02:27 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/11 17:33:50 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

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
