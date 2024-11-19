// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/18/2024

#ifndef ENCODERHANDLER_H
#define ENCODERHANDLER_H

#include <Arduino.h>
#include <ArduinoLog.h>

/**
 * A struct to represent an encoder
 */
struct Encoder {
    // Don't allow default construction
    Encoder() = delete;

    Encoder &operator=(const Encoder &) = delete;

    // Default copy-constructor, assignment-op, and destructor
    Encoder(const Encoder &) = default;

    ~Encoder() = default;

    /**
     * Primary Constructor
     *
     * @param PIN_A - The pin for channel A
     * @param PIN_B - The pin for channel B
     */
    Encoder(const uint8_t &PIN_A, const uint8_t &PIN_B);

    // Member variables
    const uint8_t pinA; // The pin for channel A
    const uint8_t pinB; // The pin for channel B
};

/**
 * A class to handle all of the encoder states and access their data
 */
class EncoderHandler {
public:
    // Delete constructor, copy-constructor, and assignment-op
    EncoderHandler() = delete;

    EncoderHandler(const EncoderHandler &) = delete;

    EncoderHandler &operator=(const EncoderHandler &) = delete;

    // Destructor
    ~EncoderHandler();

    /**
     * Initialize the Encoder Handler by creating the singleton instance
     *
     * @param FIRST_ENCODER_PIN_A - Channel A pin for the first encoder
     * @param FIRST_ENCODER_PIN_B - Channel B pin for the first encoder
     * @param SECOND_ENCODER_PIN_A - Channel A pin for the second encoder
     * @param SECOND_ENCODER_PIN_B - Channel B pin for the second encoder
     * @param THIRD_ENCODER_PIN_A - Channel A pin for the third encoder
     * @param THIRD_ENCODER_PIN_B - Channel B pin for the third encoder
     */
    static void initialize(const uint8_t &FIRST_ENCODER_PIN_A, const uint8_t &FIRST_ENCODER_PIN_B,
                           const uint8_t &SECOND_ENCODER_PIN_A, const uint8_t &SECOND_ENCODER_PIN_B,
                           const uint8_t &THIRD_ENCODER_PIN_A, const uint8_t &THIRD_ENCODER_PIN_B);


    static EncoderHandler *instance();

private:
    /**
     * Primary Constructor
     *
     * @param FIRST_ENCODER_PIN_A - Channel A pin for the first encoder
     * @param FIRST_ENCODER_PIN_B - Channel B pin for the first encoder
     * @param SECOND_ENCODER_PIN_A - Channel A pin for the second encoder
     * @param SECOND_ENCODER_PIN_B - Channel B pin for the second encoder
     * @param THIRD_ENCODER_PIN_A - Channel A pin for the third encoder
     * @param THIRD_ENCODER_PIN_B - Channel B pin for the third encoder
     */
    EncoderHandler(const uint8_t &FIRST_ENCODER_PIN_A, const uint8_t &FIRST_ENCODER_PIN_B,
                   const uint8_t &SECOND_ENCODER_PIN_A, const uint8_t &SECOND_ENCODER_PIN_B,
                   const uint8_t &THIRD_ENCODER_PIN_A, const uint8_t &THIRD_ENCODER_PIN_B);

    // Member variables
    static EncoderHandler *inst; // Ptr to the singleton inst
    Encoder firstEncoder;   // First encoder
    Encoder secondEncoder;   // Second encoder
    Encoder thirdEncoder;   // Third encoder
};

#endif // ENCODERHANDLER_H