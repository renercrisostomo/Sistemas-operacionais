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
#include <readline/readline.h>
#include <readline/history.h>

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

char *ler_comando()
{
    char s[MAX_COMANDO];
    char prompt[MAX_COMANDO];
    sprintf(prompt, "%s - > ", getcwd(s, MAX_COMANDO));
    char *input = readline(prompt);

    if (input && *input)
    {
        add_history(input);
        adicionar_historico(input);
    }

    return input;
}

int main()
{
    char *comando;
    char *parametro[MAX_PARAMETROS + 1];
    char s[MAX_COMANDO];
    int status;

    system("clear");
    printf("Bem vindo ao shell simplificado!\n\n");
    chdir(getenv("HOME"));

    while (TRUE)
    {
        comando = ler_comando();

        if (!comando)
        {
            continue;
        }

        int i = 0;
        char *token = strtok(comando, " ");
        while (token != NULL && i < MAX_PARAMETROS)
        {
            parametro[i] = strdup(token);
            token = strtok(NULL, " ");
            i++;
        }
        parametro[i] = NULL;


        if (strcmp(comando, "exit") == 0)
        {
            printf("\nSaindo do shell...\n\n");
            free(comando);
            break;
        }

        if (strcmp(comando, "cd") == 0)
        {
            if (parametro[1] == NULL)
            {
                chdir(getenv("HOME"));
                char *cwd = getcwd(NULL, 0);
                setenv("PWD", cwd, 1);
                free(cwd);
                free(comando);
                continue;
            }
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
            free(comando);
            continue;
        }

        if (strcmp(comando, "history") == 0)
        {
            exibir_historico();
            free(comando);
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
                perror("execvp");
                break;
            }
        }

        free(comando);
    }

    return 0;
}
