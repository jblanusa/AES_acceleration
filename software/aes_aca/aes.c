/*
   Copyright 2002 Colin Percival and the University of Oxford

   Copyright in this software is held by the University of Oxford
   and Colin Percival. Both the University of Oxford and Colin Percival
   have agreed that this software may be distributed free of charge and
   in accordance with the attached licence.

   The above notwithstanding, in order that the University as a
   charitable foundation protects its assets for the benefit of its
   educational and research purposes, the University makes clear that no
   condition is made or to be implied, nor is any warranty given or to
   be implied, as to the proper functioning of this work, or that it
   will be suitable for any particular purpose or for use under any
   specific conditions.  Furthermore, the University disclaims all
   responsibility for any use which is made of this work.

   For the terms under which this work may be distributed, please see
   the adjoining file "LICENSE".
   */

#include "ptypes.h"
#include "aestab.h"
#include "aes.h"


// These defines are for the non-accelerated code and key expansion routines
#define Nb 4            // number of columns in the state & expanded key

#define Nk 4            // number of columns in a key
#define Nr 10           // number of rounds in encryption

// Local helper functions
void ShiftRows (uint8 *state);

void InvShiftRows (uint8 *state);
void InvMixSubColumns (uint8 *state);


//-------------------------------------------------------------------------------
// Key expansion routines...the two produce identical output, you may use either

void aes_keyexpand128(uint8* key, uint8* keyexp)
{
    uint32 i;
    uint8 Rcon;

    *(uint32 *)(keyexp   ) = *(uint32 *)(key   );
    *(uint32 *)(keyexp+4 ) = *(uint32 *)(key+4 );
    *(uint32 *)(keyexp+8 ) = *(uint32 *)(key+8 );
    *(uint32 *)(keyexp+12) = *(uint32 *)(key+12);

    Rcon = 0x01;

    for(i=1;i<11;i++) {
        *(keyexp+i*16  )=*(keyexp+i*16-16)^Sbox[*(keyexp+i*16-3)]
            ^Rcon;
        *(keyexp+i*16+1)=*(keyexp+i*16-15)^Sbox[*(keyexp+i*16-2)];
        *(keyexp+i*16+2)=*(keyexp+i*16-14)^Sbox[*(keyexp+i*16-1)];
        *(keyexp+i*16+3)=*(keyexp+i*16-13)^Sbox[*(keyexp+i*16-4)];
        Rcon=((Rcon<<1)&0xFE)^
            ((Rcon>>3)&0x10)^
            ((Rcon>>4)&0x08)^
            ((Rcon>>6)&0x02)^
            ((Rcon>>7));
        *(uint32 *)(keyexp+i*16+4) =*(uint32 *)(keyexp+i*16-12)
            ^*(uint32 *)(keyexp+i*16   );
        *(uint32 *)(keyexp+i*16+8) =*(uint32 *)(keyexp+i*16-8 )
            ^*(uint32 *)(keyexp+i*16+4 );
        *(uint32 *)(keyexp+i*16+12)=*(uint32 *)(keyexp+i*16-4 )
            ^*(uint32 *)(keyexp+i*16+8 );
    };
}



