#include <Arduino.h>
#include <Servo.h>
// https://community.platformio.org/t/compilation-terminated-for-serialtransfer-for-include-wire-h/23164
#include <Wire.h>

#include <string.h>

#include "SerialTransfer.h"

#include "main.hpp"
// #include "pingloop.hpp"
#include "infoled.hpp"

SerialTransfer stf;

typedef struct __attribute__((packed)){
  int      msg_type;
  char     data[4];
} STRUCT_Message;


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
// #  Ping Loop                       #
// ####################################
void init_ping(STRUCT_Message& ping_msg) {
    ping_msg.msg_type = 1;
    memcpy(ping_msg.data, TYPE_CODE, 4);
}

int ping_func(SerialTransfer& s, const STRUCT_Message& ping_msg) {
    // 经检查, 这里的 `ping_msg` 也没问题
    // s.port->println(ping_msg.msg_type);
    // s.port->println(ping_msg.data);
    s.txObj(ping_msg, 0);
    s.sendData(8);
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

int ping(SerialTransfer& s, STRUCT_Message& msg, const STRUCT_Message& ping_msg) {
    ping_func(s, ping_msg);

    delay(500);

    if (not s.available()) {
        return -1;
    }

    s.rxObj(msg, 0);

    return ping_res_chk(msg);
}


// --- INIT


void setup() {

    init_ping(PingMSG);

    Serial.begin(115200);
    stf.begin(Serial);
}


// --- LOOP

void loop() {
    // 经测试, PingMSG 初始化正确
    // if (ping(stf, Message, PingMSG) == 0) {
    //     led_on();
    // } else {
    //     led_off();
    // }
    stf.txObj(PingMSG, 0);
    stf.sendData(8);
    delay(1000);
}
