#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
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

            WiFi.addAP(ssid.c_str(), password.c_str());
            int times = 0;
            while (WiFi.run() != WL_CONNECTED) {
                times++;
                if (times > 30) {
                    server->send(200, "text/html", "Could not Connect");
                    return;
                }
                Serial.println(".");
                delay(100);
            }
            Serial.println(WiFi.localIP());

            change_wifi_request data;
            data.ssid = ssid;

            HTTPClient http;

            http.begin("https://google.com/");
            // http.addHeader("Content-Type", "application/json");

            int responseCode = http.GET();//.POST('{}');
            String response = responses.change_wifi(data);
            response += "<h1>" + String(http.getString()) + "</h1>";


            server->send(200, "text/html", response.c_str());

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
