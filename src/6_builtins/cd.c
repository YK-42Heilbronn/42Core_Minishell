/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 19:10:23 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:01:41 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

static char	*resolve_cd_target(char **argv, t_shell *shell)
{
	char	*home;

	if (argv[1])
		return (argv[1]);
	home = env_get_value(shell->env, "HOME");
	if (!home)
	{
		write_str(2, "minishell: cd: HOME not set\n");
		return (NULL);
	}
	return (home);
}

static void	update_cd_env(t_shell *shell, char *old_cwd)
{
	char	new_cwd[4096];

	if (old_cwd)
		env_set_value(&shell->env, "OLDPWD", old_cwd);
	if (getcwd(new_cwd, sizeof(new_cwd)))
		env_set_value(&shell->env, "PWD", new_cwd);
}

/* subject only requires relative/absolute path support, so no `cd -`
 * and no bare `cd` falling back to $HOME beyond the simple case above. */

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*target;
	char	*old_cwd;
	int		ret;

	if (argv[1] && argv[2])
	{
		write_str(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	target = resolve_cd_target(argv, shell);
	if (!target)
		return (1);
	old_cwd = getcwd(NULL, 0);
	ret = 0;
	if (chdir(target) == -1)
	{
		print_exec_error(target, strerror(errno));
		ret = 1;
	}
	else
		update_cd_env(shell, old_cwd);
	return (free(old_cwd), ret);
}
