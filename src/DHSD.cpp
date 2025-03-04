#include "DHSD.h"

DHSD::DHSD() {}

bool DHSD::begin() {
    return SD_MMC.begin("/sdcard", true);
}

bool DHSD::isSDCardMounted() {
    if (SD_MMC.cardType() == CARD_NONE) {
        return false; // No SD card detected
    }
    
    File testFile = SD_MMC.open("/test.txt", FILE_WRITE);
    if (!testFile) {
        Serial.println("SD Card check failed! Marking as unmounted.");
        return false;
    }
    
    testFile.close();
    SD_MMC.remove("/test.txt"); // Clean up test file
    return true;
}

void DHSD::attemptReconnect() {
    Serial.println("Attempting to reconnect to SD card...");
    SD_MMC.end();
    delay(500);
    if (SD_MMC.begin("/sdcard", true)) {
        Serial.println("SD Card reconnected!");
        loggingActive = true;
    } else {
        Serial.println("SD Card reconnect failed.");
    }
}

void DHSD::printSDCardInfo() {
    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD_MMC card attached");
        return;
    }
    Serial.print("SD_MMC Card Type: ");
    if (cardType == CARD_MMC) Serial.println("MMC");
    else if (cardType == CARD_SD) Serial.println("SDSC");
    else if (cardType == CARD_SDHC) Serial.println("SDHC");
    else Serial.println("UNKNOWN");

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
}

void DHSD::listFiles(const char* folderName, uint8_t levels) {
    if (!isSDCardMounted()) {
        attemptReconnect();
        if (!isSDCardMounted()) {
            Serial.println("SD Card removed! Cannot list files.");
            return;
        }
    }
    File root = SD_MMC.open(folderName);
    if (!root || !root.isDirectory()) {
        Serial.println("Failed to open directory");
        return;
    }
    Serial.printf("Listing directory: %s\n", folderName);
    
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.printf("DIR: %s\n", file.name());
            if (levels) {
                listFiles(file.name(), levels - 1);
            }
        } else {
            Serial.printf("FILE: %s\t%u bytes\n", file.name(), file.size());
        }
        file = root.openNextFile();
    }
}

void DHSD::writeFile(const char* fileName, const char* textValue) {
    if (!isSDCardMounted()) {
        attemptReconnect();
        if (!isSDCardMounted()) {
            Serial.println("SD Card removed! Cannot write file.");
            return;
        }
    }
    File file = SD_MMC.open(fileName, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    file.print(textValue);
    file.close();
}

void DHSD::logToCSV(const char* fileName, const char* data) {
    if (!isSDCardMounted()) {
        attemptReconnect();
        if (!isSDCardMounted()) {
            Serial.println("SD Card removed! Stopping logging.");
            loggingActive = false;
            return;
        }
    }
    File file = SD_MMC.open(fileName, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for logging. SD might be removed.");
        loggingActive = false;
        return;
    }
    
    file.print(data);
    file.print("\n");
    file.close();
}

String DHSD::generateLogFilename() {
    int highestNumber = 0;
    if (!isSDCardMounted()) return "";
    File root = SD_MMC.open("/Logs");
    if (!root || !root.isDirectory()) {
        SD_MMC.mkdir("/Logs");
    } else {
        File file = root.openNextFile();
        while (file) {
            String name = file.name();
            if (name.endsWith(".csv")) {
                int lastSlash = name.lastIndexOf('/');
                String fileNumStr = name.substring(lastSlash + 1, name.lastIndexOf('.'));
                if (fileNumStr.length() == 4 && fileNumStr.toInt() > 0) {
                    int fileNum = fileNumStr.toInt();
                    if (fileNum > highestNumber) {
                        highestNumber = fileNum;
                    }
                }
            }
            file = root.openNextFile();
        }
    }
    char filename[32];
    snprintf(filename, sizeof(filename), "/Logs/%04d.csv", highestNumber + 1);
    return String(filename);
}

void DHSD::startLog() {
    if (!isSDCardMounted()) {
        attemptReconnect();
        if (!isSDCardMounted()) {
            Serial.println("SD Card not available. Cannot start logging.");
            loggingActive = false;
            return;
        }
    }
    loggingActive = true;
    logFilePath = generateLogFilename();
    Serial.printf("\nLogging started! Saving data to: %s\n", logFilePath.c_str());
    if (!isSDCardMounted() || logFilePath == "") {
        Serial.println("Logging aborted due to SD card error.");
        loggingActive = false;
        return;
    }
    writeFile(logFilePath.c_str(), "Timestamp (ms), Sensor1, Sensor2, Sensor3\n");
}

void DHSD::stopLog() {
    loggingActive = false;
    Serial.println("\nLogging stopped.");
}

void DHSD::printLogo() {
  Serial.println("██████╗ ██╗   ██╗███╗   ██╗ ██████╗ ██╗  ██╗██╗   ██╗██████╗ ");
  Serial.println("██╔══██╗╚██╗ ██╔╝████╗  ██║██╔═══██╗██║  ██║██║   ██║██╔══██╗");
  Serial.println("██║  ██║ ╚████╔╝ ██╔██╗ ██║██║   ██║███████║██║   ██║██████╔╝");
  Serial.println("██║  ██║  ╚██╔╝  ██║╚██╗██║██║   ██║██╔══██║██║   ██║██╔══██╗");
  Serial.println("██████╔╝   ██║   ██║ ╚████║╚██████╔╝██║  ██║╚██████╔╝██████╔╝");
  Serial.println("╚═════╝    ╚═╝   ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ");
  Serial.println(" ");
}

DHSD SDCARD;
