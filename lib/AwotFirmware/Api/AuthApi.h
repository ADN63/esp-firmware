#pragma once
#ifndef _AUTH_API_H
#define _AUTH_API_H

class AuthApi
{
private:
    /* data */
public:
    static void HandleEndpoint(Request &req, Response &res);
    static void HandleLoggedIn(Request &req, Response &res);
    static void HandleLoggedOut(Request &req, Response &res);
    AuthApi(/* args */);
    ~AuthApi();
};

inline void AuthApi::HandleLoggedIn(Request &req, Response &res)
{
    ReadBufferingStream input(req, 64);
    StaticJsonDocument<96> doc;
    DeserializationError error = deserializeJson(doc, input);
    input.flush();

    if (error)
    {
        cout << F("deserializeJson() failed: ") << endl;
        cout << error.f_str() << endl;
        return AwotFirmwareAPI::ApiResponse(res, HTTP_BAD_REQ);
    }

    const char *username = doc[ft_username]; // "123456789"
    const char *password = doc[ft_password]; // "123456789"

    bool validSubmit = false;

    for (JsonObject auth : config_json[api_auth].as<JsonArray>())
    {
        const char *auth_username = auth[ft_username];
        const char *auth_password = auth[ft_password];

        if (!strcmp(auth_username, username))
        {
            if (!strcmp(auth_password, password))
            {
                validSubmit = true;
            }
            else
            {
                return AwotFirmwareAPI::ApiResponseBadPassword(req, res);
            }
        }
        else
        {
            return AwotFirmwareAPI::ApiResponseBadUser(req, res);
        }
    }

    if (validSubmit)
    {
        loggedUser.LoggedIn(User(username, password));
        return AwotFirmwareAPI::ApiResponseAuth(req, res);
    }

    return AwotFirmwareAPI::ApiResponseNoAuth(req, res);
}

inline void AuthApi::HandleLoggedOut(Request &req, Response &res)
{
    cout << F("HandleLoggOut") << endl;

    ReadBufferingStream spfs(req, 64);
    StaticJsonDocument<96> doc;
    DeserializationError error = deserializeJson(doc, spfs);

    if (error)
    {
        cout << F("deserializeJson() failed: ") << endl;
        cout << error.f_str() << endl;
        return AwotFirmwareAPI::ApiResponse(res, HTTP_BAD_REQ);
    }

    const char *username = doc[ft_username]; // "123456789"
    const char *password = doc[ft_password]; // "123456789"

    if (strcmp(username, loggedUser.GetUser().username))
    {
        cout << ft_badUser << endl;
        return AwotFirmwareAPI::ApiResponseBadUser(req, res);
    }
    if (strcmp(password, loggedUser.GetUser().password))
    {
        cout << ft_badPassWord << endl;
        return AwotFirmwareAPI::ApiResponseBadPassword(req, res);
    }
    loggedUser.LoggedOut();

    return AwotFirmwareAPI::ApiResponse(res, HTTP_SUCCESS);
}

inline void AuthApi::HandleEndpoint(Request &req, Response &res)
{

    if (loggedUser.IsUserLogged())
    {
        return AwotFirmwareAPI::ApiResponseAuth(req, res);
    }
    else
        return AwotFirmwareAPI::ApiResponse(res, HTTP_NO_AUTH);
}

AuthApi::AuthApi(/* args */)
{
}

AuthApi::~AuthApi()
{
}

#endif
