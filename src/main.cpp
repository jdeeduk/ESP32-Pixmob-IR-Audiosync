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
// RED variants
uint16_t red[21] = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t red_2[21] = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t red_3[21] = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 2800, 700, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t red_4[21] = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2800, 700, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t red_5[21] = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t dim_red[25] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 1400, 2800, 700};
uint16_t dim_red_2[25] = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 1400, 2800, 700};

// GREEN variants
uint16_t green[21] = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t grn_2[23] = {1400, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t grn_3[23] = {1400, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t grn_4[21] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 2100, 1400, 2100, 1400, 2800, 700};
uint16_t grn_5[23] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t grn_6[23] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 1400, 700, 2100, 1400, 2800, 700};
uint16_t grn_7[25] = {700, 700, 700, 2100, 700, 700, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 1400, 1400, 700, 700, 700, 2800, 1400, 2800, 700};
uint16_t grn_8[23] = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700, 2800, 1400, 2800, 700};
uint16_t grn_9[21] = {1400, 1400, 700, 700, 700, 1400, 700, 700, 700, 700, 1400, 2800, 700, 2100, 700, 2100, 700, 2800, 1400, 2800, 700};
uint16_t grn_10[23] = {1400, 1400, 700, 700, 700, 1400, 700, 700, 700, 700, 1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 2800, 1400, 2800, 700};
uint16_t grn_11[23] = {1400, 2800, 700, 700, 1400, 700, 700, 700, 1400, 1400, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t grn_12[21] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t grn_13[25] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t grn_14[23] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 700, 1400, 700, 1400, 700};
uint16_t grn_15[23] = {1400, 2800, 700, 700, 1400, 700, 700, 700, 1400, 1400, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t grn_dim[21] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 2100, 700, 700, 700, 2100, 700, 2800, 1400, 2800, 700};
uint16_t light_green[26] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t light_green_2[26] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 1400, 700, 2100, 700, 700, 700, 2100, 700};

// YELLOW-GREEN variants
uint16_t yellow_green[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t yellow_green_2[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 700, 700, 700, 700, 2800, 1400, 2800, 700};
uint16_t yellow_green_3[23] = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};

// BLUE variants
uint16_t blue[27] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};
uint16_t blue_2[27] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};
uint16_t blue_3[27] = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};
uint16_t light_blue[28] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 700, 1400, 1400, 700, 2100, 700};
uint16_t light_blue_2[23] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 700, 700, 1400, 1400, 700};
uint16_t light_blue_3[23] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 700, 700, 1400, 700};
uint16_t dim_blue[27] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 1400, 700, 1400, 700};

// MAGENTA variants
uint16_t magenta[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 700, 700, 700, 1400, 2100, 700, 700, 700, 2100, 700};
uint16_t mag_2[26] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 2100, 700, 1400, 1400, 700, 700};
uint16_t mag_3[26] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 2100, 1400, 700, 1400, 700, 700};
uint16_t mag_4[27] = {700, 700, 700, 2100, 1400, 700, 700, 700, 700, 700, 1400, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 2100, 1400, 700, 1400, 700, 700};

// YELLOW variants
uint16_t yellow[21] = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t yellow_2[23] = {1400, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700, 2100, 700, 700, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t yellow_3[19] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 2100, 1400, 2100, 1400, 2800, 700};
uint16_t yellow_4[21] = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 1400, 2800, 700};
uint16_t yellow_5[25] = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 1400, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 2800, 700};
uint16_t yellow_6[25] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 2800, 700};
uint16_t yellow_7[21] = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 2100, 1400, 700, 700, 700, 700, 2100, 700, 2800, 1400, 2800, 700};
uint16_t yellow_8[23] = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t yellow_9[21] = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2100, 1400, 700, 700, 700, 700, 2100, 700, 2800, 1400, 2800, 700};

// PINK variants
uint16_t pink[23] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t pink_2[21] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 2100, 1400, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t pink_3[23] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t pink_4[26] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 700, 2100, 1400, 700, 700};
uint16_t pink_5[28] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 700, 700, 700, 700, 1400, 700, 700};

