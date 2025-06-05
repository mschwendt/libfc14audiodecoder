// This is only an excerpt of a header originally from libsidplay.
// Since libfc14audiodecoder uses a smart pointer implementation,
// and all FC modules used big-endian byte order, only the readEndian
// functions are needed.

#ifndef MYENDIAN_H
#define MYENDIAN_H

#include "Config.h"
#include "MyTypes.h"

// Convert high-byte and low-byte to 16-bit word.
// Used to read 16-bit words in little-endian order.
inline uword readEndian(ubyte hi, ubyte lo)
{
  return(( (uword)hi << 8 ) + (uword)lo );
}

// Convert high bytes and low bytes of MSW and LSW to 32-bit word.
// Used to read 32-bit words in little-endian order.
inline udword readEndian(ubyte hihi, ubyte hilo, ubyte hi, ubyte lo)
{
  return(( (udword)hihi << 24 ) + ( (udword)hilo << 16 ) + 
		 ( (udword)hi << 8 ) + (udword)lo );
}

#endif  // MYENDIAN_H
