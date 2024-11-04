#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {

	char	buffer[40];
	int		nb;

	nb = atoi(argv[1]);
	if (nb < 10) {
		memcpy(buffer, argv[2], nb * 4);
		if (nb == 1464814662) {
			execl("/bin/sh", "sh", NULL);
		}
		return (0);
	}
	return (1);
}