// ORANGE variants
uint16_t orange[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 1400, 700, 2100, 1400, 2800, 700};
uint16_t orange_2[19] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2800, 1400, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t orange_3[21] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t red_orange[21] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2800, 1400, 1400, 700, 700, 700, 1400, 700, 2100, 1400, 2800, 700};
uint16_t red_orange_2[21] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 700, 700, 700, 1400, 2100, 1400, 2800, 700};
uint16_t red_orange_3[21] = {1400, 1400, 700, 700, 700, 2100, 700, 2100, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t yellow_orange_1[23] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 700, 2100, 700, 700, 700, 700, 1400, 2100, 1400, 2800, 700};
uint16_t yellow_orange_2[21] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 1400, 2800, 700};

// WHITISH variants
uint16_t whitish[24] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t whitish_long[25] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 700, 1400, 2100, 1400, 700, 700};
uint16_t whitish_2[23] = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t whitish_long_2[26] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 700, 1400, 700, 700, 700, 1400, 700, 700};
uint16_t whitish_3[23] = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 1400, 2800, 700, 700, 700, 2100, 700};
uint16_t whitish_4[25] = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 1400, 700, 700, 1400, 1400, 700, 700, 1400, 1400, 700, 2100, 700};
uint16_t whitish_5[25] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 700, 700, 1400, 1400, 700, 700, 1400, 1400, 700, 2100, 700};
uint16_t whitish_6[27] = {700, 700, 700, 2100, 1400, 700, 700, 700, 700, 700, 1400, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 700, 1400, 700, 700, 700, 1400, 700, 700};
uint16_t whitish_7[29] = {700, 700, 700, 2100, 1400, 700, 700, 700, 700, 700, 1400, 1400, 700, 700, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 700, 700, 700, 700, 1400, 700, 700};

