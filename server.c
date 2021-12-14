#include "minitalk.h"

t_byte_struct char_byte;

void	ft_putnbr(int n)
{
	char	c;

	if (n >= 10)
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	} else {
		c = n + '0';
		write(1, &c, 1);
	}
}

void	set_bit(int bit, int	*i)
{
	if (*i == 1)
		char_byte.b1 = bit;
	if (*i == 2)
		char_byte.b2 = bit;
	if (*i == 3)
		char_byte.b3 = bit;
	if (*i == 4)
		char_byte.b4 = bit;
	if (*i == 5)
		char_byte.b5 = bit;
	if (*i == 6)
		char_byte.b6 = bit;
	if (*i == 7)
		char_byte.b7 = bit;
	if (*i == 8)
	{
		char_byte.b8 = bit;
		write(1, &*(unsigned char *)&char_byte, 1);
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
	struct sigaction sa;

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
