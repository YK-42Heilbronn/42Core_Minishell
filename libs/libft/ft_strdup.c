/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:14:10 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/13 17:26:22 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
    char	*copy;
    size_t	i;
    size_t	len;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    copy = malloc(len + 1);
    if (!copy)
        return (NULL);
    i = 0;
    while (i <= len)
    {
        copy[i] = s1[i];
        i++;
    }
    return (copy);
}
