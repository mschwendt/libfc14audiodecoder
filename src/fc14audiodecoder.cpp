// C language wrapper library for Future Composer audio decoder
// Copyright (C) 2008 Michael Schwendt
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

#include "fc14audiodecoder.h"

#include "FC.h"
#include "LamePaula.h"

#define FC14_DECLARE_DECODER \
    fc14dec *p = (fc14dec*)ptr

struct fc14dec {
    FC decoder;
    LamePaulaMixer mixer;
};

void* fc14dec_new() {
    fc14dec *p = new fc14dec;
    p->decoder.setMixer(&p->mixer);
    return (void*)p;
}

void fc14dec_delete(void* ptr) {
    FC14_DECLARE_DECODER;
    delete p;
}

int fc14dec_detect(void* ptr, void* data, unsigned long int length) {
    FC14_DECLARE_DECODER;
    return p->decoder.isOurData(data,length);
}

int fc14dec_init(void* ptr, void* data, unsigned long int length) {
    FC14_DECLARE_DECODER;
    return p->decoder.init(data,length);
}

int fc14dec_restart(void* ptr) {
    FC14_DECLARE_DECODER;
    return p->decoder.restart();
}

void fc14dec_seek(void* ptr, long int ms) {
    FC14_DECLARE_DECODER;
    if ( !p->decoder.restart() ) {
        return;
    }
    while (ms>=0) {
        p->decoder.run();
        ms -= 20;
        if ( fc14dec_song_end(p) ) {
            break;
        }
    };
}

void fc14dec_mixer_init(void* ptr, int freq, int bits, int channels, int zero) {
    FC14_DECLARE_DECODER;
    p->mixer.init(freq,bits,channels,zero);
}

void fc14dec_buffer_fill(void* ptr, void* buffer, unsigned long int length) {
    FC14_DECLARE_DECODER;
    p->mixer.fillBuffer(buffer,length,&p->decoder);
}

int fc14dec_song_end(void* ptr) {
    FC14_DECLARE_DECODER;
    return p->decoder.songEnd;
}

unsigned long int fc14dec_duration(void* ptr) {
    FC14_DECLARE_DECODER;
    // Determine duration with a dry-run till song-end.
    unsigned long int ms = 0;
    if ( !p->decoder.restart() ) {
        return ms;
    }
    do {
        p->decoder.run();
        ms += 20;
    } while ( !fc14dec_song_end(p) );
    p->decoder.restart();
    return ms;
}

const char* fc14dec_format_name(void* ptr) {
    FC14_DECLARE_DECODER;
    return p->decoder.formatName.c_str();
}

#ifdef FC_API_EXT_1
int fc14dec_get_used_patterns(void* ptr) {
    FC14_DECLARE_DECODER;
    return p->decoder.getUsedPatterns();
}

int fc14dec_get_used_snd_mod_seqs(void* ptr) {
    FC14_DECLARE_DECODER;
    return p->decoder.getUsedSndModSeqs();
}

int fc14dec_get_used_vol_mod_seqs(void* ptr) {
    FC14_DECLARE_DECODER;
    return p->decoder.getUsedVolModSeqs();
}

int fc14dec_get_sample_length(void* ptr, unsigned int num) {
    FC14_DECLARE_DECODER;
    return p->decoder.getSampleLength(num);
}

int fc14dec_get_sample_rep_offset(void* ptr, unsigned int num) {
    FC14_DECLARE_DECODER;
    return p->decoder.getSampleRepOffset(num);
}

int fc14dec_get_sample_rep_length(void* ptr, unsigned int num) {
    FC14_DECLARE_DECODER;
    return p->decoder.getSampleRepLength(num);
}

void fc14dec_mute_channel(void* ptr, bool mute, unsigned int channel) {
    FC14_DECLARE_DECODER;
    p->mixer.mute(channel,mute);
}

unsigned short int fc14dec_get_channel_volume(void* ptr, unsigned int channel) {
    FC14_DECLARE_DECODER;
    if ( !p->mixer.isMuted(channel) ) {
        return (p->mixer.getVoice(channel)->paula.volume/64.0)*100;
    }
    else {
        return 0;
    }
}
#endif
