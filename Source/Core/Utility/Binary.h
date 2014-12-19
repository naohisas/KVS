/****************************************************************************/
/**
 *  @file Binary.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Binary.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__BINARY_H_INCLUDE
#define KVS__BINARY_H_INCLUDE

#define KVS_BINARY_0000_TO_HEX 0
#define KVS_BINARY_0001_TO_HEX 1
#define KVS_BINARY_0010_TO_HEX 2
#define KVS_BINARY_0011_TO_HEX 3
#define KVS_BINARY_0100_TO_HEX 4
#define KVS_BINARY_0101_TO_HEX 5
#define KVS_BINARY_0110_TO_HEX 6
#define KVS_BINARY_0111_TO_HEX 7
#define KVS_BINARY_1000_TO_HEX 8
#define KVS_BINARY_1001_TO_HEX 9
#define KVS_BINARY_1010_TO_HEX A
#define KVS_BINARY_1011_TO_HEX B
#define KVS_BINARY_1100_TO_HEX C
#define KVS_BINARY_1101_TO_HEX D
#define KVS_BINARY_1110_TO_HEX E
#define KVS_BINARY_1111_TO_HEX F

#define KVS_HEX_DIGIT(a)  KVS_BINARY_##a##_TO_HEX

#define KVS_BINARY1H(a)               (0x##a)
#define KVS_BINARY1I(a)               KVS_BINARY1H(a)

#define KVS_BINARY2H(a,b)             (0x##a##b)
#define KVS_BINARY2I(a,b)             KVS_BINARY2H(a,b)

#define KVS_BINARY3H(a,b,c)           (0x##a##b##c)
#define KVS_BINARY3I(a,b,c)           KVS_BINARY3H(a,b,c)

#define KVS_BINARY4H(a,b,c,d)         (0x##a##b##c##d)
#define KVS_BINARY4I(a,b,c,d)         KVS_BINARY4H(a,b,c,d)

#define KVS_BINARY8H(a,b,c,d,e,f,g,h) (0x##a##b##c##d##e##f##g##h)
#define KVS_BINARY8I(a,b,c,d,e,f,g,h) KVS_BINARY8H(a,b,c,d,e,f,g,h)


#define kvsBinary8(a,b) \
    KVS_BINARY2I( KVS_HEX_DIGIT(a), \
                  KVS_HEX_DIGIT(b) )

#define kvsBinary12(a,b,c) \
    KVS_BINARY3I( KVS_HEX_DIGIT(a), \
                  KVS_HEX_DIGIT(b), \
                  KVS_HEX_DIGIT(c) )

#define kvsBinary16(a,b,c,d) \
    KVS_BINARY4I( KVS_HEX_DIGIT(a), \
                  KVS_HEX_DIGIT(b), \
                  KVS_HEX_DIGIT(c), \
                  KVS_HEX_DIGIT(d) )

#define kvsBinary32(a,b,c,d,e,f,g,h) \
    KVS_BINARY8I( KVS_HEX_DIGIT(a), \
                  KVS_HEX_DIGIT(b), \
                  KVS_HEX_DIGIT(c), \
                  KVS_HEX_DIGIT(d), \
                  KVS_HEX_DIGIT(e), \
                  KVS_HEX_DIGIT(f), \
                  KVS_HEX_DIGIT(g), \
                  KVS_HEX_DIGIT(h) )

#endif // KVS__BINARY_H_INCLUDE
