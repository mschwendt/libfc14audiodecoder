// The soundtrack modules from the game "Chambers of Shaolin"
// as included with Future Composer v1.2 and v1.3 in SMOD format
// are broken, but have been copied into many module collections.

// Fix "SMOD.Chambers_of_Shaolin_3" (aka "Test of Fire").
// Also included in "fcm.shaolin.2-4" and "mod.shaolin.2-4".
//
// The drums in pattern 0x24 are misplaced! Correcting the pattern
// data is strictly required as to make it sound right. There is
// nothing else a FC player could do about it.
if ( _admin.usedPatterns >= 0x24 ) {
    const int n = 64;
    const ubyte pattOrig[n] = {
        0x0c, 0x01, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00,
        0x0c, 0x0c, 0x00, 0x00, 0x18, 0x0c, 0x00, 0x00,
        0x0c, 0x0c, 0x00, 0x00, 0x0a, 0x03, 0x00, 0x00,
        0x0c, 0x0c, 0x00, 0x00, 0x0a, 0x01, 0x00, 0x00,
        0x0a, 0x0c, 0x00, 0x00, 0x0f, 0x0c, 0x00, 0x00,
        0x0c, 0x0c, 0x00, 0x00, 0x11, 0x0c, 0x00, 0x00,
        0x0c, 0x0c, 0x00, 0x00, 0x13, 0x0c, 0x00, 0x00,
        0x14, 0x03, 0x00, 0x00, 0x14, 0x0c, 0x00, 0x00
    };
    const ubyte pattFixed[n] = {
        0x0c, 0x01, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00,
        0x18, 0x0c, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00,
        0x0a, 0x03, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00,
        0x0a, 0x01, 0x00, 0x00, 0x0f, 0x0c, 0x00, 0x00,
        0x0c, 0x0c, 0x00, 0x00, 0x11, 0x0c, 0x00, 0x00,
        0x0c, 0x0c, 0x00, 0x00, 0x13, 0x0c, 0x00, 0x00,
        0x0c, 0x03, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00,
        0x11, 0x0c, 0x00, 0x00, 0x0f, 0x0c, 0x00, 0x00
    };
    udword pattStart = _admin.offsets.patterns+(0x24<<6);
    if ( (pattStart+n) < inputLen ) {
        if ( memcmp(input+pattStart,&pattOrig,n) == 0 ) {
            // Replace the bad pattern.
            memcpy(input+pattStart,&pattFixed,n);
#ifdef DEBUG
            cout << "FIX APPLIED: Chambers of Shaolin 3" << endl;
#endif
        }
    }
}

// FIX "SMOD.Chambers_of_Shaolin_5" (aka "Test of Balance").
// Also included in "fcm.shaolin.5-6" and "mod.shaolin.5-6".
//
// What happens here is that at step 0x33 of the track table, the third
// channel not only changes to an empty pattern (which is not used anywhere
// else) but also resets transpose from 0xef to 0, which is audibly wrong.
// Since such a change of PT/TR is legal, the player cannot ignore it
// without breaking compatibility with other SMOD/FC14 modules.
if ( _admin.usedPatterns >= 0x1a ) {
    const int n = 64;
    const ubyte pattOrig1a[n] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    const ubyte pattOrig17[n] = {
     0x24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    udword pattStart = _admin.offsets.patterns+(0x1a<<6);
    if ( (pattStart+n) < inputLen ) {
        if ( memcmp(input+pattStart,&pattOrig1a,n) == 0 ) {
            pattStart = _admin.offsets.patterns+(0x17<<6);
            if ( memcmp(input+pattStart,&pattOrig17,n) == 0 ) {

                const int t = 2*TRACKTAB_ENTRY_LENGTH;
                const ubyte trackTabOrig[t] = {
                    0x12, 0x0c, 0x04, 0x14, 0xf4, 0x00, 0x1a, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
                    0x16, 0x0c, 0x04, 0x15, 0xf4, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00
                };
                const ubyte trackTabFixed[t] = {
                    0x12, 0x0c, 0x04, 0x14, 0xf4, 0x00, 0x17, 0x07, 0x0c, 0x00, 0x00, 0x00, 0x00,
                    0x16, 0x0c, 0x04, 0x15, 0xf4, 0x00, 0x00, 0x07, 0x0c, 0x00, 0x00, 0x00, 0x00
                };
                udword trackTabPos = _admin.offsets.trackTable+(0x33*TRACKTAB_ENTRY_LENGTH);
                if ( memcmp(input+trackTabPos,&trackTabOrig,t) == 0 ) {
                    // Replace the bad/dubious track table entries.
                    memcpy(input+trackTabPos,&trackTabFixed,t);
#ifdef DEBUG
                    cout << "FIX APPLIED: Chambers of Shaolin 5" << endl;
#endif
                }
            }
        }
    }
}
