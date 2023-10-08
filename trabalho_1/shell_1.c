/*
 * Este programa é um shell simplificado que permite ao usuário executar comandos e navegar pelos diretórios.
 * Também mantém um histórico dos comandos executados e permite ao usuário visualizá-lo.
 * O programa usa chamadas de sistema como fork(), execvp(), chdir() e waitpid() para executar comandos e navegar em diretórios.
 * O programa também define constantes como TRUE, MAX_COMANDO, MAX_PARAMETROS e MAX_HISTORICO para limitar o tamanho das matrizes de comandos e parâmetros e do buffer de histórico.
 * O programa também define funções como adicionar_historico(), exibir_historico() e ler_comando() para adicionar comandos ao buffer de histórico, exibir o buffer de histórico e ler a entrada do usuário.
 * O programa usa a variável de ambiente HOME para definir o diretório inicial e a variável de ambiente PWD para controlar o diretório atual.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1
#define MAX_COMANDO 100
#define MAX_PARAMETROS 10
#define MAX_HISTORICO 10

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

void ler_comando(char *parametro[])
{
    char input[MAX_COMANDO];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0)
    {
        adicionar_historico(input);
    }

    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL && i < MAX_PARAMETROS)
    {
        parametro[i] = strdup(token);
        token = strtok(NULL, " ");
        i++;
    }
    parametro[i] = NULL;
}

int main()
{
    char *parametro[MAX_PARAMETROS + 1];
    char s[100];
    int status;

    system("clear");
    printf("Bem vindo ao shell simplificado!\n\n");
    chdir(getenv("HOME"));

    while (TRUE)
    {

        printf("%s - > ", getcwd(s, 100));
        ler_comando(parametro);

        if (parametro[0] == NULL)
        {
            continue;
        }

        if (strcmp(parametro[0], "cd") == 0)
        {
            if (parametro[1] == NULL)
            {
                chdir(getenv("HOME"));
                setenv("PWD", getenv("HOME"), 1);
                continue;
            }
            if (chdir(parametro[1]) != 0)
            {
                perror(parametro[1]);
            }
            else
            {
                char *cwd = getcwd(NULL, 0);
                setenv("PWD", cwd, 1);
                free(cwd);
            }
            continue;
        }

        if (strcmp(parametro[0], "exit") == 0)
        {
            printf("\nSaindo do shell...\n\n");
            break;
        }

        if (strcmp(parametro[0], "history") == 0)
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
            if (execvp(parametro[0], parametro) < 0)
            {
                perror(parametro[0]);
                break;
            }
        }
    }
    return 0;
}
