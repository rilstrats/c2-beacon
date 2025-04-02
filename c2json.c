#include "c2types.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cJSON* init_json(char* s_json)
{
    cJSON* json = cJSON_Parse(s_json);
    if (!json) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "cJSON_Parse() failed: %s\n", error_ptr);
        } else {
            fprintf(stderr, "cJSON_Parse() failed\n");
        }
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }
    return json;
}

int _parse_int(cJSON* json, char* key)
{
    cJSON* item = cJSON_GetObjectItem(json, key);
    if (!cJSON_IsNumber(item)) {
        fprintf(stderr, "cJSON_GetObjectItem(%s) failed\n", key);
        exit(EXIT_FAILURE);
    }
    return item->valueint;
}

int parse_int(char* s_json, char* key)
{
    return _parse_int(init_json(s_json), key);
}

char* _parse_string(cJSON* json, char* key)
{
    cJSON* item = cJSON_GetObjectItem(json, key);
    if (!cJSON_IsString(item) && item->valuestring != NULL) {
        fprintf(stderr, "cJSON_GetObjectItem(%s) failed\n", key);
        exit(EXIT_FAILURE);
    }
    return item->valuestring;
}

char* parse_string(char* s_json, char* key)
{
    return _parse_string(init_json(s_json), key);
}

struct Command* _parse_command(cJSON* json_command)
{
    struct Command* command = malloc(sizeof(struct Command));
    command->id = _parse_int(json_command, "id");
    command->beacon_id = _parse_int(json_command, "beacon_id");
    command->type = parse_command_type(_parse_string(json_command, "type"));
    command->arg = _parse_string(json_command, "arg");
    command->executed = 0;
    command->result = NULL;

    return command;
}

struct Command* parse_command(char* s_json_command)
{
    return _parse_command(init_json(s_json_command));
}

struct Command** _parse_commands(cJSON* json_commands)
{
    int size = cJSON_GetArraySize(json_commands);
    struct Command** commands = calloc(size+1, sizeof(struct Command*));

    cJSON* json_command;
    for (int i = 0; i < size; i++) {
        json_command = cJSON_GetArrayItem(json_commands, i);
        if (!json_command) {
            fprintf(stderr, "cJSON_GetArrayItem(%d) failed\n", i);
            exit(EXIT_FAILURE);
        }
        commands[i] = _parse_command(json_command);
    }

    commands[size] = NULL;

    return commands;
}

struct Command** parse_commands(char* json_s)
{
    return _parse_commands(init_json(json_s));
}

struct Beacon* _parse_beacon(cJSON* json_beacon)
{
    if (!json_beacon) {
        fprintf(stderr, "NULL pointer passed");
        exit(EXIT_FAILURE);
    }
    struct Beacon* beacon = malloc(sizeof(struct Beacon));

    beacon->id = _parse_int(json_beacon, "id");
    beacon->ip = _parse_string(json_beacon, "ip");
    beacon->hostname = _parse_string(json_beacon, "hostname");

    beacon->commands = NULL;
    cJSON* json_commands = cJSON_GetObjectItem(json_beacon, "commands");
    if (cJSON_IsArray(json_commands)) {
        beacon->commands = _parse_commands(json_commands);
    }

    return beacon;
}

struct Beacon* parse_beacon(char* s_json_beacon)
{
    return _parse_beacon(init_json(s_json_beacon));
}

cJSON* _unparse_command(struct Command* command)
{
    cJSON* json_command = cJSON_CreateObject();

    cJSON_AddNumberToObject(json_command, "id", command->id);
    cJSON_AddNumberToObject(json_command, "beacon_id", command->beacon_id);
    cJSON_AddStringToObject(json_command, "type", unparse_command_type(command->type));
    cJSON_AddStringToObject(json_command, "arg", command->arg);
    cJSON_AddBoolToObject(json_command, "executed", command->executed);
    if (command->result != NULL) {
        cJSON_AddStringToObject(json_command, "result", command->result);
    }

    return json_command;
}

char* unparse_command(struct Command* command)
{
    cJSON* json_command = _unparse_command(command);

    return cJSON_Print(json_command);
}

cJSON* _unparse_commands(struct Command** commands)
{
    cJSON* json_commands = cJSON_CreateArray();

    for (int i = 0; commands[i] != NULL; i++) {
        cJSON_AddItemToArray(json_commands, _unparse_command(commands[i]));
    }

    return json_commands;
}

char* unparse_commands(struct Command** commands)
{
    cJSON* json_commands = _unparse_commands(commands);

    return cJSON_Print(json_commands);
}

cJSON* _unparse_beacon(struct Beacon* beacon)
{
    cJSON* json_beacon = cJSON_CreateObject();

    cJSON_AddNumberToObject(json_beacon, "id", beacon->id);
    cJSON_AddStringToObject(json_beacon, "type", beacon->ip);
    cJSON_AddStringToObject(json_beacon, "arg", beacon->hostname);

    if (beacon->commands) {
        cJSON* json_commands = _unparse_commands(beacon->commands);
        cJSON_AddItemToObject(json_beacon, "commands", json_commands);
    }

    return json_beacon;
}

char* unparse_beacon(struct Beacon* beacon)
{
    cJSON* json_beacon = _unparse_beacon(beacon);

    return cJSON_Print(json_beacon);
}
