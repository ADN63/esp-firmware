#pragma once
#ifndef _OTA_API_H
#define _OTA_API_H
class OtaApi
{
private:
    /* data */
public:
    OtaApi(/* args */);
    ~OtaApi();
    static void PutHandlerOtaApi(Request &req, Response &res);
};

OtaApi::OtaApi(/* args */)
{
}

OtaApi::~OtaApi()
{
}

inline void OtaApi::PutHandlerOtaApi(Request &req, Response &res)
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
    AwotFirmwareAPI::ApiResponse(res, HTTP_SUCCESS);

    const char *hostname = docInput[ft_hostname];
    const char *password = docInput[ft_password];
    int port = docInput[ft_port];

    for (JsonObject ota : config_json[api_ota].as<JsonArray>())
    {
        ota[ft_hostname] = hostname;
        ota[ft_password] = password;
        ota[ft_port] = port;
    }

    sd.remove(static_ConfigFile_Path);
    auto file = sd.open(static_ConfigFile_Path, FILE_WRITE);
    WriteBufferingStream debug(file, 64);
    serializeJsonPretty(config_json, file);
    debug.flush();
    file.close();
    cout << F("Ota : ") << hostname << space << port << space << F("With Password ") << password << endl;

    AwotFirmwareConfig::LoadConfiguration();
    ShouldRestart = true;

    return AwotFirmwareAPI::ApiResponse(res, HTTP_SUCCESS);
}

#endif