// TURQUOISE variants
uint16_t turquoise[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 1400, 2800, 700, 700, 700, 2100, 700};
uint16_t turquoise_2[27] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 700, 700, 1400, 1400, 700, 700};
uint16_t turquoise_3[29] = {700, 700, 700, 2100, 1400, 700, 700, 700, 700, 700, 1400, 1400, 700, 700, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 700, 1400, 700, 1400, 700, 700};
uint16_t turquoise_4[27] = {700, 700, 700, 2100, 1400, 1400, 1400, 700, 1400, 1400, 700, 700, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 700, 1400, 700, 1400, 700, 700};



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
    
    // RED variants
    if (btinput == "red") {
      Serial.println("send red code");
      irsend.sendRaw(red, 21, 38);
    }
    else if (btinput == "red_2") {
      Serial.println("send red_2 code");
      irsend.sendRaw(red_2, 21, 38);
    }
    else if (btinput == "red_3") {
      Serial.println("send red_3 code");
      irsend.sendRaw(red_3, 21, 38);
    }
    else if (btinput == "red_4") {
      Serial.println("send red_4 code");
      irsend.sendRaw(red_4, 21, 38);
    }
    else if (btinput == "red_5") {
      Serial.println("send red_5 code");
      irsend.sendRaw(red_5, 21, 38);
    }
    else if (btinput == "dim_red") {
      Serial.println("send dim_red code");
      irsend.sendRaw(dim_red, 25, 38);
    }
    else if (btinput == "dim_red_2") {
      Serial.println("send dim_red_2 code");
      irsend.sendRaw(dim_red_2, 25, 38);
    }
    
    // GREEN variants
    else if (btinput == "green") {
      Serial.println("send green code");
      irsend.sendRaw(green, 21, 38);
    }
    else if (btinput == "grn_2") {
      Serial.println("send grn_2 code");
      irsend.sendRaw(grn_2, 23, 38);
    }
    else if (btinput == "grn_3") {
      Serial.println("send grn_3 code");
      irsend.sendRaw(grn_3, 23, 38);
    }
    else if (btinput == "grn_4") {
      Serial.println("send grn_4 code");
      irsend.sendRaw(grn_4, 21, 38);
    }
    else if (btinput == "grn_5") {
      Serial.println("send grn_5 code");
      irsend.sendRaw(grn_5, 23, 38);
    }
    else if (btinput == "grn_6") {
      Serial.println("send grn_6 code");
      irsend.sendRaw(grn_6, 23, 38);
    }
    else if (btinput == "grn_7") {
      Serial.println("send grn_7 code");
      irsend.sendRaw(grn_7, 25, 38);
    }
    else if (btinput == "grn_8") {
      Serial.println("send grn_8 code");
      irsend.sendRaw(grn_8, 23, 38);
    }
    else if (btinput == "grn_9") {
      Serial.println("send grn_9 code");
      irsend.sendRaw(grn_9, 21, 38);
    }
    else if (btinput == "grn_10") {
      Serial.println("send grn_10 code");
      irsend.sendRaw(grn_10, 23, 38);
    }
    else if (btinput == "grn_11") {
      Serial.println("send grn_11 code");
      irsend.sendRaw(grn_11, 23, 38);
    }
    else if (btinput == "grn_12") {
      Serial.println("send grn_12 code");
      irsend.sendRaw(grn_12, 21, 38);
    }
    else if (btinput == "grn_13") {
      Serial.println("send grn_13 code");
      irsend.sendRaw(grn_13, 25, 38);
    }
    else if (btinput == "grn_14") {
      Serial.println("send grn_14 code");
      irsend.sendRaw(grn_14, 23, 38);
    }
    else if (btinput == "grn_15") {
      Serial.println("send grn_15 code");
      irsend.sendRaw(grn_15, 23, 38);
    }
    else if (btinput == "grn_dim") {
      Serial.println("send grn_dim code");
      irsend.sendRaw(grn_dim, 21, 38);
    }
    else if (btinput == "light_green") {
      Serial.println("send light_green code");
      irsend.sendRaw(light_green, 26, 38);
    }
    else if (btinput == "light_green_2") {
      Serial.println("send light_green_2 code");
      irsend.sendRaw(light_green_2, 26, 38);
    }
    
    // YELLOW-GREEN variants
    else if (btinput == "yellow_green") {
      Serial.println("send yellow_green code");
      irsend.sendRaw(yellow_green, 24, 38);
    }
    else if (btinput == "yellow_green_2") {
      Serial.println("send yellow_green_2 code");
      irsend.sendRaw(yellow_green_2, 24, 38);
    }
    else if (btinput == "yellow_green_3") {
      Serial.println("send yellow_green_3 code");
      irsend.sendRaw(yellow_green_3, 23, 38);
    }
    
    // BLUE variants
    else if (btinput == "blue") {
      Serial.println("send blue code");
      irsend.sendRaw(blue, 27, 38);
    }
    else if (btinput == "blue_2") {
      Serial.println("send blue_2 code");
      irsend.sendRaw(blue_2, 27, 38);
    }
    else if (btinput == "blue_3") {
      Serial.println("send blue_3 code");
      irsend.sendRaw(blue_3, 27, 38);
    }
    else if (btinput == "light_blue") {
      Serial.println("send light_blue code");
      irsend.sendRaw(light_blue, 28, 38);
    }
    else if (btinput == "light_blue_2") {
      Serial.println("send light_blue_2 code");
      irsend.sendRaw(light_blue_2, 23, 38);
    }
    else if (btinput == "light_blue_3") {
      Serial.println("send light_blue_3 code");
      irsend.sendRaw(light_blue_3, 23, 38);
    }
    else if (btinput == "dim_blue") {
      Serial.println("send dim_blue code");
      irsend.sendRaw(dim_blue, 27, 38);
    }
    
    // MAGENTA variants
    else if (btinput == "magenta") {
      Serial.println("send magenta code");
      irsend.sendRaw(magenta, 24, 38);
    }
    else if (btinput == "mag_2") {
      Serial.println("send mag_2 code");
      irsend.sendRaw(mag_2, 26, 38);
    }
    else if (btinput == "mag_3") {
      Serial.println("send mag_3 code");
      irsend.sendRaw(mag_3, 26, 38);
    }
    else if (btinput == "mag_4") {
      Serial.println("send mag_4 code");
      irsend.sendRaw(mag_4, 27, 38);
    }
    
    // YELLOW variants
    else if (btinput == "yellow") {
      Serial.println("send yellow code");
      irsend.sendRaw(yellow, 21, 38);
    }
    else if (btinput == "yellow_2") {
      Serial.println("send yellow_2 code");
      irsend.sendRaw(yellow_2, 23, 38);
    }
    else if (btinput == "yellow_3") {
      Serial.println("send yellow_3 code");
      irsend.sendRaw(yellow_3, 19, 38);
    }
    else if (btinput == "yellow_4") {
      Serial.println("send yellow_4 code");
      irsend.sendRaw(yellow_4, 21, 38);
    }
    else if (btinput == "yellow_5") {
      Serial.println("send yellow_5 code");
      irsend.sendRaw(yellow_5, 25, 38);
    }
    else if (btinput == "yellow_6") {
      Serial.println("send yellow_6 code");
      irsend.sendRaw(yellow_6, 25, 38);
    }
    else if (btinput == "yellow_7") {
      Serial.println("send yellow_7 code");
      irsend.sendRaw(yellow_7, 21, 38);
    }
    else if (btinput == "yellow_8") {
      Serial.println("send yellow_8 code");
      irsend.sendRaw(yellow_8, 23, 38);
    }
    else if (btinput == "yellow_9") {
      Serial.println("send yellow_9 code");
      irsend.sendRaw(yellow_9, 21, 38);
    }
    
    // PINK variants
    else if (btinput == "pink") {
      Serial.println("send pink code");
      irsend.sendRaw(pink, 23, 38);
    }
    else if (btinput == "pink_2") {
      Serial.println("send pink_2 code");
      irsend.sendRaw(pink_2, 21, 38);
    }
    else if (btinput == "pink_3") {
      Serial.println("send pink_3 code");
      irsend.sendRaw(pink_3, 23, 38);
    }
    else if (btinput == "pink_4") {
      Serial.println("send pink_4 code");
      irsend.sendRaw(pink_4, 26, 38);
    }
    else if (btinput == "pink_5") {
      Serial.println("send pink_5 code");
      irsend.sendRaw(pink_5, 28, 38);
    }
    
    // ORANGE variants
    else if (btinput == "orange") {
      Serial.println("send orange code");
      irsend.sendRaw(orange, 24, 38);
    }
    else if (btinput == "orange_2") {
      Serial.println("send orange_2 code");
      irsend.sendRaw(orange_2, 19, 38);
    }
    else if (btinput == "orange_3") {
      Serial.println("send orange_3 code");
      irsend.sendRaw(orange_3, 21, 38);
    }
    else if (btinput == "red_orange") {
      Serial.println("send red_orange code");
      irsend.sendRaw(red_orange, 21, 38);
    }
    else if (btinput == "red_orange_2") {
      Serial.println("send red_orange_2 code");
      irsend.sendRaw(red_orange_2, 21, 38);
    }
    else if (btinput == "red_orange_3") {
      Serial.println("send red_orange_3 code");
      irsend.sendRaw(red_orange_3, 21, 38);
    }
    else if (btinput == "yellow_orange_1") {
      Serial.println("send yellow_orange_1 code");
      irsend.sendRaw(yellow_orange_1, 23, 38);
    }
    else if (btinput == "yellow_orange_2") {
      Serial.println("send yellow_orange_2 code");
      irsend.sendRaw(yellow_orange_2, 21, 38);
    }
    
    // WHITISH variants
    else if (btinput == "whitish") {
      Serial.println("send whitish code");
      irsend.sendRaw(whitish, 24, 38);
    }
    else if (btinput == "whitish_long") {
      Serial.println("send whitish_long code");
      irsend.sendRaw(whitish_long, 25, 38);
    }
    else if (btinput == "whitish_2") {
      Serial.println("send whitish_2 code");
      irsend.sendRaw(whitish_2, 23, 38);
    }
    else if (btinput == "whitish_long_2") {
      Serial.println("send whitish_long_2 code");
      irsend.sendRaw(whitish_long_2, 26, 38);
    }
    else if (btinput == "whitish_3") {
      Serial.println("send whitish_3 code");
      irsend.sendRaw(whitish_3, 23, 38);
    }
    else if (btinput == "whitish_4") {
      Serial.println("send whitish_4 code");
      irsend.sendRaw(whitish_4, 25, 38);
    }
    else if (btinput == "whitish_5") {
      Serial.println("send whitish_5 code");
      irsend.sendRaw(whitish_5, 25, 38);
    }
    else if (btinput == "whitish_6") {
      Serial.println("send whitish_6 code");
      irsend.sendRaw(whitish_6, 27, 38);
    }
    else if (btinput == "whitish_7") {
      Serial.println("send whitish_7 code");
      irsend.sendRaw(whitish_7, 29, 38);
    }
    
    // TURQUOISE variants
    else if (btinput == "turquoise") {
      Serial.println("send turquoise code");
      irsend.sendRaw(turquoise, 24, 38);
    }
    else if (btinput == "turquoise_2") {
      Serial.println("send turquoise_2 code");
      irsend.sendRaw(turquoise_2, 27, 38);
    }
    else if (btinput == "turquoise_3") {
      Serial.println("send turquoise_3 code");
      irsend.sendRaw(turquoise_3, 29, 38);
    }
    else if (btinput == "turquoise_4") {
      Serial.println("send turquoise_4 code");
      irsend.sendRaw(turquoise_4, 27, 38);
    }
    
    else {
      Serial.println("Unknown command");
    }
  } else {
    // If not connected via BT then revert to sound based controls
    // If soundPin is set HIGH then get a random number and set the colour accordingly. 
    if (data == 1) {
      // Total number of available colours: 80+ variants
      int randomNumber = random(1, 81);

      // RED variants (1-7)
      if (randomNumber == 1) {
        Serial.println("send red code");
        irsend.sendRaw(red, 21, 38);
      }
      else if (randomNumber == 2) {
        Serial.println("send red_2 code");
        irsend.sendRaw(red_2, 21, 38);
      }
      else if (randomNumber == 3) {
        Serial.println("send red_3 code");
        irsend.sendRaw(red_3, 21, 38);
      }
      else if (randomNumber == 4) {
        Serial.println("send red_4 code");
        irsend.sendRaw(red_4, 21, 38);
      }
      else if (randomNumber == 5) {
        Serial.println("send red_5 code");
        irsend.sendRaw(red_5, 21, 38);
      }
      else if (randomNumber == 6) {
        Serial.println("send dim_red code");
        irsend.sendRaw(dim_red, 25, 38);
      }
      else if (randomNumber == 7) {
        Serial.println("send dim_red_2 code");
        irsend.sendRaw(dim_red_2, 25, 38);
      }
      
      // GREEN variants (8-24)
      else if (randomNumber == 8) {
        Serial.println("send green code");
        irsend.sendRaw(green, 21, 38);
      }
      else if (randomNumber == 9) {
        Serial.println("send grn_2 code");
        irsend.sendRaw(grn_2, 23, 38);
      }
      else if (randomNumber == 10) {
        Serial.println("send grn_3 code");
        irsend.sendRaw(grn_3, 23, 38);
      }
      else if (randomNumber == 11) {
        Serial.println("send grn_4 code");
        irsend.sendRaw(grn_4, 21, 38);
      }
      else if (randomNumber == 12) {
        Serial.println("send grn_5 code");
        irsend.sendRaw(grn_5, 23, 38);
      }
      else if (randomNumber == 13) {
        Serial.println("send grn_6 code");
        irsend.sendRaw(grn_6, 23, 38);
      }
      else if (randomNumber == 14) {
        Serial.println("send grn_7 code");
        irsend.sendRaw(grn_7, 25, 38);
      }
      else if (randomNumber == 15) {
        Serial.println("send grn_8 code");
        irsend.sendRaw(grn_8, 23, 38);
      }
      else if (randomNumber == 16) {
        Serial.println("send grn_9 code");
        irsend.sendRaw(grn_9, 21, 38);
      }
      else if (randomNumber == 17) {
        Serial.println("send grn_10 code");
        irsend.sendRaw(grn_10, 23, 38);
      }
      else if (randomNumber == 18) {
        Serial.println("send grn_11 code");
        irsend.sendRaw(grn_11, 23, 38);
      }
      else if (randomNumber == 19) {
        Serial.println("send grn_12 code");
        irsend.sendRaw(grn_12, 21, 38);
      }
      else if (randomNumber == 20) {
        Serial.println("send grn_13 code");
        irsend.sendRaw(grn_13, 25, 38);
      }
      else if (randomNumber == 21) {
        Serial.println("send grn_14 code");
        irsend.sendRaw(grn_14, 23, 38);
      }
      else if (randomNumber == 22) {
        Serial.println("send grn_15 code");
        irsend.sendRaw(grn_15, 23, 38);
      }
      else if (randomNumber == 23) {
        Serial.println("send grn_dim code");
        irsend.sendRaw(grn_dim, 21, 38);
      }
      else if (randomNumber == 24) {
        Serial.println("send light_green code");
        irsend.sendRaw(light_green, 26, 38);
      }
      
      // YELLOW-GREEN variants (25-27)
      else if (randomNumber == 25) {
        Serial.println("send yellow_green code");
        irsend.sendRaw(yellow_green, 24, 38);
      }
      else if (randomNumber == 26) {
        Serial.println("send yellow_green_2 code");
        irsend.sendRaw(yellow_green_2, 24, 38);
      }
      else if (randomNumber == 27) {
        Serial.println("send yellow_green_3 code");
        irsend.sendRaw(yellow_green_3, 23, 38);
      }
      
      // BLUE variants (28-34)
      else if (randomNumber == 28) {
        Serial.println("send blue code");
        irsend.sendRaw(blue, 27, 38);
      }
      else if (randomNumber == 29) {
        Serial.println("send blue_2 code");
        irsend.sendRaw(blue_2, 27, 38);
      }
      else if (randomNumber == 30) {
        Serial.println("send blue_3 code");
        irsend.sendRaw(blue_3, 27, 38);
      }
      else if (randomNumber == 31) {
        Serial.println("send light_blue code");
        irsend.sendRaw(light_blue, 28, 38);
      }
      else if (randomNumber == 32) {
        Serial.println("send light_blue_2 code");
        irsend.sendRaw(light_blue_2, 23, 38);
      }
      else if (randomNumber == 33) {
        Serial.println("send light_blue_3 code");
        irsend.sendRaw(light_blue_3, 23, 38);
      }
      else if (randomNumber == 34) {
        Serial.println("send dim_blue code");
        irsend.sendRaw(dim_blue, 27, 38);
      }
      
      // MAGENTA variants (35-38)
      else if (randomNumber == 35) {
        Serial.println("send magenta code");
        irsend.sendRaw(magenta, 24, 38);
      }
      else if (randomNumber == 36) {
        Serial.println("send mag_2 code");
        irsend.sendRaw(mag_2, 26, 38);
      }
      else if (randomNumber == 37) {
        Serial.println("send mag_3 code");
        irsend.sendRaw(mag_3, 26, 38);
      }
      else if (randomNumber == 38) {
        Serial.println("send mag_4 code");
        irsend.sendRaw(mag_4, 27, 38);
      }
      
      // YELLOW variants (39-47)
      else if (randomNumber == 39) {
        Serial.println("send yellow code");
        irsend.sendRaw(yellow, 21, 38);
      }
      else if (randomNumber == 40) {
        Serial.println("send yellow_2 code");
        irsend.sendRaw(yellow_2, 23, 38);
      }
      else if (randomNumber == 41) {
        Serial.println("send yellow_3 code");
        irsend.sendRaw(yellow_3, 19, 38);
      }
      else if (randomNumber == 42) {
        Serial.println("send yellow_4 code");
        irsend.sendRaw(yellow_4, 21, 38);
      }
      else if (randomNumber == 43) {
        Serial.println("send yellow_5 code");
        irsend.sendRaw(yellow_5, 25, 38);
      }
      else if (randomNumber == 44) {
        Serial.println("send yellow_6 code");
        irsend.sendRaw(yellow_6, 25, 38);
      }
      else if (randomNumber == 45) {
        Serial.println("send yellow_7 code");
        irsend.sendRaw(yellow_7, 21, 38);
      }
      else if (randomNumber == 46) {
        Serial.println("send yellow_8 code");
        irsend.sendRaw(yellow_8, 23, 38);
      }
      else if (randomNumber == 47) {
        Serial.println("send yellow_9 code");
        irsend.sendRaw(yellow_9, 21, 38);
      }
      
      // PINK variants (48-52)
      else if (randomNumber == 48) {
        Serial.println("send pink code");
        irsend.sendRaw(pink, 23, 38);
      }
      else if (randomNumber == 49) {
        Serial.println("send pink_2 code");
        irsend.sendRaw(pink_2, 21, 38);
      }
      else if (randomNumber == 50) {
        Serial.println("send pink_3 code");
        irsend.sendRaw(pink_3, 23, 38);
      }
      else if (randomNumber == 51) {
        Serial.println("send pink_4 code");
        irsend.sendRaw(pink_4, 26, 38);
      }
      else if (randomNumber == 52) {
        Serial.println("send pink_5 code");
        irsend.sendRaw(pink_5, 28, 38);
      }
      
      // ORANGE variants (53-60)
      else if (randomNumber == 53) {
        Serial.println("send orange code");
        irsend.sendRaw(orange, 24, 38);
      }
      else if (randomNumber == 54) {
        Serial.println("send orange_2 code");
        irsend.sendRaw(orange_2, 19, 38);
      }
      else if (randomNumber == 55) {
        Serial.println("send orange_3 code");
        irsend.sendRaw(orange_3, 21, 38);
      }
      else if (randomNumber == 56) {
        Serial.println("send red_orange code");
        irsend.sendRaw(red_orange, 21, 38);
      }
      else if (randomNumber == 57) {
        Serial.println("send red_orange_2 code");
        irsend.sendRaw(red_orange_2, 21, 38);
      }
      else if (randomNumber == 58) {
        Serial.println("send red_orange_3 code");
        irsend.sendRaw(red_orange_3, 21, 38);
      }
      else if (randomNumber == 59) {
        Serial.println("send yellow_orange_1 code");
        irsend.sendRaw(yellow_orange_1, 23, 38);
      }
      else if (randomNumber == 60) {
        Serial.println("send yellow_orange_2 code");
        irsend.sendRaw(yellow_orange_2, 21, 38);
      }
      
      // WHITISH variants (61-69)
      else if (randomNumber == 61) {
        Serial.println("send whitish code");
        irsend.sendRaw(whitish, 24, 38);
      }
      else if (randomNumber == 62) {
        Serial.println("send whitish_long code");
        irsend.sendRaw(whitish_long, 25, 38);
      }
      else if (randomNumber == 63) {
        Serial.println("send whitish_2 code");
        irsend.sendRaw(whitish_2, 23, 38);
      }
      else if (randomNumber == 64) {
        Serial.println("send whitish_long_2 code");
        irsend.sendRaw(whitish_long_2, 26, 38);
      }
      else if (randomNumber == 65) {
        Serial.println("send whitish_3 code");
        irsend.sendRaw(whitish_3, 23, 38);
      }
      else if (randomNumber == 66) {
        Serial.println("send whitish_4 code");
        irsend.sendRaw(whitish_4, 25, 38);
      }
      else if (randomNumber == 67) {
        Serial.println("send whitish_5 code");
        irsend.sendRaw(whitish_5, 25, 38);
      }
      else if (randomNumber == 68) {
        Serial.println("send whitish_6 code");
        irsend.sendRaw(whitish_6, 27, 38);
      }
      else if (randomNumber == 69) {
        Serial.println("send whitish_7 code");
        irsend.sendRaw(whitish_7, 29, 38);
      }
      
      // TURQUOISE variants (70-73)
      else if (randomNumber == 70) {
        Serial.println("send turquoise code");
        irsend.sendRaw(turquoise, 24, 38);
      }
      else if (randomNumber == 71) {
        Serial.println("send turquoise_2 code");
        irsend.sendRaw(turquoise_2, 27, 38);
      }
      else if (randomNumber == 72) {
        Serial.println("send turquoise_3 code");
        irsend.sendRaw(turquoise_3, 29, 38);
      }
      else if (randomNumber == 73) {
        Serial.println("send turquoise_4 code");
        irsend.sendRaw(turquoise_4, 27, 38);
      }
      
      // Additional colours for remaining numbers (74-80)
      else if (randomNumber == 74) {
        Serial.println("send light_green_2 code");
        irsend.sendRaw(light_green_2, 26, 38);
      }
      else if (randomNumber == 75) {
        Serial.println("send red code");
        irsend.sendRaw(red, 21, 38);
      }
      else if (randomNumber == 76) {
        Serial.println("send green code");
        irsend.sendRaw(green, 21, 38);
      }
      else if (randomNumber == 77) {
        Serial.println("send blue code");
        irsend.sendRaw(blue, 27, 38);
      }
      else if (randomNumber == 78) {
        Serial.println("send yellow code");
        irsend.sendRaw(yellow, 21, 38);
      }
      else if (randomNumber == 79) {
        Serial.println("send pink code");
        irsend.sendRaw(pink, 23, 38);
      }
      else if (randomNumber == 80) {
        Serial.println("send magenta code");
        irsend.sendRaw(magenta, 24, 38);
      }
      
      delay(200);
      Serial.println(randomNumber);
      Serial.println("-----");
    }
  }

}

