#pragma once
#ifndef _AwotFirmware_WIFI_H
#define _AwotFirmware_WIFI_H

class AwotFirmwareWiFi
{
private:
    /* data */
    static void BeginStationRadio(const char *ssid, const char *password = "null");
    static void BeginWiFiConnection(const char *ssid, const char *password = "null");
    static void BeginBackup();

public:
    AwotFirmwareWiFi(/* args */);
    ~AwotFirmwareWiFi();
    static void Begin();
    static void ScanNetwork();
};

AwotFirmwareWiFi::AwotFirmwareWiFi(/* args */)
{
}

AwotFirmwareWiFi::~AwotFirmwareWiFi()
{
}

inline void AwotFirmwareWiFi::BeginStationRadio(const char *ssid, const char *password)
{
    cout << (WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet) ? F("Acces Point Ready\n") : F("Failed to configure Acces Point !\n"));
    if (!strcmp(password, "null"))
    {
        WiFi.softAP(ssid);
    }
    else
    {
        if (!strcmp(ssid, "null"))
        {
            if (strlen(ssid) > 0)
                WiFi.softAP(ssid, password);
        }
    }
}

inline void AwotFirmwareWiFi::BeginWiFiConnection(const char *ssid, const char *password)
{

    WiFi.setHostname(factory_ota_hostname);
    WiFi.config(sta_local_IP, sta_gateway, sta_subnet, sta_gateway);

    if (!strcmp(password, "null"))
    {
        WiFi.begin(ssid);
    }
    else
    {
        WiFi.begin(ssid, password);
    }

    cout << F("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        cout << F(".");
    }
    cout << endl;
    cout << F("Connected, IP address: \n");
    Serial.println(WiFi.localIP());
}

inline void AwotFirmwareWiFi::BeginBackup()
{
    BeginStationRadio(FACTORY_RESCUE_SSID, FACTORY_AP_PASSWORD);
    BeginWiFiConnection(FACTORY_WIFI_SSID, FACTORY_WIFI_PASSWORD);
}

inline void AwotFirmwareWiFi::Begin()
{
    WiFi.mode(WIFI_AP_STA);

    if (NoSdMount)
    {
        BeginBackup();
    }
    else
    {

        for (JsonObject wifi_item : config_json[api_wifi].as<JsonArray>())
        {

            const char *wtype = wifi_item[ft_type];   // "sta", "ap"
            const char *wssid = wifi_item[ft_ssid];         // "Livebox-4A40", "nodemcu-AwotFirmware-App"
            const char *wpassword = wifi_item[ft_password]; // "jUzeCfNcTtGxGNHTSC", "null"

            if (!strcmp(wtype, ft_sta))
            {
                WiFi.setHostname(factory_ota_hostname);
                WiFi.config(sta_local_IP, sta_gateway, sta_subnet, sta_gateway);

                if (strcmp(wssid, null))
                {
                    if (!strcmp(wpassword, null))
                        WiFi.begin(wssid);
                    else
                        WiFi.begin(wssid, wpassword);
                }
                else
                {
                    NoSTA = true;
                }
            }
            else if (!strcmp(wtype, ft_ap))
            {

                if (strcmp(wssid, null))
                {
                    cout << (WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet) ? F("Ready\n") : F("Failed!\n"));
                    if (!strcmp(wpassword, null))
                    {
                        WiFi.softAP(wssid);
                    }
                    else
                        WiFi.softAP(wssid, wpassword);
                }
            }
        }

        if (!NoSTA)
        {
            while (WiFi.status() != WL_CONNECTED)
            {
                cout << F(".");
                delay(400);
                yield();
            }
        }
    }

    cout << endl;
    cout << F("Connected !") << endl;
}

inline void AwotFirmwareWiFi::ScanNetwork()
{
    if (!NoSdMount)
        if (sd.exists(static_NetworksFile_Path))
            sd.remove(static_NetworksFile_Path);

    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t *bssid;
    int32_t channel;
    bool hidden;
    int scanResult;
    cout << F("Starting WiFi scan...") << endl;

    scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

    if (scanResult == 0)
    {
        cout << F("No networks found") << endl;
    }
    else if (scanResult > 0)
    {
        cout << (PSTR("%d networks found:\n"), scanResult) << endl;

        // Print unsorted scan results
        JsonArray networks = networks_json.createNestedArray("networks");
        for (int8_t i = 0; i < scanResult; i++)
        {

            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
            // 00: [CH 01] [E8:D2:FF:E0:4A:40] -36dBm * V Livebox-4A40
            // Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %s\n"),
            //               i,
            //               channel,
            //               bssid[0], bssid[1], bssid[2],
            //               bssid[3], bssid[4], bssid[5],
            //               rssi,
            //               (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
            //               hidden ? 'H' : 'V',
            //               ssid.c_str());
            // get extra info
            // cout << ssid << ' ' << rssi << ' ' << encryptionType << ' ' << bssid << ' ' << channel << endl;

            // WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : ""

            char char_bssid[MAX_INPUT];
            char char_rssi[MAX_INPUT];
            char char_channel[MAX_INPUT];
            char char_ssid[MAX_INPUT];
            char char_status[MAX_INPUT];

            sprintf_P(char_bssid, PSTR("[%02X:%02X:%02X:%02X:%02X:%02X]"), bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
            sprintf_P(char_rssi, PSTR("%ddBm"), rssi);
            sprintf_P(char_channel, PSTR("[CH %02d]"), channel);
            sprintf(char_ssid, ssid.c_str());

            JsonObject networks_0 = networks.createNestedObject();
            networks_0[ft_channel] = char_channel;
            networks_0[ft_bssid] = char_bssid;
            networks_0[ft_rssi] = char_rssi;
            networks_0[ft_ssid] = char_ssid;

            switch (encryptionType)
            {
            case ENC_TYPE_AUTO:
                sprintf(char_status, "AUTO");
                /* code */
                // networks_0[ft_status] = "auto";
                break;
            case ENC_TYPE_CCMP:
                sprintf(char_status, "WPA2");
                // networks_0[ft_status] = "CCMP";
                /* code */
                break;
            case ENC_TYPE_NONE:
                sprintf(char_status, "NONE");
                cout << F("Create Feature For auto Connect to no secure") << endl;
                // networks_0[ft_status] = "NONE";
                /* code */
                break;
            case ENC_TYPE_TKIP:
                sprintf(char_status, "TKIP");
                // networks_0[ft_status] = "TKIP";
                /* code */
                break;
            case ENC_TYPE_WEP:
                sprintf(char_status, "WEB");
                /* code */
                break;

            default:
                break;
            }

            networks_0[ft_status] = char_status;

            // networks_0["status"] = encryptionType == ENC
            yield();
            cout << char_channel << space << char_bssid << space << char_rssi << space << char_ssid << space << char_status << endl;
        }

        if (!NoSdMount)
        {
            auto file = sd.open(static_NetworksFile_Path, FILE_WRITE);
            WriteBufferingStream output(file, 96);
            serializeJsonPretty(networks_json, output);
            output.flush();
            file.close();
        }
    }
    else
    {
        cout << (PSTR("WiFi scan error %d"), scanResult) << endl;
    }
}

#endif