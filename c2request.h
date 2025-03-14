#include <stddef.h>

struct string {
    char* ptr;
    size_t len;
};

void init_string(struct string* s);
size_t writefunc(void* ptr, size_t size, size_t nmemb, struct string* s);

char* request_post(char* url, char* data);
char* request_get(char url[]);
