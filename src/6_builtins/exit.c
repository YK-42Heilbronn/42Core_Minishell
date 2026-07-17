/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 23:35:15 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:02:12 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

/* prints bash's "exit" line, validates the (optional) numeric argument,
 * then hands off to shell_exit() which frees what we own and calls the
 * real exit(). Correct whether "exit" is the sole foreground command
 * (terminates the whole shell) or a stage inside a pipeline (terminates
 * only that forked child, matching bash's `exit | cat` behaviour). */

int	builtin_exit(char **argv, t_shell *shell)
{
	int	status;

	write_str(1, "exit\n");
	if (argv[1] && argv[2])
	{
		write_str(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	status = shell->last_status;
	if (argv[1] && !is_numeric_arg(argv[1]))
	{
		write_str(2, "minishell: exit: ");
		write_str(2, argv[1]);
		write_str(2, ": numeric argument required\n");
		shell_exit(shell, 2);
	}
	if (argv[1])
		status = ft_atoi(argv[1]);
	shell_exit(shell, status);
	return (0);
}

int	is_numeric_arg(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]))
			return (0);
		i++;
	}
	return (1);
}
