/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hask <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:56:31 by ael-hask          #+#    #+#             */
/*   Updated: 2021/12/14 19:56:32 by ael-hask         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static size_t	check(size_t n, const char *nptr, int sign)
{
	while (*nptr >= '0' && *nptr <= '9')
	{
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

void	send_signal(int bit, int pid)
{
	if (bit == 0)
	{
		if (kill(pid, SIGUSR1) == -1)
		{
			write(1, "Problem sending the signal!\n", 29);
			exit(1);
		}
		usleep(150);
	}
	if (bit == 1)
	{
		if (kill(pid, SIGUSR2) == -1)
		{
			write(1, "Problem sending the signal!\n", 29);
			exit(1);
		}
		usleep(150);
	}
}

void	convert(int c, int pid)
{
	t_byte_struct	byte_char;

	*(unsigned char *)&byte_char = (unsigned char)c;
	send_signal(byte_char.b1, pid);
	send_signal(byte_char.b2, pid);
	send_signal(byte_char.b3, pid);
	send_signal(byte_char.b4, pid);
	send_signal(byte_char.b5, pid);
	send_signal(byte_char.b6, pid);
	send_signal(byte_char.b7, pid);
	send_signal(byte_char.b8, pid);
}

int	main(int argc, char **argv)
{
	int	pid;
	int	i;

	i = 0;
	if (argc == 3)
	{
		pid = ft_atoi((const char *)argv[1]);
		if (pid == 0 || pid < 0)
		{
			write(1, "Invalid PID!\n", 13);
			return (0);
		}
		while (argv[2][i])
		{
			convert((int)argv[2][i], pid);
			i++;
		}
	}
	else
		write(1, "Usage: ./client [PID] [STRING_TO_PASS]\n", 40);
	return (0);
}
