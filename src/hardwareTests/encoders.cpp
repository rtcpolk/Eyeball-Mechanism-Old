// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/24/2024

#include <Arduino.h>
#include <array>
#include "ESP32Encoder.h"

// Configuration variables - set these to match the hardware setup
constexpr uint8_t FIRST_ENCODER_PIN_A = 21;
constexpr uint8_t FIRST_ENCODER_PIN_B = 19;
constexpr uint8_t SECOND_ENCODER_PIN_A = 18;
constexpr uint8_t SECOND_ENCODER_PIN_B = 5;
constexpr uint8_t THIRD_ENCODER_PIN_A = 17;
constexpr uint8_t THIRD_ENCODER_PIN_B = 16;
constexpr uint32_t BAUD_RATE = 115200;

// Program variables
constexpr std::array<std::array<uint8_t, 2>, 3> pins = {FIRST_ENCODER_PIN_A,
                                                        FIRST_ENCODER_PIN_B,
                                                        SECOND_ENCODER_PIN_A,
                                                        SECOND_ENCODER_PIN_B,
                                                        THIRD_ENCODER_PIN_A,
                                                        THIRD_ENCODER_PIN_B};
std::array<ESP32Encoder, 3> encoders;
std::array<int64_t, 3> curCounts;
std::array<int64_t, 3> prevCounts;

void setup() {
    Serial.begin(BAUD_RATE);

    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    for (size_t i(0); i < encoders.size(); ++i) {
        encoders[i].attachFullQuad(pins[i][0], pins[i][1]);
        encoders[i].clearCount();
    }

    Serial.print("Counts:\t");
    Serial.print(curCounts[0]);
    Serial.print("\t");
    Serial.print(curCounts[1]);
    Serial.print("\t");
    Serial.println(curCounts[2]);
}

void loop() {
    for (size_t i(0); i < encoders.size(); ++i) {
        prevCounts[i] = curCounts[i];
        curCounts[i] = encoders[i].getCount();
    }

    if (curCounts[0] != prevCounts[0] ||curCounts[1] != prevCounts[1] ||curCounts[2] !=
    prevCounts[2]) {
        Serial.print("Counts:\t");
        Serial.print(curCounts[0]);
        Serial.print("\t");
        Serial.print(curCounts[1]);
        Serial.print("\t");
        Serial.println(curCounts[2]);
    }
}