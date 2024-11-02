#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_auth {
    char s[32];
    int  n;
} t_auth;

t_auth  *auth;
char    *service;

int main(void)
{
    char buffer[129];

    while (1)
    {
        printf("%p, %p\n", auth, service);
        if (fgets(buffer, 128, stdin) == NULL)
        {
            return 0;
        }
        if (memcmp(buffer, "auth ", 5) == 0)
        {
            auth = malloc(4);
            auth->s[0] = 0;
            auth->s[1] = 0;
            auth->s[2] = 0;
            auth->s[3] = 0;
            if (strlen(buffer + 5) <= 30)
            {
                strcpy((char *)auth, buffer + 5);
            }
        }
        if (memcmp(buffer, "reset", 5) == 0)
        {
            free(auth);
        }
        if (memcmp(buffer, "service", 6) == 0)
        {
            service = strdup(buffer + 7);
        }
        if (memcmp(buffer, "login", 5) == 0)
        {
            if (auth->n != 0)
            {
                system("/bin/sh");
            }
            else
            {
                fwrite("Password:\n",1,10,stdout);
            }
        }
    }
}
