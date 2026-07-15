/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:39:34 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:48:15 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	builtin_pwd(void)
{
	char	cwd[4096];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		write_str(2, "minishell: pwd: error retrieving current directory\n");
		return (1);
	}
	write(1, cwd, ft_strlen(cwd));
	write_str(1, "\n");
	return (0);
}

// int	builtin_pwd(t_cmd *cmd)
// {
// 	char	*pwd;

// 	(void)cmd;
// 	pwd = getcwd(NULL, 0);
// 	if (pwd == NULL)
// 		return (perror("getcwd() failed"), 1);
// 	ft_putstr_fd(pwd, 1);
// 	ft_putchar_fd('\n', 1);
// 	free(pwd);
// 	return (0);
// }
