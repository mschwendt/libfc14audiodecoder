// Future Composer & Hippel player library
// by Michael Schwendt
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

#include <algorithm>

#include "FC.h"
#include "Analyze.h"

void FC::TraitsByChecksum() {
    // If there's player machine code before the TFMX header, checksum it.
    if ( offsets.header != 0 ) {
        udword crc1 = 0;

        // Search for silence seq in player object,
        // because that is before the tons of voice variables.
        ubyte* pEnd = fcBuf.tellBegin()+offsets.header;
        const ubyte* pSilence = silenceData;
        const ubyte* r = std::search(fcBuf.tellBegin(),pEnd,pSilence,pSilence+sizeof(silenceData));
        if (r != pEnd) {
            udword silenceOffs = (r-fcBuf.tellBegin());
            crc1 = analyze->crc(fcBuf.tellBegin(),silenceOffs);
        }
            
        // Wings of Death  end, intro, outro, title
        // skip from sndmod sustain to volmod seq processing
        // the others skip to to wave modulation
        // audible in "outro" bass line beginning
        if (crc1 == 0x3bcb814b) {
            traits.skipToWaveMod = true;
        }
        else if (crc1 == 0x4c0a6454) {
            traits.skipToWaveMod = false;
        }
        if (crc1 == 0x3bcb814b || crc1 == 0x4c0a6454) {
            traits.vibScaling = false;
            TFMX_sndModFuncs[1] = &FC::TFMX_sndSeq_E1_waveMod;
            TFMX_sndModFuncs[5] = &FC::TFMX_sndSeq_E5;
            TFMX_sndModFuncs[6] = &FC::TFMX_sndSeq_E6;
            TFMX_sndModFuncs[7] = &FC::TFMX_sndSeq_E7_setDiffWave;
            TFMX_sndModFuncs[9] = &FC::TFMX_sndSeq_UNDEF;
        }

        // The Seven Gates of Jambala
        // Leaving Teramis
        // some others
        if (crc1 == 0xb6105cbf || crc1 == 0xb4f798e6 ||
            crc1 == 0xed5cfac0 ||
            crc1 == 0x6c55f30a ) {
            traits.sndSeqToTrans = true;
            traits.sndSeqGoto = false;
            TFMX_sndModFuncs[5] = &FC::TFMX_sndSeq_E5_repOffs;  // NB: but not used by those modules!
            TFMX_sndModFuncs[7] = &FC::TFMX_sndSeq_E7_setDiffWave;
        }
    }
}
