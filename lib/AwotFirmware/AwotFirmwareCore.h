#pragma once
#ifndef _AwotFirmware_CORE_H
#define _AwotFirmware_CORE_H
#include "AwotFirmwareDefs.h"
#include "AwotFirmwareSD.h"
#include "AwotFirmwareConfig.h"
#include "AwotFirmwareWiFi.h"
#include "AwotFirmwareAPI.h"
#include "AwotFirmwareOta.h"
#include "Api/AuthApi.h"
#include "Api/FeatureApi.h"
#include "Api/OtaApi.h"
#include "Api/WiFiApi.h"

class AwotFirmwareCore
{
private:
    /* data */
    static void BeginWithSd();

public:
    static void Begin();
    static void Loop();
    AwotFirmwareCore(/* args */);
    ~AwotFirmwareCore();
};

inline void AwotFirmwareCore::Begin()
{
    AwotFirmwareDefs::Initialise_PTR();
    AwotFirmwareSD::Begin();
    AwotFirmwareConfig::Begin();
    
    cout << F("Configuration Terminer ! ") << (NoSdMount ? F("SD Unmount") : F("SD Mount")) << endl;

    if (!NoSdMount)
    {
        AwotFirmwareWiFi::ScanNetwork();
        AwotFirmwareWiFi::Begin();
        AwotFirmwareOta::Begin();
        AwotFirmwareCore::BeginWithSd();
    }

    static unsigned long blockTimer = 0;

    while (NoSdMount)
    {
        if (millis() - blockTimer > 2000)
        {
            cout << F("Setup Freeze Because NoSdMount Implementation is incoming ") << endl;
            blockTimer = millis();
        }
        yield();
    }
}

inline void AwotFirmwareCore::BeginWithSd()
{
    app.use(&AwotFirmwareSD::FileHandler); // MUST BE FIRST OR ERROR APPEND NO FILE PROVIDER
    app.use(&AwotFirmwareAPI::ApiMiddleWare);

    app.get(route_api_factory_reset, &AwotFirmwareAPI::ApiResponseFactoryReset);
    app.get(route_api_config, &AwotFirmwareAPI::ApiResponseSendConfigJson);

    app.get(route_api_feature, &FeaturesApi::GetHandlerFeatureApi);
    app.put(route_api_feature, &FeaturesApi::PutHandlerFeatureApi);

    app.get(route_api_wifi, &WiFiApi::GetHandlerWiFiApi);
    app.put(route_api_wifi, &WiFiApi::PutHandlerFeatureApi);

    app.get(route_api_auth, &AuthApi::HandleEndpoint);
    app.put(route_api_auth_signin, &AuthApi::HandleLoggedIn);
    app.put(route_api_auth_signout, &AuthApi::HandleLoggedOut);

    app.get(route_api_networks, &AwotFirmwareAPI::ApiScanNetwork);
    app.put(route_api_networks, &AwotFirmwareAPI::ApiScanNetwork);

    app.put(route_api_ota, &OtaApi::PutHandlerOtaApi);

    app.get(route_api_system, &AwotFirmwareAPI::ApiResponseSystemJson);

    app.get(route_api_generate_204, &AwotFirmwareAPI::ApiEndWare);

    server.begin();
    cout << WiFi.getHostname() << endl;

    IPAddress ip = WiFi.softAPIP();
    dnsServer.start(53, header_value_all, ip);
    delay(50);
    sprintf(redirectURL, "http://%d.%d.%d.%d/", ip[0], ip[1], ip[2], ip[3]);

    cout << F("Server Setup End! GateWay Esp :") << ip.toString().c_str() << endl;

    if (WiFi.localIP().isSet())
    {
        cout << F("Local IP : ") << WiFi.localIP().toString().c_str() << endl;
        if (MDNS.begin(factory_ota_hostname, WiFi.localIP()))
        { // Start mDNS with name esp8266
            cout << F("MDNS Started : ") << factory_ota_hostname << endl;
        }
    }
}

inline void AwotFirmwareCore::Loop()
{
    client = server.available();

    if (ShouldRestart)
    {
        AwotFirmwareConfig::RestartBoard();
    }

    ArduinoOTA.handle();

    if (client.connected())
    {
        app.process(&client);
        // client.stop();
    }

    dnsServer.processNextRequest();
}

AwotFirmwareCore::AwotFirmwareCore(/* args */)
{
}

AwotFirmwareCore::~AwotFirmwareCore()
{
}

#endif