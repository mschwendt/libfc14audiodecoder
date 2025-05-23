// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

#ifndef PAULA_H
#define PAULA_H

#include "common.h"

class PaulaVoice {
 public:
    // Paula
    struct _paula {
        const uint8_t* start;  // start address
        uint16_t length;        // length in 16-bit words
        uint16_t period;
        uint16_t volume;        // 0-64
    } paula;

    virtual ~PaulaVoice() { };
    virtual void on();
    virtual void off();
    virtual void takeNextBuf();   // take parameters from paula.* (or just to repeat.*)
};

class PaulaMixer {
 public:
    virtual ~PaulaMixer() { };
    virtual void init(uint8_t voices) = 0;
    virtual PaulaVoice* getVoice(uint8_t) = 0;
};

class PaulaPlayer {
 public:
    virtual ~PaulaPlayer() { };
    virtual void run() = 0;
};

#endif  // PAULA_H
