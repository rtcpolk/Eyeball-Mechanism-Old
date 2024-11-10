// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/10/2024

#ifndef ERRORCODES_H
#define ERRORCODES_H

enum ErrorCode {
    NONE = 0,

    // Error codes for the BLE Client
    CLIENT_ALREADY_INITIALIZED,
    CLIENT_MEMORY_ALLOCATION_FAILED,
    CLIENT_GET_SCANNER_FAILED
};

#endif // ERRORCODES_H
