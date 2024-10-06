#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	int n;
	char *str;
	uid_t euid;
	gid_t egid;

	n = atoi(argv[1]);
	if (n == 423)
	{
		str = strdup("/bin/sh");
		egid = getegid();
		euid = geteuid();
		setresgid(egid,egid,egid);
		setresuid(euid,euid,euid);
		execv("/bin/sh",&str);
	}
	else
	{
		fwrite("No !\n",1,5,(FILE *)stderr);
	}
	return 0;
}