// produce Nb bytes for each round
void ExpandKey (uint8* key, uint8* expkey)
{
    int i;
    uint8 tmp0, tmp1, tmp2, tmp3, tmp4;
    unsigned idx;
    uint8 Rcon_[11] = {
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

    for(i = 0; i < Nk*4; i++)  // Ugly substitute for memcpy
        expkey[i] = key[i];

    //		   4         4*11
    for( idx = Nk; idx < Nb * (Nr + 1); idx++ ) {
        tmp0 = expkey[4*idx - 4];
        tmp1 = expkey[4*idx - 3];
        tmp2 = expkey[4*idx - 2];
        tmp3 = expkey[4*idx - 1];
        if( !(idx % Nk) ) {
            tmp4 = tmp3;
            tmp3 = Sbox[tmp0];
            tmp0 = Sbox[tmp1] ^ Rcon_[idx/Nk];
            tmp1 = Sbox[tmp2];
            tmp2 = Sbox[tmp4];
        } else if( Nk > 6 && idx % Nk == 4 ) {
            tmp0 = Sbox[tmp0];
            tmp1 = Sbox[tmp1];
            tmp2 = Sbox[tmp2];
            tmp3 = Sbox[tmp3];
        }

        expkey[4*idx+0] = expkey[4*idx - 4*Nk + 0] ^ tmp0;
        expkey[4*idx+1] = expkey[4*idx - 4*Nk + 1] ^ tmp1;
        expkey[4*idx+2] = expkey[4*idx - 4*Nk + 2] ^ tmp2;
        expkey[4*idx+3] = expkey[4*idx - 4*Nk + 3] ^ tmp3;
    }
}



//-------------------------------------------------------------------------------
// Software-accelerated block encryption routine
void aes_blockenc_SWacc(uint8* src, uint8* dest, uint8* keyexp)
{
    uint32 rounds = 10; //aes_128;
    uint32 i;
    uint32 temp[16];
    *(uint32 *)(dest   )=*(uint32 *)(src   )^*(uint32 *)(keyexp   );
    *(uint32 *)(dest+4 )=*(uint32 *)(src+4 )^*(uint32 *)(keyexp+4 );
    *(uint32 *)(dest+8 )=*(uint32 *)(src+8 )^*(uint32 *)(keyexp+8 );
    *(uint32 *)(dest+12)=*(uint32 *)(src+12)^*(uint32 *)(keyexp+12);
    for(i=1;i<rounds;i++) {
        *(uint32 *)(temp  )=*(uint32 *)(aestab_SBox0+*(dest   )*4)^
            *(uint32 *)(aestab_SBox1+*(dest+5 )*4)^
            *(uint32 *)(aestab_SBox2+*(dest+10)*4)^
            *(uint32 *)(aestab_SBox3+*(dest+15)*4);
        *(uint32 *)(temp+1)=*(uint32 *)(aestab_SBox0+*(dest+4 )*4)^
            *(uint32 *)(aestab_SBox1+*(dest+9 )*4)^
            *(uint32 *)(aestab_SBox2+*(dest+14)*4)^
            *(uint32 *)(aestab_SBox3+*(dest+3 )*4);
        *(uint32 *)(temp+2)=*(uint32 *)(aestab_SBox0+*(dest+8 )*4)^
            *(uint32 *)(aestab_SBox1+*(dest+13)*4)^
            *(uint32 *)(aestab_SBox2+*(dest+2 )*4)^
            *(uint32 *)(aestab_SBox3+*(dest+7 )*4);
        *(uint32 *)(temp+3)=*(uint32 *)(aestab_SBox0+*(dest+12)*4)^
            *(uint32 *)(aestab_SBox1+*(dest+1 )*4)^
            *(uint32 *)(aestab_SBox2+*(dest+6 )*4)^
            *(uint32 *)(aestab_SBox3+*(dest+11)*4);
        *(uint32 *)(dest   )=*(uint32 *)(temp  )^
            *(uint32 *)(keyexp+i*16   );
        *(uint32 *)(dest+4 )=*(uint32 *)(temp+1)^
            *(uint32 *)(keyexp+i*16+4 );
        *(uint32 *)(dest+8 )=*(uint32 *)(temp+2)^
            *(uint32 *)(keyexp+i*16+8 );
        *(uint32 *)(dest+12)=*(uint32 *)(temp+3)^
            *(uint32 *)(keyexp+i*16+12);
    };

    for(i=0;i<16;i++)
        ((uint8 *)temp)[i]=Sbox[*(dest+((i*5)&0x0f))];

    *(uint32 *)(dest   )=*(uint32 *)(temp  )^
        *(uint32 *)(keyexp+rounds*16   );
    *(uint32 *)(dest+4 )=*(uint32 *)(temp+1)^
        *(uint32 *)(keyexp+rounds*16+4 );
    *(uint32 *)(dest+8 )=*(uint32 *)(temp+2)^
        *(uint32 *)(keyexp+rounds*16+8 );
    *(uint32 *)(dest+12)=*(uint32 *)(temp+3)^
        *(uint32 *)(keyexp+rounds*16+12);

    *(uint32 *)(temp  )=0;
    *(uint32 *)(temp+1)=0;
    *(uint32 *)(temp+2)=0;
    *(uint32 *)(temp+3)=0;
}


//------------------------------------------------------------------------
// Un-accelerated block encryption
void aes_blockenc_NOacc(uint8* in, uint8* out, uint8* expkey)
{
    uint8 i;
    unsigned round;

    for(i = 0; i < Nb*4; i++)  // ugly substitute for memcpy
        out[i] = in[i];

    AddRoundKey ((unsigned *)out, (unsigned *)expkey);

    for( round = 1; round < Nr + 1; round++ ) {
        if( round < Nr )
            MixSubColumns (out); // Includes SubBytes and ShiftRows
        else
            ShiftRows (out); // Includes SubBytes

        AddRoundKey ((unsigned *)out, (unsigned *)expkey + round * Nb);
    }
}

void AddRoundKey (unsigned *state, unsigned *key)
{
    int idx;

    for( idx = 0; idx < 4; idx++ )  // This may also be done byte-at-a-time
        state[idx] ^= key[idx];
}
//
// This combines SubBytes and ShiftRows steps
void ShiftRows (uint8 *state)
{
    uint8 tmp;

    // just substitute row 0
    state[0] = Sbox[state[0]]; 
    state[4] = Sbox[state[4]];
    state[8] = Sbox[state[8]];
    state[12] = Sbox[state[12]];

    // rotate row 1
    tmp = Sbox[state[1]];
    state[1] = Sbox[state[5]];
    state[5] = Sbox[state[9]];
    state[9] = Sbox[state[13]];
    state[13] = tmp;

    // rotate row 2
    tmp = Sbox[state[2]];
    state[2] = Sbox[state[10]];
    state[10] = tmp;
    tmp = Sbox[state[6]];
    state[6] = Sbox[state[14]];
    state[14] = tmp;

    // rotate row 3
    tmp = Sbox[state[15]];
    state[15] = Sbox[state[11]];
    state[11] = Sbox[state[7]];
    state[7] = Sbox[state[3]];
    state[3] = tmp;
}

// Tis combines SubBytes, ShiftRows, and MixColumns
void MixSubColumns (uint8 *state)
{
    uint8 tmp[4 * Nb];
    uint8 i;

    // mixing column 0
    tmp[0] = Xtime2Sbox[state[0]] ^ Xtime3Sbox[state[5]] ^ Sbox[state[10]] ^ Sbox[state[15]];
    tmp[1] = Sbox[state[0]] ^ Xtime2Sbox[state[5]] ^ Xtime3Sbox[state[10]] ^ Sbox[state[15]];
    tmp[2] = Sbox[state[0]] ^ Sbox[state[5]] ^ Xtime2Sbox[state[10]] ^ Xtime3Sbox[state[15]];
    tmp[3] = Xtime3Sbox[state[0]] ^ Sbox[state[5]] ^ Sbox[state[10]] ^ Xtime2Sbox[state[15]];

    // mixing column 1
    tmp[4] = Xtime2Sbox[state[4]] ^ Xtime3Sbox[state[9]] ^ Sbox[state[14]] ^ Sbox[state[3]];
    tmp[5] = Sbox[state[4]] ^ Xtime2Sbox[state[9]] ^ Xtime3Sbox[state[14]] ^ Sbox[state[3]];
    tmp[6] = Sbox[state[4]] ^ Sbox[state[9]] ^ Xtime2Sbox[state[14]] ^ Xtime3Sbox[state[3]];
    tmp[7] = Xtime3Sbox[state[4]] ^ Sbox[state[9]] ^ Sbox[state[14]] ^ Xtime2Sbox[state[3]];

    // mixing column 2
    tmp[8] = Xtime2Sbox[state[8]] ^ Xtime3Sbox[state[13]] ^ Sbox[state[2]] ^ Sbox[state[7]];
    tmp[9] = Sbox[state[8]] ^ Xtime2Sbox[state[13]] ^ Xtime3Sbox[state[2]] ^ Sbox[state[7]];
    tmp[10]  = Sbox[state[8]] ^ Sbox[state[13]] ^ Xtime2Sbox[state[2]] ^ Xtime3Sbox[state[7]];
    tmp[11]  = Xtime3Sbox[state[8]] ^ Sbox[state[13]] ^ Sbox[state[2]] ^ Xtime2Sbox[state[7]];

    // mixing column 3
    tmp[12] = Xtime2Sbox[state[12]] ^ Xtime3Sbox[state[1]] ^ Sbox[state[6]] ^ Sbox[state[11]];
    tmp[13] = Sbox[state[12]] ^ Xtime2Sbox[state[1]] ^ Xtime3Sbox[state[6]] ^ Sbox[state[11]];
    tmp[14] = Sbox[state[12]] ^ Sbox[state[1]] ^ Xtime2Sbox[state[6]] ^ Xtime3Sbox[state[11]];
    tmp[15] = Xtime3Sbox[state[12]] ^ Sbox[state[1]] ^ Sbox[state[6]] ^ Xtime2Sbox[state[11]];

    //memcpy (state, tmp, sizeof(tmp));
    for(i = 0; i < Nb*4; i++)
        state[i] = tmp[i];
}


//-------------------------------------------------------------------------------------
// Decryption routines for correctness testing
void Decrypt (uint8 *in, uint8 *expkey, uint8 *out)
{
    uint8 i;
    unsigned round;

    for(i = 0; i < Nb*4; i++)  // ugly substitute for memcpy
        out[i] = in[i];

    AddRoundKey ((unsigned *)out, (unsigned *)expkey + Nr * Nb);
    InvShiftRows(out);

    for( round = Nr; round--; )
    {
        AddRoundKey ((unsigned *)out, (unsigned *)expkey + round * Nb);
        if( round )
            InvMixSubColumns (out);
    } 
}

void InvShiftRows (uint8 *state)
{
    uint8 tmp;

    // restore row 0
    state[0] = InvSbox[state[0]];
    state[4] = InvSbox[state[4]];
    state[8] = InvSbox[state[8]];
    state[12] = InvSbox[state[12]];

    // restore row 1
    tmp = InvSbox[state[13]];
    state[13] = InvSbox[state[9]];
    state[9] = InvSbox[state[5]];
    state[5] = InvSbox[state[1]];
    state[1] = tmp;

    // restore row 2
    tmp = InvSbox[state[2]];
    state[2] = InvSbox[state[10]];
    state[10] = tmp;
    tmp = InvSbox[state[6]];
    state[6] = InvSbox[state[14]];
    state[14] = tmp;

    // restore row 3
    tmp = InvSbox[state[3]];
    state[3] = InvSbox[state[7]];
    state[7] = InvSbox[state[11]];
    state[11] = InvSbox[state[15]];
    state[15] = tmp;
}

// restore and un-mix each row in a column
void InvMixSubColumns (uint8 *state)
{
    uint8 tmp[4 * Nb];
    int i;

    // restore column 0
    tmp[0] = XtimeE[state[0]] ^ XtimeB[state[1]] ^ XtimeD[state[2]] ^ Xtime9[state[3]];
    tmp[5] = Xtime9[state[0]] ^ XtimeE[state[1]] ^ XtimeB[state[2]] ^ XtimeD[state[3]];
    tmp[10] = XtimeD[state[0]] ^ Xtime9[state[1]] ^ XtimeE[state[2]] ^ XtimeB[state[3]];
    tmp[15] = XtimeB[state[0]] ^ XtimeD[state[1]] ^ Xtime9[state[2]] ^ XtimeE[state[3]];

    // restore column 1
    tmp[4] = XtimeE[state[4]] ^ XtimeB[state[5]] ^ XtimeD[state[6]] ^ Xtime9[state[7]];
    tmp[9] = Xtime9[state[4]] ^ XtimeE[state[5]] ^ XtimeB[state[6]] ^ XtimeD[state[7]];
    tmp[14] = XtimeD[state[4]] ^ Xtime9[state[5]] ^ XtimeE[state[6]] ^ XtimeB[state[7]];
    tmp[3] = XtimeB[state[4]] ^ XtimeD[state[5]] ^ Xtime9[state[6]] ^ XtimeE[state[7]];

    // restore column 2
    tmp[8] = XtimeE[state[8]] ^ XtimeB[state[9]] ^ XtimeD[state[10]] ^ Xtime9[state[11]];
    tmp[13] = Xtime9[state[8]] ^ XtimeE[state[9]] ^ XtimeB[state[10]] ^ XtimeD[state[11]];
    tmp[2]  = XtimeD[state[8]] ^ Xtime9[state[9]] ^ XtimeE[state[10]] ^ XtimeB[state[11]];
    tmp[7]  = XtimeB[state[8]] ^ XtimeD[state[9]] ^ Xtime9[state[10]] ^ XtimeE[state[11]];

    // restore column 3
    tmp[12] = XtimeE[state[12]] ^ XtimeB[state[13]] ^ XtimeD[state[14]] ^ Xtime9[state[15]];
    tmp[1] = Xtime9[state[12]] ^ XtimeE[state[13]] ^ XtimeB[state[14]] ^ XtimeD[state[15]];
    tmp[6] = XtimeD[state[12]] ^ Xtime9[state[13]] ^ XtimeE[state[14]] ^ XtimeB[state[15]];
    tmp[11] = XtimeB[state[12]] ^ XtimeD[state[13]] ^ Xtime9[state[14]] ^ XtimeE[state[15]];

    for( i=0; i < 4 * Nb; i++ )
        state[i] = InvSbox[tmp[i]];
}

