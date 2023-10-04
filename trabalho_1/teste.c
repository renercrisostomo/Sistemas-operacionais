#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1
#define MAX_COMANDO 100
#define MAX_PARAMETROS 10
#define MAX_HISTORICO 1000

char historico[MAX_HISTORICO][MAX_COMANDO];
int posicao_historico = 0;

void adicionar_historico(char *comando)
{
    if (strlen(comando) > 0)
    {
        strcpy(historico[posicao_historico], comando);
        posicao_historico = (posicao_historico + 1) % MAX_HISTORICO;
    }
}

void exibir_historico()
{
    int i, j;
    j = posicao_historico;
    for (i = 0; i < MAX_HISTORICO; i++)
    {
        if (strlen(historico[j]) > 0)
        {
            printf("%s\n", historico[j]);
        }
        j = (j + 1) % MAX_HISTORICO;
    }
}

void ler_comando(char *comando, char *parametro[])
{
    char input[MAX_COMANDO];
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0)
    {
        adicionar_historico(input);
    }

    char *token = strtok(input, " ");
    if (token != NULL)
    {
        strcpy(comando, token);

        if (strcmp(comando, "exit") == 0)
        {
            printf("\nSaindo do shell...\n\n");
            exit(0);
        }

        int i = 0;
        while (token != NULL && i < MAX_PARAMETROS)
        {
            parametro[i] = strdup(token);
            token = strtok(NULL, " ");
            i++;
        }
        parametro[i] = NULL;
    }
}

int main()
{
    char comando[MAX_COMANDO];
    char *parametro[MAX_PARAMETROS + 1];
    char s[100];
    int status;

    system("clear");
    printf("Bem vindo ao shell simplificado!\n\n");

    while (TRUE)
    {
        printf("%s - >", getcwd(s, 100));
        ler_comando(comando, parametro);

        if (strcmp(comando, "cd") == 0)
        {
            if (chdir(parametro[1]) != 0)
            {
                perror("Falha ao mudar o diretório");
            }
            else
            {
                char *cwd = getcwd(NULL, 0);
                setenv("PWD", cwd, 1);
                free(cwd);
            }
            continue;
        }

        if (strcmp(comando, "history") == 0)
        {
            exibir_historico();
            continue;
        }

        if (fork() != 0)
        {
            waitpid(-1, &status, 0);
        }
        else
        {
            if (execvp(comando, parametro) < 0)
            {
                perror("execvp");
                break;
            }
        }
    }
    return 0;
}
