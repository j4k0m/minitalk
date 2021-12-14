#include "minitalk.h"

void	send_signal(int bit, int pid)
{
	if (bit == 0)
	{
		kill(pid, SIGUSR1);
		usleep(100);
	}
	if (bit == 1)
	{
		kill(pid, SIGUSR2);
		usleep(100);
	}
}

void	convert(int c, int pid)
{
	t_byte_struct byte_char;
	
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
		while (argv[2][i])
		{
			convert((int)argv[2][i], pid);
			i++;
		}
	}
	else {
		write(1, "Usage: ./client [PID] [STRING_TO_PASS]\n", 39);
	}
	return (0);
}
