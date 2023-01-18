#include "wifi_credentials.h"


const int EEPROM_SIZE = 512;


std::tuple<String, String> readWiFiCredentials() {
    String ssid = "";
    int i = 0;
    char c = EEPROM.read(i);
    while(c != '\0' && i < EEPROM_SIZE) {
        ssid += c;
        i++;
        c = EEPROM.read(i);
    }
    i++;
    String pass = "";
    c = EEPROM.read(i);
    while(c != '\0' && i < EEPROM_SIZE) {
        pass += c;
        i++;
        c = EEPROM.read(i);
    }
    return std::make_tuple(ssid, pass);
}


void writeWiFiCredentials(String ssid, String pass) {
    int i = 0;
    for (i = 0; i < ssid.length(); i++) {
        EEPROM.put(i, ssid[i]);
    }
    EEPROM.put(i,'\0');
    i++;
    for (int j = 0; j < pass.length(); j++,i++) {
        EEPROM.put(i, pass[j]);
    }
    EEPROM.put(i,'\0');
    EEPROM.commit();
}

