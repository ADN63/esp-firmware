#pragma once
#ifndef _PARTIAL_OTA_BASE_H
#define _PARTIAL_OTA_BASE_H
class Ota
{
private:
    /* data */
public:
    char hostname[20];
    char password[20];
    int port;
    Ota(const char*, const char*, int);
    Ota(/* args */);
    ~Ota();
};

inline Ota::Ota(const char *hostname, const char *password, int port)
{
    sprintf_P(this->hostname, PSTR("%s"), hostname);
    sprintf_P(this->password, PSTR("%s"), password);
    this->port = port;    
}

Ota::Ota(/* args */)
{
}

Ota::~Ota()
{
}

#endif
