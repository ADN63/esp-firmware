#pragma once
#ifndef _AwotFirmware_DEFS_H
#define _AwotFirmware_DEFS_H
#include <Arduino.h>

#define USE_SD FEATURES_SD == = 1 ? true : false
#define SD_FAT_TYPE 3
#define CS_PIN FACTORY_PIN_SDCS
#define READ_BUFFER_SIZE 128
#define JSON_SIZE 1024
#define sizeT(x) (sizeof(x) / sizeof(x[0]))

#define MAX_FEATURES 4
#define MAX_USERS 1
#define MAX_WIFI_MODE 2
#define MAX_INPUT 50
#define INTERVAL_RESTART 1000

#if defined(ESP32)
#include <WiFi.h>
#include <WiFiServer.h>

#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#error("Unknow Board")
#endif
#include <ArduinoOTA.h>
#include <aWOT.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"

#if SD_FAT_TYPE == 0
#error("Use type 3 INSTEAD")
#elif SD_FAT_TYPE == 1
SdFat32 sd;
FsFile configFile;
File32 webFile;
#elif SD_FAT_TYPE == 2
SdExFat sd;
FsFile configFile;
ExFile webFile;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile webFile;
// FsFile configFile;
// FsFile networksFile;
#else // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif // SD_FAT_TYPE

IPAddress ap_local_IP(192,168,4,2);
IPAddress ap_gateway(192,168,4,1);
IPAddress ap_subnet(255,255,255,0);

IPAddress sta_local_IP(192,168,1,154);
IPAddress sta_gateway(192,168,1,254);
IPAddress sta_subnet(255,255,255,0);

WiFiServer server(80);
Application app;
WiFiClient client;
DNSServer dnsServer;
char redirectURL[50];
ArduinoOutStream cout(Serial);
// input buffer for line
char cinBuf[50];
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));

byte readBuffer[READ_BUFFER_SIZE];
StaticJsonDocument<JSON_SIZE> config_json;
StaticJsonDocument<JSON_SIZE> networks_json;
bool NoSTA, ShouldRestart, NoSdMount, NoConfigFile, NoWebFile = false;

static unsigned long RESTART_TIMER = 0;
static int RESTART_COUNTER = 0;

#include "Partial/Partial.h"
#include "Partial/classes/Feature.h"
#include "Partial/classes/User.h"
#include "Partial/classes/CurrentUser.h"
#include "Partial/classes/MyWiFi.h"
#include "Partial/classes/Ota.h"
#include "Partial/routes/Routes.h"

CurrentUser loggedUser;
class AwotFirmwareDefs
{
private:
    /* data */
public:
    static void Initialise_PTR();
    AwotFirmwareDefs(/* args */);
    ~AwotFirmwareDefs();
};

