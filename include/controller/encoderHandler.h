// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/19/2024

#ifndef ENCODERHANDLER_H
#define ENCODERHANDLER_H

#include <Arduino.h>
#include "ArduinoLog.h"
#include "ESP32Encoder.h"
#include <array>

/**
 * A class to handle all the 3 encoders. It manages their states and allows access to their data
 */
class EncoderHandler {
public:
    // Delete copy-constructor and assignment-op
    EncoderHandler(const EncoderHandler &) = delete;

    EncoderHandler &operator=(const EncoderHandler &) = delete;

    // Destructor
    ~EncoderHandler() noexcept;

    /**
     * Initialize the Encoder Handler by setting the encoder pins
     *
     * @param pins - An array that holds all of the pins for the encoders
     *               {Channel A pin, Channel B pin}
     */
    void initialize(const std::array<std::array<uint8_t, 2>, 3> &pins);

    /**
     * Get the singleton EncoderHandler instance
     * @return The instance ptr
     */
    static EncoderHandler *instance();

    /**
    * Get the encoder counts
    * @return A reference to the array containing the encoder counts
    */
    const std::array<int64_t, 3> &getCounts() const noexcept;

    /**
     * Continuously update the encoder counts
     */
    [[noreturn]] void loop();

private:
    /**
     * Primary Constructor
     */
    EncoderHandler();

    /**
     * Updates each encoder's count
     */
    void updateCounts() noexcept;

    /**
     * Resets each of the encoder counts to 0. Used for calibration
     */
    void resetCounts() noexcept;

    // Member variables
    static EncoderHandler *inst; // Ptr to the singleton inst
    static bool initialized; // Initialization flag
    std::array<ESP32Encoder, 3> encoders; // Array to hold the encoders
    std::array<int64_t, 3> counts;  // Array to hold the encoder counts
};

#endif // ENCODERHANDLER_H