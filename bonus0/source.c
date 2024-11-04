#include <string.h>
#include <unistd.h>
#include <stdio.h>

void	p(char *str, char *sep) {
	char			*end;
	char			buffer[4104];

	puts(sep);
	read(0, buffer, 4096);
	end = strchr(buffer, '\n');
	*end = '\0';
	strncpy(str, buffer, 20);
}

void pp(char *buffer) {
	char			var1;
	unsigned int	var2;
	char			var3;
	char			var4;
	char			buffer1[20];
	char			buffer2[20];

	var4 = 0;

	p(buffer1, " - ");
	p(buffer2, " - ");
	strcpy(buffer, buffer1);
	var2 = strlen(buffer);
	buffer[var2] = ' ';
	buffer[var2 + 1] =  '\0';
	strcat(buffer, buffer2);

	return ;
}

int main(void) {
	char buffer[54];

	pp(buffer);
	puts(buffer);

	return (0);
}