void AwotFirmwareDefs::Initialise_PTR()
{
    sprintf_P(static_WebFile_Path, PSTR("%S"), p_static_WebFile_Path);
    sprintf_P(static_ConfigFile_Path, PSTR("%S"), p_static_ConfigFile_Path);
    sprintf_P(static_NetworksFile_Path, PSTR("%S"), p_static_NetworksFile_Path);
    
    sprintf_P(secretAdmin_username, PSTR("%S"), p_secretAdmin_username);
    sprintf_P(secretAdmin_password, PSTR("%S"), p_secretAdmin_password);
    sprintf_P(user_username, PSTR("%S"), p_user_username);

    sprintf_P(factory_ssid, PSTR("%S"), p_factory_ssid);
    sprintf_P(factory_pswd, PSTR("%S"), p_factory_pswd);
    sprintf_P(factory_hotSpot_ssid, PSTR("%S"), p_factory_hotSpot_ssid);
    sprintf_P(factory_hotSpot_pswd, PSTR("%S"), p_factory_hotSpot_pswd);
    sprintf_P(factory_ota_hostname, PSTR("%S"), p_factory_ota_hostname);
    sprintf_P(factory_ota_password, PSTR("%S"), p_factory_ota_password);

    sprintf_P(ft_authStrict, PSTR("%S"), p_ft_authStrict);
    sprintf_P(ft_fileUpload, PSTR("%S"), p_ft_fileUpload);
    sprintf_P(ft_ota, PSTR("%S"), p_ft_ota);
    sprintf_P(ft_name, PSTR("%S"), p_ft_name);
    sprintf_P(ft_active, PSTR("%S"), p_ft_active);
    sprintf_P(ft_username, PSTR("%S"), p_ft_username);
    sprintf_P(ft_password, PSTR("%S"), p_ft_password);
    sprintf_P(ft_type, PSTR("%S"), p_ft_mywifitype);
    sprintf_P(ft_ssid, PSTR("%S"), p_ft_ssid);
    sprintf_P(ft_port, PSTR("%S"), p_ft_port);
    sprintf_P(ft_ap, PSTR("%S"), p_ft_ap);
    sprintf_P(ft_sta, PSTR("%S"), p_ft_sta);
    sprintf_P(ft_badPassWord, PSTR("%S"), p_ft_badPassWord);
    sprintf_P(ft_badUser, PSTR("%S"), p_ft_badUser);
    sprintf_P(ft_noAuth, PSTR("%S"), p_ft_noAuth);
    sprintf_P(ft_hostname, PSTR("%S"), p_ft_hostname);
    sprintf_P(ft_status, PSTR("%S"), p_ft_status);
    sprintf_P(ft_channel, PSTR("%S"), p_ft_channel);
    sprintf_P(ft_bssid, PSTR("%S"), p_ft_bssid);
    sprintf_P(ft_rssi, PSTR("%S"), p_ft_rssi);
    sprintf_P(space, PSTR("%S"), p_space);
    sprintf_P(null, PSTR("%S"), p_null);

    sprintf_P(header_connection, PSTR("%S"), p_header_connection);
    sprintf_P(header_close, PSTR("%S"), p_header_close);
    sprintf_P(header_accept, PSTR("%S"), p_header_accept);
    sprintf_P(header_location, PSTR("%S"), p_header_location);
    sprintf_P(header_value_all, PSTR("%S"), p_header_value_all);
    sprintf_P(header_origin, PSTR("%S"), p_header_origin);
    sprintf_P(header_method, PSTR("%S"), p_header_method);
    sprintf_P(header_header, PSTR("%S"), p_header_header);
    sprintf_P(header_contentType, PSTR("%S"), p_header_contentType);
    sprintf_P(header_contentType_value_json, PSTR("%S"), p_header_contentType_value_json);

    sprintf_P(api_features, PSTR("%S"), p_api_features);
    sprintf_P(api_auth, PSTR("%S"), p_api_auth);
    sprintf_P(api_wifi, PSTR("%S"), p_api_wifi);
    sprintf_P(api_signin, PSTR("%S"), p_api_signin);
    sprintf_P(api_ota, PSTR("%S"), p_api_ota);
    sprintf_P(api_system, PSTR("%S"), p_api_system);

    sprintf_P(route_api_config, PSTR("%S"), p_route_api_config);
    sprintf_P(route_api_feature, PSTR("%S"), p_route_api_feature);
    sprintf_P(route_api_auth, PSTR("%S"), p_route_api_auth);
    sprintf_P(route_api_auth_signin, PSTR("%S"), p_route_api_auth_signin);
    sprintf_P(route_api_auth_signout, PSTR("%S"), p_route_api_auth_signout);
    sprintf_P(route_api_generate_204, PSTR("%S"), p_route_api_generate_204);
    sprintf_P(route_api_networks, PSTR("%S"), p_route_api_networks);
    sprintf_P(route_api_wifi, PSTR("%S"), p_route_api_wifi);
    sprintf_P(route_api_ota, PSTR("%S"), p_route_api_ota);
    sprintf_P(route_api_factory_reset, PSTR("%S"), p_route_api_factory_reset);
    sprintf_P(route_api_system, PSTR("%S"), p_route_api_system);

    sprintf(ft_sketch, PSTR("%S"), p_ft_sketch);
    sprintf(ft_filesystem, PSTR("%S"), p_ft_filesystem);
    sprintf(ft_version, PSTR("%S"), p_ft_version);
    sprintf(ft_board_esp8266, PSTR("%S"), p_ft_board_esp8266);
    sprintf(ft_board_esp32, PSTR("%S"), p_ft_board_esp32);



}

AwotFirmwareDefs::AwotFirmwareDefs()
{
}

AwotFirmwareDefs::~AwotFirmwareDefs()
{
}

#endif