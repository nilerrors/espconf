#include <Arduino.h>
#include "network.h"


typedef struct {
    String title;
    String content;
} request_error;

typedef struct {
    String ssid;
} change_wifi_request;


class Responses
{
private:
    const char* base(String body, String title, bool styles);
public:
    const char* root(int len, network networks[]);
    const char* change_wifi(request_error error);
    const char* change_wifi(change_wifi_request data);
    const char* not_found();
};
