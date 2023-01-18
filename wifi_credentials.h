#include <Arduino.h>
#include <EEPROM.h>
#include <tuple>



std::tuple<String, String> readWiFiCredentials();
void writeWiFiCredentials(String ssid, String pass);

