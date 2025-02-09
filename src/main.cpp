#include <Arduino.h>
#include <Servo.h>
// https://community.platformio.org/t/compilation-terminated-for-serialtransfer-for-include-wire-h/23164
#include <Wire.h>

#include <string.h>

#include "SerialTransfer.h"

#include "main.hpp"
#include "infoled.hpp"
#include "jointservo.hpp"


SerialTransfer stf;
Servo servo;
int servo_current_angle = 90; // 初始默认中间位置


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


// ####################################
// #  Joint Servo                     #
// ####################################

void servo_init(int initial_position) {
    // 初始化舵机
    servo.attach(SERVO_PIN);
    
    // 将舵机转到初始位置
    servo.write(initial_position);
    servo_current_angle = initial_position;
    
    // 延迟让舵机完成定位
    delay(500);
}

void servo_to(int targetAngle) {
    // 确保目标角度在有效范围内
    targetAngle = constrain(targetAngle, 0, 180);
    
    // 计算旋转方向和角度差
    int angleDiff = targetAngle - servo_current_angle;
    
    // 如果角度差为0，直接返回
    if (angleDiff == 0) {
        return;
    }
    
    // 防止过大角度变化，控制在合理范围
    angleDiff = constrain(angleDiff, -90, 90);
    
    // 控制舵机旋转参数
    int stepDelay = 15;  // 每步延迟时间
    int stepSize = 3;    // 每次步进角度
    
    // 根据角度差确定正负旋转
    int increment = (angleDiff > 0) ? stepSize : -stepSize;
    
    // 计算需要执行的步数
    int steps = abs(angleDiff) / abs(stepSize);
    
    // 平滑旋转
    for (int i = 0; i < steps; i++) {
        servo_current_angle += increment;
        servo.write(servo_current_angle);
        delay(stepDelay);
    }
    
    // 确保最终位置精确到目标角度
    servo_current_angle = targetAngle;
    servo.write(servo_current_angle);
}


// --- INIT


void setup() {

    init_led();

    // init_ping();

    servo_init(0);

    Serial.begin(115200);
    // stf.begin(Serial);
}


// --- LOOP

void loop() {
    // led_on();
    // if (ping() == 0) {
    // } else {
    //     led_off();
    // }
    // delay(500);
    
    // 示例：在不同角度间切换
    Serial.println(0);
    servo_to(0);   // 旋转到 0 度
    delay(1000);
    Serial.println(90);
    servo_to(90); // 旋转到 90 度
    delay(1000);
    Serial.println(-90);
    servo_to(180); // 旋转到 180 度
    delay(1000);
}