#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void print_command_and_parameters(char *command, char **parameters)
{
    printf("Command: %s\n", command);
    printf("Parameters:\n");
    for (int i = 0; parameters[i] != NULL; i++)
    {
        printf("  - %s\n", parameters[i]);
    }
}

int main() {
    char texte_digitado[200];
    int sair = 0;
    int status;
    char *comando;
    char *parametros[10];
    int contador;
    char *parametro;
    pid_t pid;

    printf("Bem vindo ao shell simplif  icado!\n\n");

    do {
        printf(">>> ");
        scanf("%[^\n]%*c", texte_digitado);
        contador = 0;   

        if (strcmp(texte_digitado, "exit") == 0) {
            printf("\nSaindo do shell...\n\n");
            sair = 1;
        } else {
            pid = fork();
            if (pid != 0) {
                waitpid(-1, &status, 0);
            } else {
                comando = (char *)strtok(texte_digitado, " ");
                parametros[0] = comando;  

                do {
                    parametro = (char *)strtok(NULL, " ");
                    parametros[++contador ] = parametro;
                } while (parametro != NULL);

                parametros[contador] = NULL;
                print_command_and_parameters(comando, parametros);
                execvp(comando, parametros);
                sair = 1;            
            }
        }
    } while (!sair);
    return 0;
}