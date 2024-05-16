#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pTxCharacteristic;
BLECharacteristic *pRxCharacteristic;

float rxValue = 0;
float txValue = 0;
const int LED = 2;
const int readPin = 32;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("esp32BTTest");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pTxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  pTxCharacteristic->setValue("Hello From ESP32");
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop()
{
 
  txValue = analogRead(readPin) / 3.456;
  String txString = String(txValue, 2);
  pTxCharacteristic->setValue(txString.c_str());
  pTxCharacteristic->notify();
  Serial.print("*** Sent Value: ");
  Serial.print(txString);
  Serial.println(" ***");


  std::string rxValue = pRxCharacteristic->getValue();
  if (!rxValue.empty())
  {
    Serial.println("*********");
    Serial.print("Received Value: ");
    Serial.println(rxValue.c_str());
    if (rxValue.find("A") != -1)
    {
      Serial.println("Turning ON!");
      digitalWrite(LED, HIGH);
    }
    else if (rxValue.find("B") != -1)
    {
      Serial.println("Turning OFF!");
      digitalWrite(LED, LOW);
    }
    Serial.println("*********");
  }
  delay(2000);
}