#include <Arduino.h>
#include <Servo.h>
// https://community.platformio.org/t/compilation-terminated-for-serialtransfer-for-include-wire-h/23164
#include <Wire.h>

#include <string.h>

#include "SerialTransfer.h"

#include "main.hpp"
#include "infoled.hpp"

SerialTransfer stf;


STRUCT_Message Message = {0, 0};
STRUCT_Message PingMSG = {0, 0};


// ####################################
// #  Info LED                        #
// ####################################
void init_led() {
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, LED_IS_ON? HIGH: LOW);

}

void led_off() {
    if (LED_IS_ON) {
        LED_IS_ON = false;
        digitalWrite(LED_PIN, LED_IS_ON? HIGH: LOW);
    }
}

void led_on() {
    if (!LED_IS_ON) {
        LED_IS_ON = true;
        digitalWrite(LED_PIN, LED_IS_ON? HIGH: LOW);
    }
}


// ####################################
// #  loop ping                       #
// ####################################

int ping_func() {
    stf.txObj(PingMSG, 0);
    stf.sendData(8);
    return 0;
}

int ping_res_chk(const STRUCT_Message& msg) {
    if (msg.msg_type != 0x0001) {
        return -1;
    }
    for (int i=0; i < 4; i++) {
        if (((int) msg.data[i]) != 0) {
            return -1;
        }
    }
    return 0;
}

int ping() {
    ping_func();

    delay(300);

    if (not stf.available()) {
        return -1;
    }

    stf.rxObj(Message, 0);

    return ping_res_chk(Message);
}


// --- INIT

void init_ping() {
    PingMSG.msg_type = 0x0001;
    memcpy(PingMSG.data, TYPE_CODE, 4);
}

void setup() {

    init_ping();

    Serial.begin(115200);
    stf.begin(Serial);
}


// --- LOOP

void loop() {
    led_on();
    if (ping() == 0) {
    } else {
        led_off();
    }
    delay(500);
}