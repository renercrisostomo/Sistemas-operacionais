#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_HISTORY_SIZE 10

void print_history(char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH], int history_size)
{
    printf("Command history:\n");
    for (int i = 0; i < history_size; i++)
    {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

int main()
{
    char command[MAX_COMMAND_LENGTH];
    char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH];
    int history_size = 0;

    while (1)
    {
        printf("Enter a command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove trailing newline character

        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "history") == 0)
        {
            print_history(history, history_size);
        }
        else
        {
            if (history_size < MAX_HISTORY_SIZE)
            {
                strcpy(history[history_size], command);
                history_size++;
            }
            else
            {
                // Shift the history array to make space for the new command
                for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++)
                {
                    strcpy(history[i], history[i + 1]);
                }
                strcpy(history[MAX_HISTORY_SIZE - 1], command);
            }

            printf("Executing command: %s\n", command);
            // Add code here to execute the command
        }
    }

    return 0;
}
