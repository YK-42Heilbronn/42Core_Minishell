/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:23 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/11 16:10:03 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"

int process_line(t_shell *shell)
{
    if (!shell || !shell->line)
        return(-1);
    shell->tokens = lex_input(shell-line, shell);
    if (!shell->tokens)
        return (-1);
    if (syntax_check_tokens(shell->tokens))
        return (-1);
    shell->cmds = parse_tokens(shell->tokens, shell);
    if (!shell->cmds)
        return(-1);
    shell->last_status = execute_shell(shell);
    return (0);
}
