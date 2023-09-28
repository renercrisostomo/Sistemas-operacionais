#include <stdio.h>
#include <string.h>

int main()
{
    char input[100];
    char command[1];

    printf("Digite um comando: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    char *token = strtok(input, " ");
    strcpy(command, token);

    printf("Comando: %s\n", command);

    return 0;
}
