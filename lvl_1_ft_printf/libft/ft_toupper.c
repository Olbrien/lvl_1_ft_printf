/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 22:55:27 by marvin            #+#    #+#             */
/*   Updated: 2020/12/19 17:51:41 by ncameiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	int re;

	if (c < -1)
		return ((unsigned char)c);
	if (c >= 97 && c <= 122)
		return (c - 32);
	else
		re = c;
	return (re);
}
