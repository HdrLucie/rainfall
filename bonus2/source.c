#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int language = 0;

int greetuser(char *name) {
    char greeting[72];

    if (language == 1)
        strcpy(greeting, "Hyvää päivää ");
    else if (language == 2)
        strcpy(greeting, "Goedemiddag! ");
    else
        strcpy(greeting, "Hello ");

    strcat(greeting, name);
    return (puts(greeting));
}

int main(int argc, char **argv) {
    char buffer[160];
    char *env_lang;

    if (argc != 3)
        return 1;

    memset(buffer, 0, 76);

    strncpy(buffer, argv[1], 40);

    strncpy(buffer + 40, argv[2], 32);

    env_lang = getenv("LANG");
    if (env_lang != NULL) {
        if (memcmp(env_lang, "fi", 2) == 0) {
            language = 1;
        }
        else if (memcmp(env_lang, "nl", 2) == 0) {
            language = 2;
        }
    }

    char stack_buffer[76];
    memcpy(stack_buffer, buffer, 19 * sizeof(int));

    return (greetuser(stack_buffer));
}
