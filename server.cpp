#include <Arduino.h>
#include <WiFi.h>
#include "server.h"
#include "res.h"
#include "wifi_credentials.h"


Responses responses;



ConfigServer::ConfigServer() {
    createServer();
}

void ConfigServer::createServer() {
    server = new WebServer();

    server->on("/", HTTP_GET, [&]() {
        int n = WiFi.scanNetworks();
        network* networks = new network[n];
        for (int i = 0; i < n; i++) {
            network netw;
            netw.ssid = WiFi.SSID(i);
            netw.open = WiFi.encryptionType(i) == WIFI_AUTH_OPEN;
            networks[i] = netw;
        }
        server->send(200, "text/html", responses.root(n, networks));
        Serial.println("Root");
    });

    server->on("/change_wifi", HTTP_POST, [&]() {
        if (!server->hasArg("ssid") || !server->hasArg("pass")) {
            request_error error;
            error.title = "400 Bad Request";
            error.content = "";
            server->send(400, "text/html", responses.change_wifi(error));
        }
        else {
            String ssid = server->arg("ssid");
            String password = server->arg("password");
            writeWiFiCredentials(ssid, password);

            WiFi.begin(ssid.c_str(), password.c_str());

            change_wifi_request data;
            data.ssid = ssid;
            server->send(200, "text/html", responses.change_wifi(data));

            end();
        }
    });

    server->onNotFound([&]() {
        server->send(404, "text/html", responses.not_found());
    });
}

void ConfigServer::begin() {
    server->begin();
}

void ConfigServer::handleClient() {
    server->handleClient();
}

void ConfigServer::end() {
    delete server;
}

bool ConfigServer::ended() {
	return false;
}
