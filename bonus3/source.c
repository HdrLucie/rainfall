#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char **argv) {
	char *ptr;
	char bVar2;
	char buffer [65];
	char local_57;
	char buffer2 [66];
	FILE *passfile;

	bVar2 = 0;
	passfile = fopen("/home/user/end/.pass","r");
	ptr = buffer;
	memset(ptr, 0, 0x21 * 4);
	if ((passfile == NULL) || (argc != 2)) {
		return (-1);	
	}
	else {
		fread(buffer,1,0x42,passfile);
		local_57 = 0;
		buffer[atoi(argv[1])] = '\0';
		fread(buffer2,1,0x41,passfile);
		fclose(passfile);
		if (strcmp(buffer, argv[1]) == 0) {
			execl("/bin/sh","sh",NULL);
		}
		else {
			puts(buffer2);
		}
	}
	return (0);
}
