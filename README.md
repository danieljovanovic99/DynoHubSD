# EasySDMMC Library

## Overview
The **EasySDMMC Library** is designed for SD card operations using **SD MMC** on **ESP32**, specifically for the **DynoHub Dev Board** and **OLIMEX POE ISO**. This library provides functions to read, write, list, delete, and append to files on SD cards, making data logging and file management easy.

## Features
- **Designed for ESP32-based DynoHub and OLIMEX POE ISO boards**
- **Uses SD MMC interface for high-speed SD card access**
- **File operations: create, read, write, append, delete, rename**
- **Directory management**
- **CSV logging support**
- **Automatic SD card reconnection**
- **Print SD card information**

## Installation
## 1. With Git
1. Clone this repository:
   ```sh
   git clone https://github.com/jvnvic/EasySDMMC.git
   ```
2. Include the library in your project:
   ```cpp
   #include "EasySDMMC.h"
   ```
3. Ensure your ESP32 project is configured to use **SD MMC**.

## 2. With .zip
1. Download the latest library release

2. In **Arduino IDE** choose 
   ```sh
   Sketch > Include Library > Add .ZIP Library
   ```
   
3. Include the library in your project:
   ```cpp
   #include "EasySDMMC.h"
   ```

4. Ensure your ESP32 project is configured to use **SD MMC**.

## Usage

### Initialize SD Card
```cpp
#include "EasySDMMC.h"

void setup() {
    Serial.begin(115200);
    if (!SDCARD.begin()) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    Serial.println("SD Card initialized successfully.");
}
```

### Write to a File
```cpp
SDCARD.writeFile("/example.txt", "Hello, DynoHub!");
```

### Read from a File
```cpp
SDCARD.readFile("/example.txt");
```

### Append to a File
```cpp
SDCARD.appendToFile("/example.txt", "\nAppended content!");
```

### Delete a File
```cpp
SDCARD.deleteFile("/example.txt");
```

### Rename a File
```cpp
SDCARD.renameFile("/example.txt", "/renamed_example.txt");
```

### Create and Remove Directories
```cpp
SDCARD.createFolder("/logs");
SDCARD.removeFolder("/logs");
```

### List Files in a Directory
```cpp
SDCARD.listFiles("/");
```

### Print SD Card Info
```cpp
SDCARD.printSDCardInfo();
```

### Start and Stop Logging
```cpp
SDCARD.startLog();
SDCARD.logToCSV("/logs/data.csv", "Timestamp, Value1, Value2");
SDCARD.stopLog();
```

### Display Library Logo
```cpp
SDCARD.printLogo();
```

## API Reference
| Function | Description |
|----------|-------------|
| `begin()` | Initializes the SD_MMC interface. Returns `true` on success. |
| `listFiles(folderName, levels)` | Lists all files and directories in the specified path. |
| `createFolder(folderName)` | Creates a new directory on the SD card. |
| `removeFolder(folderName)` | Removes a directory from the SD card. |
| `writeFile(fileName, textValue)` | Writes `textValue` to the specified file. |
| `appendToFile(fileName, textValue)` | Appends `textValue` to an existing file. |
| `readFile(fileName)` | Reads the content of a file. |
| `renameFile(oldName, newName)` | Renames a file. |
| `deleteFile(fileName)` | Deletes a file from the SD card. |
| `logToCSV(fileName, data)` | Appends CSV-formatted `data` to a file. |
| `printSDCardInfo()` | Prints SD card type and size. |
| `generateLogFilename()` | Generates a unique filename for logging. |
| `startLog()` | Starts a new logging session. |
| `stopLog()` | Stops the logging session. |
| `printLogo()` | Prints an ASCII logo of the library. |
| `isSDCardMounted()` | Checks if an SD card is available. |
| `attemptReconnect()` | Tries to reconnect the SD card if removed. |


## Contributions
Contributions are welcome! Feel free to open an issue or submit a pull request.

## Author
**Daniel Jovanovic**  
[GitHub](https://github.com/danieljovanovic99)  
[Email](mailto:daniel.jovanovic@outlook.com)
