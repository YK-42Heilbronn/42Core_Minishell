/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:56:39 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/13 14:42:59 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"
#include <stdlib.h>

t_redir	*new_redir(t_redir_type type, char *file)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->type = type;
	new_redir->file = file;
	new_redir->next = NULL;
	return (new_redir);
}

void	cmd_add_redir(t_cmd *cmd, t_redir *redir)
{
	t_redir	*last;

	if (!cmd || !redir)
		return ;
	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return ;
	}
	last = cmd->redirs;
	while (last->next)
		last = last->next;
	last->next = redir;
}

// void free_redirs(t_redir *redirs)
// {
// 	t_redir	*tmp;

// 	while (redirs)
// 	{
// 		tmp = redirs;
// 		redirs = redirs->next;
// 		free(tmp->file);
// 		free(tmp);
// 	}
// }

static t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOK_REDIR_IN)
		return (R_IN);
	if (type == TOK_REDIR_OUT)
		return (R_OUT);
	if (type == TOK_APPEND)
		return (R_APPEND);
	return (R_HEREDOC);
}

int	parse_redirection(t_cmd *cmd, t_token **tokens)
{
	t_token			*op;
	t_token			*file_tok;
	t_redir_type	type;
	t_redir			*redir;

	if (!cmd || !tokens || !*tokens)
		return (0);
	op = *tokens;
	file_tok = op->next;
	if (!file_tok)
		return (0);
	type = get_redir_type(op->type);
	redir = new_redir(type, file_tok->value);
	if (!redir)
		return (0);
	cmd_add_redir(cmd, redir);
	*tokens = file_tok->next;
	return (1);
}
