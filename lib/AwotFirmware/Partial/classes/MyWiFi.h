#pragma once
#ifndef _PARTIAL_MYWIFI_BASE_H
#define _PARTIAL_MYWIFI_BASE_H

#ifndef _PARTIAL_USER_BASE_H
#error ("Include User before MyWiFi")
#endif

class MyWiFi
{
private:
public:
    User user;
    MyWiFiType type;
    MyWiFi();
    MyWiFi(MyWiFiType type, const char *username, const char *password);
    // const char* stringType  = this->type == MyWiFiType::STA ? ft_sta : ft_ap;
};
MyWiFi::MyWiFi() {}
MyWiFi::MyWiFi(MyWiFiType type, const char *username, const char *password)
{
    this->type = type;
    this->user = User(username, password);
}
#endif