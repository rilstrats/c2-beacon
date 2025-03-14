#include <stdio.h>
#include "c2json.h"
#include "c2request.h"

int main(int argc, char* argv[])
{
    char* data = "{\"ip\": \"192.168.100.1\","
                 "\"hostname\": \"c2-beacon-test1\"}";
    char* response = request_post("http://0.0.0.0:8080/beacon/register", data);

    int id = get_int_from_json(response, "id");
    char* url;
    asprintf(&url, "http://0.0.0.0:8080/beacon/%d", id);
    request_get(url);
    return 0;
}
