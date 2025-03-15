/// Expects a string json object like so:
/// { "key": "value", ... }
int parse_int(char* s_json, char* key);

/// Expects a string json object like so:
/// { "key": "value", ... }
char* parse_string(char* s_json, char* key);

/// Expects a string json object like so:
/// { "id": 1, "beacon_id" : 1, "type": "webshell", "arg": "" }
struct Command* parse_command(char* s_json_command);

/// Expects a string json object like so:
/// [
///   { "id": 1, "beacon_id" : 1, "type": "webshell", "arg": "" },
///   ...
/// ]
struct Command** parse_commands(char* json_s);

/// Expects a string json object like so:
/// {
///   "id": 1, "ip": "192.168.100.1", "hostname": "c2-beacon-test",
///   "commands": [
///     { "id": 1, "beacon_id" : 1, "type": "webshell", "arg": "" },
///     ...
///   ]
/// }
struct Beacon* parse_beacon(char* json_s);

char* unparse_command(struct Command* command);
char* unparse_commands(struct Command** commands);
char* unparse_beacon(struct Beacon* beacon);
