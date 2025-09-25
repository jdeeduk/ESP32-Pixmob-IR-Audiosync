#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
#include <BluetoothSerial.h>


const int IRTPin = 14;     // IR LED GPIO pin (KY-005 / HW489)
const int soundPin = 27; // Sound Sensor DO pin (KY-038 / HW485)

IRsend irsend(IRTPin);

BluetoothSerial SerialBT;


// Raw colour codes (source: https://github.com/danielweidman/flipper-pixmob-ir-codes)
uint16_t red[21] = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t green[21] = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t light_green[26] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t yellow_green[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t blue[27] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};
uint16_t light_blue[28] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 700, 1400, 1400, 700, 2100, 700};
uint16_t magenta[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 700, 700, 700, 1400, 2100, 700, 700, 700, 2100, 700};
uint16_t yellow[21] = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t pink[23] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t orange[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 1400, 700, 2100, 1400, 2800, 700};
uint16_t whitish[24] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t turquoise[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 1400, 2800, 700, 700, 700, 2100, 700};



void setup() {
  Serial.begin(9600); // Setup serial for debugging
  
  // Init IR Transmitter
  irsend.begin();

  // Init Sound Sensor
  pinMode(soundPin, INPUT);

  // Init BT Receiver
  SerialBT.begin("ESP32-Bluetooth");


  Serial.println("Setup complete - waiting...");
}

void loop() {

  int data = digitalRead(soundPin); // If sound sensor is picking up sound, soundPin will be set HIGH.


  if (SerialBT.available()) {
    // Read data from Bluetooth
    String btinput = SerialBT.readString();
    btinput.trim();
    
    if (btinput == "yellow") {
      Serial.println("send yellow code");
      irsend.sendRaw(yellow, 21, 38);
    } 
    else if (btinput == "green") {
      Serial.println("send green code");
      irsend.sendRaw(green, 21, 38);
    } 
    else if (btinput == "pink") {
      Serial.println("send pink code");
      irsend.sendRaw(pink, 23, 38);
    } 
    else if (btinput == "blue") {
      Serial.println("send blue code");
      irsend.sendRaw(blue, 27, 38);
    } 
    else if (btinput == "red") {
      Serial.println("send red code");
      irsend.sendRaw(red, 21, 38);
    } 
    else {
      Serial.println("Unknown command");
    }
  } else {
    // If not connected via BT then revert to sound based controls
    // If soundPin is set HIGH then get a random number 1-6 and set the colour accordingly. 
    if (data == 1) {
      int randomNumber = random(1, 6);

      if (randomNumber == 1) {
        Serial.println("send yellow code");
        irsend.sendRaw(yellow, 21, 38);
      }
      if (randomNumber == 2) {
        Serial.println("send green code");
        irsend.sendRaw(green, 21, 38);
      }
      if (randomNumber == 3) {
        Serial.println("send pink code");
        irsend.sendRaw(pink, 23, 38);
      }
      if (randomNumber == 4){
        Serial.println("send blue code");
        irsend.sendRaw(blue, 27, 38);
      }
      if (randomNumber == 5){
        Serial.println("send red code");
        irsend.sendRaw(red, 21, 38);
      }
      delay(200);
      Serial.println(randomNumber);
      Serial.println("-----");
    }
  }

}

