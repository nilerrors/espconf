#include <WebServer.h>
#include <WiFiMulti.h>


class ConfigServer {
private:
    bool running = false;
    WebServer* server;
    void createServer();
public:
    ConfigServer();
    void begin();
    void handleClient();
    void end();
    bool ended();
};

