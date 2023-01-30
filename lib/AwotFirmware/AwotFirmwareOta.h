#pragma once
#ifndef _AwotFirmware_OTA_H
#define _AwotFirmware_OTA_H
class AwotFirmwareOta
{
private:
  /* data */
  static void SetConfigiration(int port, const char *hostname, const char *password = null);

public:
  static bool Begin();
  AwotFirmwareOta(/* args */);
  ~AwotFirmwareOta();
};

inline void AwotFirmwareOta::SetConfigiration(int port, const char *hostname, const char *password)
{

  ArduinoOTA.setPort(port);
  ArduinoOTA.setHostname(hostname);
  if (!strcmp(password, null))
  {
    ArduinoOTA.setPassword(NULL);
  }
  else
  {
    ArduinoOTA.setPassword(password);
  }
}

inline bool AwotFirmwareOta::Begin()
{
  if (!NoSdMount)
  {
    auto ota = config_json[api_ota].as<JsonObject>();

    const char *ota_hostname = ota[ft_hostname]; // "adn"
    const char *ota_password = ota[ft_password]; // "adnota"
    int ota_port = ota[ft_port];                 // 8266
    SetConfigiration(ota_port, ota_hostname, ota_password);
  }
  else
  {
    SetConfigiration(factory_ota_port, factory_ota_hostname, factory_ota_password);
  }

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = ft_sketch;
    } else {  // U_FS
      type = ft_filesystem;
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    cout << F("Start updating ") << type << endl; });
  ArduinoOTA.onEnd([]()
                   { cout << F("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    cout << F("Error") << error << endl;
    
    if (error == OTA_AUTH_ERROR) {
      cout << F("Auth Failed") << endl;
    } else if (error == OTA_BEGIN_ERROR) {
      cout << F("Begin Failed") << endl;
    } else if (error == OTA_CONNECT_ERROR) {
      cout << F("Connect Failed") << endl;
    } else if (error == OTA_RECEIVE_ERROR) {
      cout << F("Receive Failed") << endl;
    } else if (error == OTA_END_ERROR) {
      cout << F("End Failed") << endl;
    } });
  ArduinoOTA.begin();

  return true;
}

AwotFirmwareOta::AwotFirmwareOta(/* args */)
{
}

AwotFirmwareOta::~AwotFirmwareOta()
{
}

#endif