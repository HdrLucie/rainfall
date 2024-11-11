#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int language_choice = 0;

void greetuser(char *name) {
    char greeting[72];

    if (language_choice == 1)
        strcpy(greeting, "Hyvää päivää ");
    else if (language_choice == 2)
        strcpy(greeting, "Goedemiddag! ");
    else
        strcpy(greeting, "Hello ");

    strcat(greeting, name);
    puts(greeting);
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
            language_choice = 1;
        }
        else if (memcmp(env_lang, "nl", 2) == 0) {
            language_choice = 2;
        }
    }

    char stack_buffer[76];
    memcpy(stack_buffer, buffer, 19 * sizeof(int));
    greetuser(stack_buffer);

    return 0;
}
