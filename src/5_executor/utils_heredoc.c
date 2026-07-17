/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 02:24:17 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:07:37 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	is_delimiter(char *line, char *delim)
{
	return (ft_strlen(line) == ft_strlen(delim)
		&& ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0);
}

void	write_heredoc_line(int fd, char *expanded)
{
	if (!expanded)
		return ;
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

/* sweeps up any stray heredoc-writer children that may still be around
 * (they normally finish on their own once the reading command is done). */

void	reap_leftover_children(void)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

int	append_dollar_var(char **result, const char *line, t_shell *shell,
		int *i)
{
	char	*name;
	char	*value;
	int		len;

	len = var_name_len(&line[*i + 1]);
	name = extract_var_name(&line[*i + 1], len);
	if (!name)
		return (0);
	value = env_get_value(shell->env, name);
	free(name);
	*result = str_append_str(*result, value);
	*i += len + 1;
	return (*result != NULL);
}

int	append_dollar(char **result, const char *line, t_shell *shell,
		int *i)
{
	char	*value;

	if (line[*i + 1] == '?')
	{
		value = expand_exit_status(shell);
		if (!value)
			return (0);
		*result = str_append_str(*result, value);
		free(value);
		*i += 2;
		return (*result != NULL);
	}
	if (!is_valid_var_start(line[*i + 1]))
	{
		*result = str_append_char(*result, '$');
		(*i)++;
		return (*result != NULL);
	}
	return (append_dollar_var(result, line, shell, i));
}
