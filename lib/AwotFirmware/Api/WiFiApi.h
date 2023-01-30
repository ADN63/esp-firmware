#pragma once
#ifndef _WIFI_API_H
#define _WIFI_API_H
class WiFiApi
{
private:
    /* data */
public:
    WiFiApi(/* args */);
    ~WiFiApi();
    static void GetHandlerWiFiApi(Request &req, Response &res);
    static void PutHandlerFeatureApi(Request &req, Response &res);
};

WiFiApi::WiFiApi(/* args */)
{
}

WiFiApi::~WiFiApi()
{
}

inline void WiFiApi::GetHandlerWiFiApi(Request &req, Response &res)
{
    return AwotFirmwareAPI::ApiResponseFromConfigJsonFiltered(req, res, api_wifi);
}

inline void WiFiApi::PutHandlerFeatureApi(Request &req, Response &res)
{
    ReadBufferingStream input(req, 64);
    StaticJsonDocument<JSON_SIZE> docInput;
    DeserializationError errorInput = deserializeJson(docInput, input);

    if (errorInput)
    {
        cout << F("deserializeJson() failed: ") << endl;
        cout << errorInput.f_str() << endl;
        return AwotFirmwareAPI::ApiResponse(res, HTTP_BAD_REQ);
    }

    const char *type = docInput[ft_type];
    const char *ssid = docInput[ft_ssid];
    const char *password = docInput[ft_password];

    bool validSubmit = false;
    for (JsonObject mywifi : config_json[api_wifi].as<JsonArray>())
    {
        const char *ctype = mywifi[ft_type];
        if (!strcmp(type, ctype))
        {
            mywifi[ft_type] = type;
            mywifi[ft_ssid] = ssid;
            mywifi[ft_password] = password;

            validSubmit = !validSubmit;
        }
    }
    if (validSubmit)
    {
        sd.remove(static_ConfigFile_Path);
        auto file = sd.open(static_ConfigFile_Path, FILE_WRITE);
        WriteBufferingStream debug(file, 64);
        serializeJsonPretty(config_json, file);
        debug.flush();
        file.close();
        cout << F("Wifi  ") << type << F(" Toogle ") << ssid << F("With Password ") << password << endl;

        AwotFirmwareConfig::LoadConfiguration();
        ShouldRestart = true;

        return AwotFirmwareAPI::ApiResponse(res, HTTP_SUCCESS);
    }
    else
    {
        return AwotFirmwareAPI::ApiResponse(res, HTTP_BAD_REQ);
    }
}

#endif