/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:22:02 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 23:31:49 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#incluce "../libs/libft/libft.h"

/*main*/
int main(int argc, char **argv, char **envp);

/*init*/
void    init_shell(t_shell *sh, char **envp);
t_env   *init_env(char **envp);

#endif