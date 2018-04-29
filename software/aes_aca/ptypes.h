#ifndef PTYPES_H_
#define PTYPES_H_

typedef unsigned char uint8;
typedef unsigned long uint32;

#define bswap32(x) (((x<<24)&0xFF000000)|((x<<8)&0x00FF0000)|(((uint32)x>>8)&0x0000FF00)|(((uint32)x>>24)&0x000000FF))
#define bswap16(x) (((x<<8)&0xFF00)|(((uint16)x>>8)&0x00FF))

#ifdef LITTLE_ENDIAN
#define int32_big(x) bswap32(x)
#define int16_big(x) bswap16(x)
#define int32_little(x) (x)
#define int16_little(x) (x)
#else
#define int32_big(x) (x)
#define int16_big(x) (x)
#define int32_little(x) bswap32(x)
#define int16_little(x) bswap16(x)
#endif

#endif /*PTYPES_H_*/
