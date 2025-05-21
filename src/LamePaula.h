// Simple AMIGA Paula Audio channel mixer -- Copyright (C) Michael Schwendt
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

#ifndef LAMEPAULA_H
#define LAMEPAULA_H

#include "Paula.h"

class LamePaulaMixer;

class LamePaulaVoice : public PaulaVoice
{
 public:
    LamePaulaVoice();
    ~LamePaulaVoice();

    void on();
    void off();
    void takeNextBuf();    // take parameters from paula.* (or just to repeat.*)

    friend class LamePaulaMixer;

 private:
    bool isOn;
    bool looping;  // whether to loop sample buffer continously (PAULA emu)
    
    const uint8_t* start;
    const uint8_t* end;
    uint32_t length;
    
    const uint8_t* repeatStart;
    const uint8_t* repeatEnd;
    uint32_t repeatLength;
    
    uint16_t curPeriod;
    uint32_t stepSpeed;
    uint32_t stepSpeedPnt;
    uint32_t stepSpeedAddPnt;
};

class LamePaulaMixer : public PaulaMixer
{
 public:
    LamePaulaMixer();
    ~LamePaulaMixer();
    void init(uint32_t freq, uint8_t bits, uint8_t channels, uint16_t zero);
    void init(uint8_t voices);
    PaulaVoice* getVoice(uint8_t); 

    unsigned long int fillBuffer(void* buffer, uint32_t bufferLen, PaulaPlayer *player);

 private:
    void setReplayingSpeed();
    void setBpm(uint16_t bpm);
    void end();

    void* (LamePaulaMixer::*_fillFunc)(void*, uint32_t);

    void* fill8bitMono(void*, uint32_t);
    void* fill8bitStereo(void*, uint32_t);
    void* fill16bitMono(void*, uint32_t);
    void* fill16bitStereo(void*, uint32_t);

    static const int _maxVoices = 32;
    LamePaulaVoice* _voice[_maxVoices];
    int _voices;

    uint32_t _pcmFreq;
    uint8_t _bitsPerSample;
    uint8_t _channels;
    uint16_t _zero;

    static const uint32_t AMIGA_CLOCK_PAL = 3546895;
    static const uint32_t AMIGA_CLOCK_NTSC = 3579546;
    const uint32_t AMIGA_CLOCK;

    int8_t mix8[256];
    int16_t mix16[256];

    uint8_t zero8bit;   // ``zero''-sample
    uint16_t zero16bit;  // either signed or unsigned
    
    uint8_t bufferScale;
    
    uint32_t samplesAdd;
    uint32_t samplesPnt;
    uint16_t samples, samplesOrg;
    
    uint32_t toFill;
    
    uint8_t emptySample;
};

#endif  // LAMEPAULA_H
