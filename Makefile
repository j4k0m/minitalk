all: server client

server:
	cc -Wall -Wextra -Werror server.c -o server

client:
	cc -Wall -Wextra -Werror client.c -o client

clean:
	rm -f server client

fclean: clean

re: fclean all