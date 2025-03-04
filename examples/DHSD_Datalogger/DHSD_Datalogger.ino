#include <DHSD.h>

#define BUTTON_PIN 34
#define ADC_SENSOR_1 32
#define ADC_SENSOR_2 33
#define ADC_SENSOR_3 35

const int samplingRateHz = 10;
unsigned long logInterval = 1000 / samplingRateHz;
bool lastButtonState = HIGH;
bool sdCardMounted = false;

// Function to log sensor data to CSV
void logSensorData() {
    int sensor1 = analogRead(ADC_SENSOR_1);
    int sensor2 = analogRead(ADC_SENSOR_2);
    int sensor3 = analogRead(ADC_SENSOR_3);

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%lu, %d, %d, %d", millis(), sensor1, sensor2, sensor3);
    SDCARD.logToCSV(SDCARD.logFilePath.c_str(), buffer);

    Serial.printf("Logged: %s\n", buffer);
}

// Function to handle button press
void checkButtonPress() {
    bool buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW && lastButtonState == HIGH) {
        delay(300);

        if (!sdCardMounted) {
            return;
        }

        if (SDCARD.loggingActive) {
            SDCARD.stopLog();
        } else {
            SDCARD.startLog();
        }
    }

    lastButtonState = buttonState;
}

void setup() {
    Serial.begin(115200);
    SDCARD.printLogo();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(ADC_SENSOR_1, INPUT);
    pinMode(ADC_SENSOR_2, INPUT);
    pinMode(ADC_SENSOR_3, INPUT);

    if (SDCARD.begin()) {
        sdCardMounted = true;
        SDCARD.printSDCardInfo();
        SDCARD.listFiles("/");
        Serial.println("Press the button to start/stop logging.");
    } else {
        sdCardMounted = false;
        Serial.println("SD Card Mount Failed. Logging disabled.");
    }
}

void loop() {
    static unsigned long lastLogTime = 0;
    checkButtonPress();

    if (SDCARD.loggingActive && millis() - lastLogTime >= logInterval) {
        logSensorData();
        lastLogTime = millis();
    }
}
