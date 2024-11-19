// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/19/2024

#include <Arduino.h>
#include <array>
#include "C:\Users\rober\blink\lib\ESP32Encoder\src\ESP32Encoder.h"

// Serial variables
constexpr uint32_t BAUD_RATE = 115200;

// Pin Variables
constexpr uint8_t FIRST_ENCODER_PIN_A = 15;
constexpr uint8_t FIRST_ENCODER_PIN_B = 16;
constexpr uint8_t SECOND_ENCODER_PIN_A = 17;
constexpr uint8_t SECOND_ENCODER_PIN_B = 18;
constexpr uint8_t THIRD_ENCODER_PIN_A = 19;
constexpr uint8_t THIRD_ENCODER_PIN_B = 20;

// Encoders
ESP32Encoder encoderA;
ESP32Encoder encoderB;
ESP32Encoder encoderC;

void setup() {
    Serial.begin(BAUD_RATE);

    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    encoderA.attachFullQuad(FIRST_ENCODER_PIN_A, FIRST_ENCODER_PIN_B);
    encoderB.attachFullQuad(SECOND_ENCODER_PIN_A, SECOND_ENCODER_PIN_B);
    encoderC.attachFullQuad(THIRD_ENCODER_PIN_A, THIRD_ENCODER_PIN_B);


}