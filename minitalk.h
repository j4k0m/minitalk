#ifndef MINITALK_H
# define MINITALK_H
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

typedef struct byte
{
    unsigned char b1:1;
    unsigned char b2:1;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char b6:1;
    unsigned char b7:1;
    unsigned char b8:1;
} t_byte_struct;

int	ft_atoi(const char *nptr);

#endif
