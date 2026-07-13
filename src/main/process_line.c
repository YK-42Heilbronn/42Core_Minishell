/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:23 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 15:19:32 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	process_line(t_shell *shell)
{
	if (!shell || !shell->line)
		return (-1);
	shell->tokens = lex_input(shell->line, shell);
	if (!shell->tokens)
		return (shell->last_status = -1, -1);
	if (syntax_check_tokens(shell->tokens))
		return (shell->last_status = -1, -1);
	shell->cmds = parse_tokens(shell->tokens, shell);
	if (!shell->cmds)
		return (shell->last_status = -1, -1);
	// process_input(shell->line, shell);
	shell->last_status = execute_shell(shell);
	return (0);
}
