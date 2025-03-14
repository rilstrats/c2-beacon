#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_int_from_json(char* json_s, char* key) {
    cJSON* json = cJSON_Parse(json_s);
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
    cJSON* item = cJSON_GetObjectItem(json, "id");
    if (!cJSON_IsNumber(item)) {
        fprintf(stderr, "cJSON_GetObjectItem() failed");
        exit(EXIT_FAILURE);
    }
    return item->valueint;
}

