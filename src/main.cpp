#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BLE settings
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pTxCharacteristic;
BLECharacteristic *pRxCharacteristic;
bool deviceConnected = false;

// Hardware settings
const int LED_PIN = 2;
const int SENSOR_PIN = 32;

// Function prototypes
void setupOLED();
void setupBLE();
void updateOLED(const char* message);
void notifySensorValue();
void handleReceivedValue();

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        updateOLED("Connected");
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        BLEDevice::startAdvertising();
        updateOLED("Disconnected");
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    setupOLED();
    setupBLE();
}

void loop() {
    if (deviceConnected) {
        notifySensorValue();
        handleReceivedValue();
    }
    delay(2000);
}

void setupOLED() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;) ; // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.setTextSize(1.5);
    display.setTextColor(SSD1306_WHITE);
    updateOLED("Starting BLE...");
}

void setupBLE() {
    BLEDevice::init("esp32BTTest");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );

    pTxCharacteristic->setValue("Hello From ESP32");
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("Characteristic defined! Now you can read it in your phone!");
    updateOLED("BLE started");
}

void updateOLED(const char* message) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(message);
    display.display();
}

void notifySensorValue() {
    float txValue = analogRead(SENSOR_PIN) / 3.456;
    String txString = String(txValue, 2);
    pTxCharacteristic->setValue(txString.c_str());
    pTxCharacteristic->notify();
    Serial.print("*** Sent Value: ");
    Serial.print(txString);
    Serial.println(" ***");

    // Update OLED display with the transmitted value
    updateOLED(("Sent Value: " + txString).c_str());
}

void handleReceivedValue() {
    std::string rxValue = pRxCharacteristic->getValue();
    if (!rxValue.empty()) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        Serial.println(rxValue.c_str());

        if (rxValue.find("A") != std::string::npos) {
            Serial.println("Turning ON!");
            digitalWrite(LED_PIN, HIGH);
        } else if (rxValue.find("B") != std::string::npos) {
            Serial.println("Turning OFF!");
            digitalWrite(LED_PIN, LOW);
        }

        Serial.println("*********");
        // Update OLED display with the received value
        updateOLED(("Received: " + String(rxValue.c_str())).c_str());

        // Clear the receive buffer
        pRxCharacteristic->setValue("");
    }
}
