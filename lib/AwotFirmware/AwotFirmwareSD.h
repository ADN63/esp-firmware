#pragma once
#ifndef _AwotFirmware_SD_H
#define _AwotFirmware_SD_H

class AwotFirmwareSD
{
private:
    static uint32_t CalculateHash(const char *, uint32_t value = 0x811C9DC5);
    static uint32_t ReadNumber(uint32_t address);
    static uint32_t LookupTableIndex(const char *key, uint32_t length);
    static bool BeginSD();
    static bool BeginWebFile();
    /* data */
public:
    static void FileHandler(Request &req, Response &res);

    AwotFirmwareSD(/* args */);
    ~AwotFirmwareSD();
    static bool Begin();
};

inline uint32_t AwotFirmwareSD::CalculateHash(const char *string, uint32_t value)
{
    size_t length = strlen(string);

    for (size_t i = 0; i < length; i += 1)
    {
        value += (value << 1) + (value << 4) + (value << 7) + (value << 8) + (value << 24);
        value ^= string[i];
    }

    return value & 0x7FFFFFFF;
}

inline uint32_t AwotFirmwareSD::ReadNumber(uint32_t address)
{
    webFile.seek(address);
    long four = webFile.read();
    long three = webFile.read();
    long two = webFile.read();
    long one = webFile.read();

    return ((four << 0) & 0xFF) +
           ((three << 8) & 0xFFFF) +
           ((two << 16) & 0xFFFFFF) +
           ((one << 24) & 0xFFFFFFFF);
}

inline uint32_t AwotFirmwareSD::LookupTableIndex(const char *key, uint32_t length)
{
    uint32_t hash = CalculateHash(key);
    int32_t seed = ReadNumber(((hash % length) * 4) + 4);

    if (seed < 0)
    {
        return 0 - seed - 1;
    }

    return CalculateHash(key, seed) % length;
}

inline void AwotFirmwareSD::FileHandler(Request &req, Response &res)
{
    uint32_t fileCount = ReadNumber(0);
    uint32_t tableIndex = LookupTableIndex(req.path(), fileCount);
    uint32_t infoIndex = 4 + (fileCount * 4) + (tableIndex * 12);
    uint32_t calculatedHash = CalculateHash(req.path());
    uint32_t storedHash = ReadNumber(infoIndex);

    if (calculatedHash != storedHash)
    {
        return;
    }

    uint32_t offset = ReadNumber(infoIndex + 4);
    uint32_t length = ReadNumber(infoIndex + 8);

    webFile.seek(offset);

    res.set(header_connection, header_close);
    res.beginHeaders();

    while (length > 0)
    {
        int toRead = length > READ_BUFFER_SIZE ? READ_BUFFER_SIZE : length;
        webFile.read(readBuffer, toRead);
        res.write(readBuffer, toRead);
        length = length - toRead;
    }
    res.end();
}

AwotFirmwareSD::AwotFirmwareSD()
{
}

AwotFirmwareSD::~AwotFirmwareSD()
{
}

inline bool AwotFirmwareSD::BeginSD()
{

    if (!sd.begin(CS_PIN, SPI_HALF_SPEED))
    {
        return false;
    }

    return true;
}
inline bool AwotFirmwareSD::BeginWebFile()
{

    webFile = sd.open(static_WebFile_Path);
    if (!webFile)
    {
        return false;
    }
    return true;
}

inline bool AwotFirmwareSD::Begin()
{

    if (!BeginSD())
    {
        cout << F("Carte SD Hors Ligne") << endl;
        delay(2000);
        NoSdMount = true;    
        return false; 
    }

    if (!BeginWebFile())
    {
        cout << F("Fichiers Serveur Binaires absent") << static_WebFile_Path << endl;
        delay(2000);
        NoWebFile = true;  
        return false; 

    }

    return true;
}

#endif