#include "wifi_credentials.h"


const int EEPROM_SIZE = 512;

Preferences preferences;

void readWiFiCredentials(String &ssid, String &password) {
    preferences.begin("wifi", false);
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");
    preferences.end();
}


void writeWiFiCredentials(String ssid, String pass) {
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("pass", pass);
    preferences.end();
}

