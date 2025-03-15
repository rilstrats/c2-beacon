#include "c2api.h"
#include "c2json.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    int id = register_beacon("192.168.100.1", "c2-beacon-test1");
    struct Beacon* beacon = get_beacon(id);

    struct Beacon* beacon1 = get_beacon(1);
    struct Command** commands = get_beacon_commands(1, false);

    printf("%s\n", unparse_beacon(beacon));
    printf("%s\n", unparse_beacon(beacon1));
    printf("%s\n", unparse_commands(commands));

    return 0;
}
