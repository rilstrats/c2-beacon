#include <stdbool.h>

struct Command {
    int id;
    int beacon_id;
    char* type;
    char* arg;
    bool executed;
    char* result;
};

struct Beacon {
    int id;
    char* ip;
    char* hostname;
    struct Command** commands;
    int len_commands;
};
