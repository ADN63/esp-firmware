#pragma once
#ifndef _AwotFirmware_API_H
#define _AwotFirmware_API_H
class AwotFirmwareAPI
{
private:
    /* data */
    static void SetHeaders(Response &res);

public:
    AwotFirmwareAPI(/* args */);
    ~AwotFirmwareAPI();
    static void ApiCors(Request &req, Response &res);
    static void ApiResponse(Response &res, HTTP_CODE code);
    static void ApiResponseFactoryReset(Request &req, Response &res);
    static void ApiResponseFromConfigJsonFiltered(Request &req, Response &res, const char *);
    static void ApiResponseNoAuth(Request &req, Response &res);
    static void ApiResponseBadUser(Request &req, Response &res);
    static void ApiResponseBadPassword(Request &req, Response &res);
    static void ApiResponseAuth(Request &req, Response &res);
    static void ApiResponseSendConfigJson(Request &req, Response &res);
    static void ApiResponseSendNetworksJson(Request &req, Response &res);
    static void ApiResponseSystemJson(Request &req, Response &res);
    static void ApiMiddleWare(Request &req, Response &res);
    static void ApiEndWare(Request &req, Response &res);
    static void ApiScanNetwork(Request &req, Response &res);
};

AwotFirmwareAPI::AwotFirmwareAPI(/* args */)
{
}

AwotFirmwareAPI::~AwotFirmwareAPI()
{
}

inline void AwotFirmwareAPI::ApiCors(Request &req, Response &res)
{
    res.set(header_origin, header_value_all);
    res.set(header_method, header_value_all);
    res.set(header_header, header_value_all);
}

inline void AwotFirmwareAPI::ApiResponse(Response &res, HTTP_CODE code)
{
    res.sendStatus(code);
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseFactoryReset(Request &req, Response &res)
{
    AwotFirmwareAPI::ApiResponse(res, HTTP_SUCCESS);
    AwotFirmwareConfig::FactoryReset();
}

inline void AwotFirmwareAPI::ApiResponseFromConfigJsonFiltered(Request &req, Response &res, const char *filter)
{
    AwotFirmwareAPI::SetHeaders(res);
    WriteBufferingStream out(req, 64);
    serializeJson(config_json[filter], out);
    out.flush();
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseNoAuth(Request &req, Response &res)
{

    StaticJsonDocument<64> doc;
    doc[ft_type] = ft_noAuth;

    AwotFirmwareAPI::SetHeaders(res);

    WriteBufferingStream out(req, 64);
    serializeJson(doc, out);
    out.flush();
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseBadUser(Request &req, Response &res)
{
    StaticJsonDocument<64> doc;
    doc[ft_type] = ft_badUser;

    AwotFirmwareAPI::SetHeaders(res);

    WriteBufferingStream out(req, 64);
    serializeJson(doc, out);
    out.flush();
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseBadPassword(Request &req, Response &res)
{
    StaticJsonDocument<64> doc;
    doc[ft_type] = ft_badPassWord;

    AwotFirmwareAPI::SetHeaders(res);

    WriteBufferingStream out(req, 64);
    serializeJson(doc, out);
    out.flush();
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseAuth(Request &req, Response &res)
{
    StaticJsonDocument<48> doc;
    doc[ft_username] = loggedUser.GetUser().username;
    AwotFirmwareAPI::SetHeaders(res);

    WriteBufferingStream out(req, 64);
    serializeJson(doc, out);
    out.flush();
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseSendConfigJson(Request &req, Response &res)
{
    AwotFirmwareAPI::SetHeaders(res);

    WriteBufferingStream out(req, 64);
    serializeJson(config_json, out);
    out.flush();
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseSendNetworksJson(Request &req, Response &res)
{
    AwotFirmwareAPI::SetHeaders(res);

    WriteBufferingStream out(req, 64);
    serializeJson(networks_json, out);
    out.flush();
    res.end();
}

inline void AwotFirmwareAPI::ApiResponseSystemJson(Request &req, Response &res)
{
    return AwotFirmwareAPI::ApiResponseFromConfigJsonFiltered(req, res, api_system);
}

inline void AwotFirmwareAPI::ApiMiddleWare(Request &req, Response &res)
{
    AwotFirmwareAPI::ApiCors(req, res);

    if (req.method() == METHOD::OPTIONS)
    {
        res.set(header_accept, header_value_all);
        res.end();
    }
}

inline void AwotFirmwareAPI::ApiEndWare(Request &req, Response &res)
{
    String table_route[MAX_INPUT];
    char routeBuffer[MAX_INPUT];

    for (size_t i = 0; i < sizeT(table_route); i++)
    {
        if (req.route(i, routeBuffer, MAX_INPUT))
        {
            table_route[i] = routeBuffer;
        }

        if (table_route[i].length() > 0)
        {
            if (!strcmp(table_route[i].c_str(), "generate_204"))
            {
                if (!res.statusSent())
                {
                    cout << F("End Ware") << endl;
                    res.set(header_location, redirectURL);
                    return res.sendStatus(302);
                }
            }

            cout << table_route[i].c_str() << endl;
        }
    }
}

inline void AwotFirmwareAPI::ApiScanNetwork(Request &req, Response &res)
{
    switch (req.method())
    {
    case METHOD::GET:
        return AwotFirmwareAPI::ApiResponseSendNetworksJson(req, res);
        /* code */
        break;
    case METHOD::PUT:
        ShouldRestart = true;
        return AwotFirmwareAPI::ApiResponse(res, HTTP_SUCCESS);
        break;

    default:
        return AwotFirmwareAPI::ApiResponse(res, HTTP_BAD_REQ);
        break;
    }
}

inline void AwotFirmwareAPI::SetHeaders(Response &res)
{
    res.set(header_contentType, header_contentType_value_json);
}

#endif