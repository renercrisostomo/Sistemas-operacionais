#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1
#define MAX_COMMAND_LENGTH 100
#define MAX_PARAMETERS 10

void type_prompt()
{
    printf("Shell> ");
}

void read_command(char *command, char *parameters[])
{
    char input[MAX_COMMAND_LENGTH];
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    char *token = strtok(input, " ");
    strcpy(command, token);

    int i = 0;
    while (token != NULL && i < MAX_PARAMETERS)
    {
        parameters[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    parameters[i] = NULL;
}

int main()
{
    char command[MAX_COMMAND_LENGTH];
    char *parameters[MAX_PARAMETERS + 1];
    int status;

    system("clear");
    printf("Bem vindo ao shell simplificado!\n\n");


    while (TRUE)
    {
        type_prompt();
        read_command(command, parameters);

        if (fork() != 0)
        {
            waitpid(-1, &status, 0);
        }
        else
        {
            execvp(command, parameters);
        }
    }
    return 0;
}
