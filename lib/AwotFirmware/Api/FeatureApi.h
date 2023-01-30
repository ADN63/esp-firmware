#pragma once
#ifndef _FEATURE_API_H
#define _FEATURE_API_H

class FeaturesApi
{
private:
    /* data */
public:
    FeaturesApi(/* args */);
    ~FeaturesApi();
    static void GetHandlerFeatureApi(Request &req, Response &res);
    static void PutHandlerFeatureApi(Request &req, Response &res);
};

FeaturesApi::FeaturesApi(/* args */)
{
}

FeaturesApi::~FeaturesApi()
{
}

inline void FeaturesApi::GetHandlerFeatureApi(Request &req, Response &res)
{
    return AwotFirmwareAPI::ApiResponseFromConfigJsonFiltered(req, res, api_features);
}

inline void FeaturesApi::PutHandlerFeatureApi(Request &req, Response &res)
{

    ReadBufferingStream input(req, 64);
    StaticJsonDocument<96> docInput;
    DeserializationError errorInput = deserializeJson(docInput, input);

    if (errorInput)
    {
        cout << F("deserializeJson() failed: ") << endl;
        cout << errorInput.f_str() << endl;
        return AwotFirmwareAPI::ApiResponse(res, HTTP_BAD_REQ);
    }

    const char *inputName = docInput[ft_name]; // "ft_authStrict"
    bool inputValue = docInput[ft_active];

    bool validSubmit = false;

    for (JsonObject feature : config_json[api_features].as<JsonArray>())
    {

        const char *feature_name = feature[ft_name]; // "ft_authStrict", "ft_fileUpload", "ft_ota"
        bool feature_value = feature[ft_active];     // true, true, true

        if (!strcmp(feature_name, inputName))
        {
            if (feature_value != inputValue)
            {
                feature[ft_active] = inputValue;
                validSubmit = !validSubmit;
            }
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
        cout << F("Feature ") << inputName << F(" Toogle ") << inputValue << endl;

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
