/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:30:59 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:47:56 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

static int	is_echo_flag(char *arg)
{
	int	i;

	if (arg[0] != '-' || !arg[1])
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	while (argv[i] && is_echo_flag(argv[i]))
	{
		nl = 0;
		i++;
	}
	while (argv[i])
	{
		write_str(1, argv[i]);
		if (argv[i + 1])
			write_str(1, " ");
		i++;
	}
	if (nl)
		write_str(1, "\n");
	return (0);
}

// int	builtin_echo(t_cmd *cmd)
// {
// 	int	i;
// 	int	new_line;

// 	i = 1;
// 	new_line = 1;
// 	if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-n", 3) == 0)
// 	{
// 		new_line = 0;
// 		i++;
// 	}
// 	while (cmd->argv[i])
// 	{
// 		write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
// 		if (cmd->argv[i + 1])
// 			write(1, " ", 1);
// 		i++;
// 	}
// 	if (new_line)
// 		write(1, "\n", 1);
// 	return (0);
// }
