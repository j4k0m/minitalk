/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hask <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:31:25 by ael-hask          #+#    #+#             */
/*   Updated: 2021/12/13 20:58:45 by ael-hask         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static size_t	check(size_t n, const char *nptr, int sign)
{
	while (*nptr >= '0' && *nptr <= '9')
	{
		if (n > 9223372036854775807 && sign == 1)
			return (0);
		if (n > 9223372036854775808U && sign == -1)
			return (-1);
		n = *nptr - '0' + (n * 10);
		nptr++;
	}
	return (n * sign);
}

int	ft_atoi(const char *nptr)
{
	size_t	n;
	int		sign;
	int		d;

	d = 0;
	sign = 1;
	n = 0;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	while (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		d++;
		nptr++;
	}
	if (d > 1)
		return (n);
	n = check(n, nptr, sign);
	return (n);
}

