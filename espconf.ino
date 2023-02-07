#include <WiFi.h>
#include <WiFiMulti.h>
#include "server.h"
#include "wifi_credentials.h"


#define proto_ssid "ESPCONF"
#define proto_password "esp12345"


IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);


ConfigServer server;


void setup() {
    Serial.begin(115200);

    if (startConfigServer()) {
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
        server.begin();
        Serial.println("Done");
    }
    else {
        std::tuple<String, String> credentials = readWiFiCredentials();
        WiFi.addAP(std::get<0>(credentials).c_str(), std::get<1>(credentials).c_str());
    }
}

void loop() {
    if (startConfigServer()) {
        server.handleClient();
        return;
    }

    std::tuple<String, String> credentials = readWiFiCredentials();
    Serial.println(std::get<0>(credentials).c_str());
    Serial.println(std::get<1>(credentials).c_str());
}


bool startConfigServer() {
    std::tuple<String, String> credentials = readWiFiCredentials();
    return std::get<0>(credentials).c_str() != "" || std::get<1>(credentials).c_str() != "";
}
