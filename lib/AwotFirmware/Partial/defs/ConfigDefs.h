#pragma once
#ifndef _CONFIG_PARTIAL_BASE_H
#define _CONFIG_PARTIAL_BASE_H
//10
char static_WebFile_Path[13];
P(p_static_WebFile_Path) = "/static.bin";

char static_ConfigFile_Path[14];
P(p_static_ConfigFile_Path) = "/configs.json";

char static_NetworksFile_Path[16];
P(p_static_NetworksFile_Path) = "/networks.json";

char secretAdmin_username[15];
P(p_secretAdmin_username) = FACTORY_ADMIN_USERNAME;

char secretAdmin_password[7];
P(p_secretAdmin_password) = FACTORY_ADMIN_PASSWORD;

char user_username[12];
P(p_user_username) = "utilisateur";

char factory_ssid[MAX_INPUT];
P(p_factory_ssid) = FACTORY_WIFI_SSID;

char factory_pswd[MAX_INPUT];
P(p_factory_pswd) = FACTORY_WIFI_PASSWORD;

char factory_hotSpot_ssid[MAX_INPUT];
P(p_factory_hotSpot_ssid) = FACTORY_AP_SSID;

char factory_hotSpot_pswd[MAX_INPUT];
P(p_factory_hotSpot_pswd) = FACTORY_AP_PASSWORD;

char factory_ota_hostname[MAX_INPUT];
P(p_factory_ota_hostname) = FACTORY_MDNS_HOSTNAME;

char factory_ota_password[MAX_INPUT];
P(p_factory_ota_password) = FACTORY_OTA_PASSWORD;

int factory_ota_port = FACTORY_OTA_PORT;

char ft_sketch[8];
P(p_ft_sketch) =  "sketch";

char ft_filesystem[12];
P(p_ft_filesystem) = "filesystem";

char ft_version[12];
P(p_ft_version) = "version";

char ft_board_esp8266[9];
P(p_ft_board_esp8266) = "ESP8266";

char ft_board_esp32[7];
P(p_ft_board_esp32) = "ESP32";


#endif
