// #include <Arduino.h>

// // https://community.platformio.org/t/compilation-terminated-for-serialtransfer-for-include-wire-h/23164
// #include <Wire.h>

// #include "SerialTransfer.h"


// SerialTransfer stf;

// struct __attribute__((packed)) STRUCT {
//     char z;
//     float y;
// } testStruct;

// char arr[6];


// void setup() {
//     Serial.begin(115200);
//     // Serial.println("Hello World!");

//     stf.begin(Serial);

//     testStruct.z = '$';
//     testStruct.y = 4.5;
// }

// void loop() {

//     // use this variable to keep track of how many
//     // bytes we're stuffing in the transmit buffer
//     uint16_t sendSize = 0;

//     ///////////////////////////////////////// Stuff buffer with struct
//     sendSize = stf.txObj(testStruct, sendSize);

//     ///////////////////////////////////////// Send buffer
//     stf.sendData(sendSize);
//     delay(500);
// }
