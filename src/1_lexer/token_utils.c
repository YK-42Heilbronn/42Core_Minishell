/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 17:23:54 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 18:48:08 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "lexer.h"

int  is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}

int  is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

// int is_quote(char c)
// {
// }

char *extract_word(const char *line, int start, int len)
{
    char    *word;
    // int     i;

    word = ft_substr(line, start, len);
    return (word);
}

int  word_len(const char *line, int i)
{
    int     len;
    char    quote;

    len = 0;
    while (line[i + len] && !is_whitespace(line[i + len])
        && !is_operator_char(line[i + len]))
    {
        if (line[i + len] == '\'' || line[i + len] == '"')
        {
            quote = line[i + len++];
            while (line[i + len] && line[i + len] != quote)
                len++;
            if (line[i + len] == quote)
                len++;
        }
        else
            len++;
    }
    return (len);
}

