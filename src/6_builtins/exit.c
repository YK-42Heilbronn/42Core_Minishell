/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 23:35:15 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:48:05 by ileongar         ###   ########.fr       */
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

// // exit with no args: print exit, set shell->exit_flag,
// //	exit with shell->last_status
// // exit N: print exit, set exit status to N if numeric,
// //	else print error and exit with status 2
// // This is what the minishell subject expects for exit without options

// int	is_numeric(const char *s)
// {
// 	int	i;

// 	if (!s || !*s)
// 		return (0);
// 	i = 0;
// 	if (s[i] == '+' || s[i] == '-')
// 		i++;
// 	while (s[i])
// 	{
// 		if (!ft_isdigit(s[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// long	ft_long(const char *s)
// {
// 	long	res;
// 	int		sign;
// 	int		i;

// 	sign = 1;
// 	i = 0;
// 	if (s[i] == '+' || s[i] == '-')
// 	{
// 		if (s[i] == '-')
// 			sign = -1;
// 		i++;
// 	}
// 	res = 0;
// 	while (s[i] && ft_isdigit(s[i]))
// 	{
// 		res = res * 10 + (s[i] - '0');
// 		i++;
// 	}
// 	return (res * sign);
// }

// int	builtin_exit(t_shell *shell, t_cmd *cmd)
// {
// 	long	code;

// 	write(1, "exit\n", 5);
// 	if (!cmd->argv[1])
// 	{
// 		shell->exit_flag = 1;
// 		return (shell->last_status);
// 	}
// 	if (!is_numeric(cmd->argv[1]))
// 	{
// 		write(2, "minishell: exit: ", 17);
// 		write(2, cmd->argv[1], ft_strlen(cmd->argv[1]));
// 		write(2, ": numeric argument required\n", 28);
// 		shell->exit_flag = 1;
// 		return (2);
// 	}
// 	if (cmd->argv[2])
// 	{
// 		write(2, "minishell: exit: too many arguments\n", 36);
// 		return (1);
// 	}
// 	code = ft_long(cmd->argv[1]);
// 	shell->exit_flag = 1;
// 	return ((int)(code & 0xFF));
// }
