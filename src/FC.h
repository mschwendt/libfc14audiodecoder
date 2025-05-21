// Future Composer audio decoder -- Copyright (C) Michael Schwendt
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef FC_H
#define FC_H

#include <string>

#include "SmartPtr.h"
#include "Paula.h"

class FC : public PaulaPlayer {
 public:
    FC();
    ~FC();

    void setMixer(PaulaMixer*);
    bool isOurData(void*,unsigned long int);
    bool init(void*,uint32_t,int=0,int=0);
    void run();
    void restart(int=0,int=0);
    void off();
    bool songEnd; // whether song end has been reached

    bool isSMOD;  // whether file is in Future Composer 1.0 - 1.3 format
    bool isFC14;  // whether file is in Future Composer 1.4 format

    std::string formatName;
    static const std::string SMOD_FORMAT_NAME;
    static const std::string FC14_FORMAT_NAME;
    static const std::string UNKNOWN_FORMAT_NAME;

    static const uint16_t SMOD_SONGTAB_OFFSET = 0x0064;      // 100

    static const uint16_t FC14_SMPHEADERS_OFFSET = 0x0028;   // 40
    static const uint16_t FC14_WAVEHEADERS_OFFSET = 0x0064;  // 100
    static const uint16_t FC14_SONGTAB_OFFSET = 0x00b4;      // 180

    static const uint16_t TRACKTAB_ENTRY_LENGTH = 0x000d;    // 3*4+1
    static const uint16_t PATTERN_LENGTH = 0x0040;           // 32*2
    static const uint8_t PATTERN_BREAK = 0x49;

    static const uint8_t SEQ_END = 0xE1;

    static const uint8_t SNDMOD_LOOP = 0xE0;
    static const uint8_t SNDMOD_END = SEQ_END;
    static const uint8_t SNDMOD_SETWAVE = 0xE2;
    static const uint8_t SNDMOD_CHANGEWAVE = 0xE4;
    static const uint8_t SNDMOD_NEWVIB = 0xE3;
    static const uint8_t SNDMOD_SUSTAIN = 0xE8;
    static const uint8_t SNDMOD_NEWSEQ = 0xE7;
    static const uint8_t SNDMOD_SETPACKWAVE = 0xE9;
    static const uint8_t SNDMOD_PITCHBEND = 0xEA;

    static const uint8_t ENVELOPE_LOOP = 0xE0;
    static const uint8_t ENVELOPE_END = SEQ_END;
    static const uint8_t ENVELOPE_SUSTAIN = 0xE8;
    static const uint8_t ENVELOPE_SLIDE = 0xEA;

    static const int channels = 4;

    static const int recurseLimit = 64;  // way more than needed
    int readModRecurse;

 private:
    PaulaVoice _dummyVoices[channels];

    uint8_t *input;
    uint32_t inputLen;

    smartPtr<uint8_t> fcBuf;   // for safe unsigned access
    smartPtr<int8_t> fcBufS;  // for safe signed access

    // This array will be moved behind the input file. So don't forget
    // to allocate additional sizeof(..) bytes.
    static const uint8_t silenceData[8];

    // Index is AND 0x7f. Table is longer.
    static const uint16_t periods[(5+6)*12+4];

    static const uint16_t SMOD_waveInfo[47*4];
    static const uint8_t SMOD_waveforms[];

    struct Admin {
        uint16_t dmaFlags;  // which audio channels to turn on (AMIGA related)
        uint8_t count;     // speed count
        uint8_t speed;     // speed
        uint8_t RScount;
        bool initialized;  // true => restartable
        bool isEnabled;    // player on => true, else false
    
        struct _moduleOffsets {
            uint32_t trackTable;
            uint32_t patterns;
            uint32_t sndModSeqs;
            uint32_t volModSeqs;
            uint32_t silence;
        } offsets;

        int usedPatterns;
        int usedSndModSeqs;
        int usedVolModSeqs;
    } _admin;

    struct Sound {
        const uint8_t* start;
        uint16_t len, repOffs, repLen;
        // rest was place-holder (6 bytes)
    };
    // 10 samples/sample-packs
    // 80 waveforms
    Sound _sounds[10+80];

    struct CHdata
    {
        PaulaVoice *ch;  // paula and mixer interface
    
        uint16_t dmaMask;
    
        uint32_t trackStart;     // track/step pattern table
        uint32_t trackEnd;
        uint16_t trackPos;

        uint32_t pattStart;
        uint16_t pattPos;
    
        int8_t transpose;       // TR
        int8_t soundTranspose;  // ST
        int8_t seqTranspose;    // from sndModSeq
    
        uint8_t noteValue;
    
        int8_t pitchBendSpeed;
        uint8_t pitchBendTime, pitchBendDelayFlag;
    
        uint8_t portaInfo, portDelayFlag;
        int16_t portaOffs;
    
        uint32_t volSeq;
        uint16_t volSeqPos;
    
        uint8_t volSlideSpeed, volSlideTime, volSustainTime,
            volSlideDelayFlag;
    
        uint8_t envelopeSpeed, envelopeCount;
    
        uint32_t sndSeq;
        uint16_t sndSeqPos;
    
        uint8_t sndModSustainTime;
    
        uint8_t vibFlag, vibDelay, vibSpeed,
            vibAmpl, vibCurOffs;
    
        int8_t volume;
        uint16_t period;
    
        const uint8_t* pSampleStart;
        uint16_t repeatOffset;
        uint16_t repeatLength;
        uint16_t repeatDelay;
    };

    struct CHdata _CHdata[channels];

    void killChannel(CHdata&);
    void nextNote(CHdata&);
    void processModulation(CHdata&);
    void readModCommand(CHdata&);
    void readModCommand_recurse(CHdata&);
    void processPerVol(CHdata&);
    inline void setWave(CHdata&, uint8_t num);
    inline void readSeqTranspose(CHdata&);
    void volSlide(CHdata&);
};

#endif  // FC_H
