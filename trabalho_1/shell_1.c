/*
 * Este programa é um shell simplificado que permite ao usuário executar comandos e navegar pelos diretórios.
 * Ele também fornece um recurso de histórico de comandos que permite ao usuário visualizar os últimos comandos executados.
 * O programa usa a biblioteca readline para ler a entrada do usuário e as chamadas do sistema fork e execvp para executar comandos.
 * O programa também usa as chamadas de sistema chdir e setenv para navegar pelos diretórios e definir variáveis ​​de ambiente.
 * O programa possui os seguintes comandos:
 * - cd: alterar diretório
 * - exit: sai do shell
 * - histórico: veja o histórico de comandos
 * O programa possui um comprimento máximo de comando de 100 caracteres e um máximo de 10 parâmetros por comando.
 * O programa possui um tamanho máximo de histórico de 10 comandos.
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

/*
 * Esta função adiciona um comando ao histórico.
 * Se o comando tiver mais de 0 caracteres, ele será adicionado ao array de histórico.
 * O array de histórico tem tamanho máximo de 10 comandos.
 * A posição do último comando adicionado é armazenada na variável posicao_historico.
 */
void adicionar_historico(char *comando)
{
    if (strlen(comando) > 0)
    {
        strcpy(historico[posicao_historico], comando);
        posicao_historico = (posicao_historico + 1) % MAX_HISTORICO;
    }
}

/*
 * Esta função exibe o histórico de comandos.
 * A função itera pela matriz de histórico e imprime cada comando com comprimento maior que 0.
 */
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

/*
 * Esta função lê a entrada do usuário e a analisa em um comando e seus parâmetros.
 * A função usa a biblioteca readline para ler a entrada do usuário e a função strtok para analisar a entrada.
 * A função também adiciona o comando ao histórico utilizando a função adicionar_historico.
 * A função possui no máximo 10 parâmetros por comando.
 */
void ler_comando(char *parametro[])
{
    char *prompt = strcat(getcwd(NULL, 0), " - > ");
    char *input = readline(prompt);

    if (strlen(input) > 0)
    {
        add_history(input);
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
    free(input);
}

/*
 * Esta é a principal função do programa.
 * A função inicializa o array de histórico e define o diretório atual como o diretório inicial.
 * A função então entra em um loop que lê a entrada do usuário e executa comandos até que o usuário insira o comando “exit”.
 * A função usa as chamadas de sistema fork e execvp para executar comandos e a chamada de sistema waitpid para aguardar a conclusão do processo filho.
 * A função também usa as chamadas de sistema chdir e setenv para navegar pelos diretórios e definir variáveis ​​de ambiente.
 * A função tem um comprimento máximo de comando de 100 caracteres e um máximo de 10 parâmetros por comando.
 * A função tem um tamanho máximo de histórico de 10 comandos.
 */
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
