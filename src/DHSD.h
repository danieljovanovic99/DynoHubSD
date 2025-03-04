#ifndef DHSD_H
#define DHSD_H

#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"

class DHSD {
public:
    DHSD();
    bool begin();
    void listFiles(const char* folderName, uint8_t levels = 0);
    void createFolder(const char* folderName);
    void removeFolder(const char* folderName);
    void writeFile(const char* fileName, const char* textValue);
    void appendToFile(const char* fileName, const char* textValue);
    void readFile(const char* fileName);
    void renameFile(const char* oldName, const char* newName);
    void deleteFile(const char* fileName);
    void logToCSV(const char* fileName, const char* data);
    void printSDCardInfo();
    void printLogo();
    String generateLogFilename();
    void startLog();
    void stopLog();
    bool isSDCardMounted();
    void attemptReconnect();  // New function to reconnect SD card
    bool loggingActive = false;
    String logFilePath;
};

extern DHSD SDCARD;

#endif
