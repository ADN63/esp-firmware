#pragma once
#ifndef _AwotFirmware_CONFIG_H
#define _AwotFirmware_CONFIG_H
class AwotFirmwareConfig
{
private:
    /* data */
public:
    static bool MakeFactoryConfig();
    static bool Begin();
    static bool LoadConfiguration();
    static bool FactoryReset();
    static void RestartBoard();
    static bool VerifyConfig();
    AwotFirmwareConfig(/* args */);
    ~AwotFirmwareConfig();
};

inline bool AwotFirmwareConfig::MakeFactoryConfig()
{
    JsonArray features = config_json.createNestedArray(api_features);
    JsonObject features_0 = features.createNestedObject();
    features_0[ft_name] = ft_authStrict;
    features_0[ft_active] = 1;

    JsonObject features_1 = features.createNestedObject();
    features_1[ft_name] = ft_fileUpload;
    features_1[ft_active] = 1;

    JsonObject features_2 = features.createNestedObject();
    features_2[ft_name] = ft_ota;
    features_2[ft_active] = 1;

    JsonArray users = config_json.createNestedArray(api_auth);

    JsonObject users_0 = users.createNestedObject();
    users_0[ft_username] = secretAdmin_username;
    users_0[ft_password] = secretAdmin_password;

    JsonObject users_1 = users.createNestedObject();
    users_1[ft_username] = user_username;
    users_1[ft_password] = null;

    JsonArray wifi = config_json.createNestedArray(api_wifi);

    JsonObject wifi_0 = wifi.createNestedObject();
    wifi_0[ft_type] = ft_ap;
    wifi_0[ft_ssid] = factory_hotSpot_ssid;
    wifi_0[ft_password] = factory_hotSpot_pswd;

    JsonObject wifi_1 = wifi.createNestedObject();
    wifi_1[ft_type] = ft_sta;
    wifi_1[ft_ssid] = factory_ssid;
    wifi_1[ft_password] = factory_pswd;

    JsonObject ota = config_json.createNestedObject(api_ota);
    ota[ft_hostname] = factory_ota_hostname;
    ota[ft_password] = factory_ota_password;
    ota[ft_port] = factory_ota_port;

    JsonObject system = config_json.createNestedObject(api_system);
    system[ft_version] = FACTORY_VERSION;
    system[ft_type] = ESP8266 ? ft_board_esp8266 : ft_board_esp32;

    auto file = sd.open(static_ConfigFile_Path, FILE_WRITE);
    WriteBufferingStream output(file, 96);
    serializeJsonPretty(config_json, output);
    output.flush();
    file.close();

    return true;
}

inline bool AwotFirmwareConfig::Begin()
{
    if (NoSdMount)
    {
        cout << F("Implementation Sans SD Manquante \n");
        return false;
    }
    if (!sd.exists(static_ConfigFile_Path))
    {
        AwotFirmwareConfig::MakeFactoryConfig();
    }

    if (!AwotFirmwareConfig::LoadConfiguration())
    {
        sd.remove(static_ConfigFile_Path);
        AwotFirmwareConfig::RestartBoard();
        return false;
    }

    if (!AwotFirmwareConfig::VerifyConfig())
    {
        sd.remove(static_ConfigFile_Path);
        AwotFirmwareConfig::RestartBoard();
        return false;
    }

    return true;
}
inline bool AwotFirmwareConfig::VerifyConfig()
{
    cout << F("Verification Configurations : ") << endl;
    auto features = config_json[api_features].as<JsonArray>();
    auto auth = config_json[api_auth].as<JsonArray>();
    auto wifi = config_json[api_wifi].as<JsonArray>();
    auto ota = config_json[api_ota].as<JsonObject>();
    auto system = config_json[api_system].as<JsonObject>();

    Serial.println(features.isNull() ? F("Erreur Feature") : F("Features OK !"));
    Serial.println(auth.isNull() ? F("Erreur Auth") : F("Auth OK !"));
    Serial.println(wifi.isNull() ? F("Erreur WiFi") : F("WiFi OK !"));
    Serial.println(ota.isNull() ? F("Erreur Ota") : F("Ota OK !"));
    Serial.println(system.isNull() ? F("Erreur System") : F("System OK !"));

    return true;
}
inline bool AwotFirmwareConfig::LoadConfiguration()
{
    auto configFile = sd.open(static_ConfigFile_Path);

    DeserializationError errorConfig = deserializeJson(config_json, configFile);
    if (errorConfig)
    {
        cout << F("Erreur de deserialisation configs.json") << endl;
        sd.remove(static_ConfigFile_Path);
        return false;
    }

    return true;
}

inline bool AwotFirmwareConfig::FactoryReset()
{
    if (sd.exists(static_ConfigFile_Path))
        sd.remove(static_ConfigFile_Path);

    ShouldRestart = true;
    return ShouldRestart;
}

inline void AwotFirmwareConfig::RestartBoard()
{

    cout << F("Board while be restart in ") << RESTART_COUNTER;
    while (true)
    {

        if (millis() - RESTART_TIMER > INTERVAL_RESTART)
        {
            RESTART_TIMER = millis();
            RESTART_COUNTER++;
            cout << space << RESTART_COUNTER;
        }

        if (RESTART_COUNTER == 3)
        {
            if (client)
                client.stop();

            ESP.restart();
        }
    }
}

AwotFirmwareConfig::AwotFirmwareConfig(/* args */)
{
}

AwotFirmwareConfig::~AwotFirmwareConfig()
{
}

#endif