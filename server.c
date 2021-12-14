/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hask <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:56:21 by ael-hask          #+#    #+#             */
/*   Updated: 2021/12/14 19:56:26 by ael-hask         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_byte_struct	g_char_byte;

void	ft_putnbr(int n)
{
	char	c;

	if (n >= 10)
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
	else
	{
		c = n + '0';
		write(1, &c, 1);
	}
}

void	set_bit(int bit, int	*i)
{
	if (*i == 1)
		g_char_byte.b1 = bit;
	if (*i == 2)
		g_char_byte.b2 = bit;
	if (*i == 3)
		g_char_byte.b3 = bit;
	if (*i == 4)
		g_char_byte.b4 = bit;
	if (*i == 5)
		g_char_byte.b5 = bit;
	if (*i == 6)
		g_char_byte.b6 = bit;
	if (*i == 7)
		g_char_byte.b7 = bit;
	if (*i == 8)
	{
		g_char_byte.b8 = bit;
		write(1, &*(unsigned char *)&g_char_byte, 1);
		*i = 0;
	}
}

void	handler(int sig)
{
	static int	i;

	if (!i)
		i = 1;
	if (sig == SIGUSR1)
	{
		set_bit(0, &i);
		i++;
	}
	if (sig == SIGUSR2)
	{
		set_bit(1, &i);
		i++;
	}
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	write(1, "PID: ", 6);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
