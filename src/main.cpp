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

void init_ping() {
    PingMSG.msg_type = 0x0001;
    memcpy(PingMSG.data, TYPE_CODE, 4);
}

int ping_func() {
    stf.txObj(PingMSG, 0);
    stf.sendData(8);
    return 0;
}

int ping_res_chk(const STRUCT_Message& msg) {
    if (msg.msg_type != 0x0001) {
        return -1;
    }
    if (memcmp(msg.data, "-hc-", 4) != 0) {
        return -2;
    }
    return 0;
}

int ping() {
    static uint8_t passed_times = 0b00000111;

    ping_func();

    delay(300);

    if (not stf.available()) {
        return -1;
    }

    stf.rxObj(Message, 0);

    int passed = ping_res_chk(Message);

    passed_times <<= 1;

    if (passed != 0) {
        return passed;
    }

    // check times
    passed_times += 1;
    uint8_t counting = 0;
    for (; passed < 8; passed++) {
        counting += (passed_times & (1 << passed)) == 0;
        if (counting >= 4) {
            return -3;
        }
    }
    return 0;
}


// --- INIT


void setup() {

    init_led();

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