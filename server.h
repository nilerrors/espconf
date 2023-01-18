#include <WebServer.h>


class ConfigServer {
private:
    WebServer* server;
    void createServer();
public:
    ConfigServer();
    void begin();
    void handleClient();
    void end();
    bool ended();
};

