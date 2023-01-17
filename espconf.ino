#include <WiFi.h>
#include <WebServer.h>
#include "res.h"

const char *proto_ssid = "ESPCONF";
const char *proto_password = "esp12345";

// char *ssid = "";
// char *password = "";


IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);
Responses responses;


void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);

    Serial.print("Setting soft-AP configuration ... ");
    bool configReady = WiFi.softAPConfig(local_IP, gateway, subnet);
    Serial.println(configReady ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    bool settingReady = WiFi.softAP(proto_ssid, proto_password);
    Serial.println(settingReady ? "Ready" : "Failed!");
    
    Serial.print("IP Address = ");
    Serial.println(WiFi.softAPIP());

    Serial.print("Setting up Web Server ... ");

    server.on("/", HTTP_GET, []() {
        int n = WiFi.scanNetworks();
        network* networks = new network[n];
        for (int i = 0; i < n; i++) {
            network netw;
            netw.ssid = WiFi.SSID(i);
            netw.open = WiFi.encryptionType(i) == WIFI_AUTH_OPEN;
            networks[i] = netw;
        }
        server.send(200, "text/html", responses.root(n, networks));
        Serial.println("Root");
    });

    server.on("/change_wifi", HTTP_POST, []() {
        if (!server.hasArg("ssid") || !server.hasArg("pass")) {
            request_error error;
            error.title = "400 Bad Request";
            error.content = "";
            server.send(400, "text/html", responses.change_wifi(error));
        }
        else {
            // Connect WiFi with network
            // ...

            change_wifi_request data;
            data.ssid = server.arg("ssid");
            server.send(200, "text/html", responses.change_wifi(data));
        }
    });

    server.onNotFound([]() {
        server.send(404, "text/html", responses.not_found());
    });

    server.begin();
    Serial.println("Done");
}

void loop() {
    server.handleClient();
    delay(2);
}
