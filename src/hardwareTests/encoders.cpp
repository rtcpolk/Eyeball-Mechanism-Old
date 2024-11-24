// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/24/2024

#include <Arduino.h>
#include <array>
#include "ESP32Encoder.h"

// Program variables
constexpr uint32_t BAUD_RATE = 115200;
std::array<ESP32Encoder, 3> encoders;
std::array<int64_t, 3> curCounts;
std::array<int64_t, 3> prevCounts;

// Pin variables - set these to match the hardware setup
constexpr uint8_t FIRST_ENCODER_PIN_A = 16;
constexpr uint8_t FIRST_ENCODER_PIN_B = 17;
constexpr uint8_t SECOND_ENCODER_PIN_A = 0;
constexpr uint8_t SECOND_ENCODER_PIN_B = 0;
constexpr uint8_t THIRD_ENCODER_PIN_A = 0;
constexpr uint8_t THIRD_ENCODER_PIN_B = 0;
constexpr std::array<std::array<uint8_t, 2>, 3> pins = {FIRST_ENCODER_PIN_A,
                                                               FIRST_ENCODER_PIN_B,
                                                               SECOND_ENCODER_PIN_A,
                                                               SECOND_ENCODER_PIN_B,
                                                               THIRD_ENCODER_PIN_A,
                                                               THIRD_ENCODER_PIN_B};

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