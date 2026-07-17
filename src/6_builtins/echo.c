/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:30:59 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:01:51 by ileongar         ###   ########.fr       */
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
