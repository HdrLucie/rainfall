#include <string.h>
#include <unistd.h>
#include <stdio.h>

void	p(char *str, char *sep) {
	char			*end;
	char			buffer[4120];

	puts(sep);
	read(0, buffer, 4096);
	end = strchr(buffer, '\n');
	*end = '\0';
	strncpy(str, buffer, 20);
}

void pp(char *buffer) {
	unsigned int	len;
	char			buffer1[20];
	char			buffer2[20];

	p(buffer1, " - ");
	p(buffer2, " - ");
	strcpy(buffer, buffer1);
	len = strlen(buffer);
	buffer[len] = ' ';
	buffer[len + 1] =  '\0';
	strcat(buffer, buffer2);

	return ;
}

int main(void) {
	char buffer[54];

	pp(buffer);
	puts(buffer);

	return (0);
}
