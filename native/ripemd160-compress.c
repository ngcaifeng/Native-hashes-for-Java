/* 
 * Native hash functions for Java
 * 
 * Copyright (c) Project Nayuki
 * http://www.nayuki.io/page/native-hash-functions-for-java
 */

#include <stdint.h>
#include <jni.h>


void ripemd160_compress_block(const jbyte *block, uint32_t state[5]) {
	#define LOADSCHEDULE(i)  \
		schedule[i] =                                    \
			  (uint32_t)(uint8_t)block[i * 4 + 0]        \
			| (uint32_t)(uint8_t)block[i * 4 + 1] <<  8  \
			| (uint32_t)(uint8_t)block[i * 4 + 2] << 16  \
			| (uint32_t)(uint8_t)block[i * 4 + 3] << 24;
	
	uint32_t schedule[16];
	LOADSCHEDULE( 0)
	LOADSCHEDULE( 1)
	LOADSCHEDULE( 2)
	LOADSCHEDULE( 3)
	LOADSCHEDULE( 4)
	LOADSCHEDULE( 5)
	LOADSCHEDULE( 6)
	LOADSCHEDULE( 7)
	LOADSCHEDULE( 8)
	LOADSCHEDULE( 9)
	LOADSCHEDULE(10)
	LOADSCHEDULE(11)
	LOADSCHEDULE(12)
	LOADSCHEDULE(13)
	LOADSCHEDULE(14)
	LOADSCHEDULE(15)
	
	#define ROTL32(x, n)  (((x) << (n)) | ((x) >> (32 - (n))))  // Assumes that x is uint32_t and 0 < n < 32
	#define ROUND0(a, b, c, d, e, k, r, s)  ROUND(a, b, c, d, e, k, r, s, (b ^ c ^ d))
	#define ROUND1(a, b, c, d, e, k, r, s)  ROUND(a, b, c, d, e, k, r, s, ((b & c) | (~b & d)))
	#define ROUND2(a, b, c, d, e, k, r, s)  ROUND(a, b, c, d, e, k, r, s, ((b | ~c) ^ d))
	#define ROUND3(a, b, c, d, e, k, r, s)  ROUND(a, b, c, d, e, k, r, s, ((b & d) | (c & ~d)))
	#define ROUND4(a, b, c, d, e, k, r, s)  ROUND(a, b, c, d, e, k, r, s, (b ^ (c | ~d)))
	
	#define ROUND(a, b, c, d, e, k, r, s, f)  \
		a += f + schedule[r] + UINT32_C(k);  \
		a = ROTL32(a, s) + e;  \
		c = ROTL32(c, 10);
	
	uint32_t al = state[0], ar = state[0];
	uint32_t bl = state[1], br = state[1];
	uint32_t cl = state[2], cr = state[2];
	uint32_t dl = state[3], dr = state[3];
	uint32_t el = state[4], er = state[4];
	
	ROUND0(al, bl, cl, dl, el, 0x00000000,  0, 11)
	ROUND0(el, al, bl, cl, dl, 0x00000000,  1, 14)
	ROUND0(dl, el, al, bl, cl, 0x00000000,  2, 15)
	ROUND0(cl, dl, el, al, bl, 0x00000000,  3, 12)
	ROUND0(bl, cl, dl, el, al, 0x00000000,  4,  5)
	ROUND0(al, bl, cl, dl, el, 0x00000000,  5,  8)
	ROUND0(el, al, bl, cl, dl, 0x00000000,  6,  7)
	ROUND0(dl, el, al, bl, cl, 0x00000000,  7,  9)
	ROUND0(cl, dl, el, al, bl, 0x00000000,  8, 11)
	ROUND0(bl, cl, dl, el, al, 0x00000000,  9, 13)
	ROUND0(al, bl, cl, dl, el, 0x00000000, 10, 14)
	ROUND0(el, al, bl, cl, dl, 0x00000000, 11, 15)
	ROUND0(dl, el, al, bl, cl, 0x00000000, 12,  6)
	ROUND0(cl, dl, el, al, bl, 0x00000000, 13,  7)
	ROUND0(bl, cl, dl, el, al, 0x00000000, 14,  9)
	ROUND0(al, bl, cl, dl, el, 0x00000000, 15,  8)
	ROUND1(el, al, bl, cl, dl, 0x5A827999,  7,  7)
	ROUND1(dl, el, al, bl, cl, 0x5A827999,  4,  6)
	ROUND1(cl, dl, el, al, bl, 0x5A827999, 13,  8)
	ROUND1(bl, cl, dl, el, al, 0x5A827999,  1, 13)
	ROUND1(al, bl, cl, dl, el, 0x5A827999, 10, 11)
	ROUND1(el, al, bl, cl, dl, 0x5A827999,  6,  9)
	ROUND1(dl, el, al, bl, cl, 0x5A827999, 15,  7)
	ROUND1(cl, dl, el, al, bl, 0x5A827999,  3, 15)
	ROUND1(bl, cl, dl, el, al, 0x5A827999, 12,  7)
	ROUND1(al, bl, cl, dl, el, 0x5A827999,  0, 12)
	ROUND1(el, al, bl, cl, dl, 0x5A827999,  9, 15)
	ROUND1(dl, el, al, bl, cl, 0x5A827999,  5,  9)
	ROUND1(cl, dl, el, al, bl, 0x5A827999,  2, 11)
	ROUND1(bl, cl, dl, el, al, 0x5A827999, 14,  7)
	ROUND1(al, bl, cl, dl, el, 0x5A827999, 11, 13)
	ROUND1(el, al, bl, cl, dl, 0x5A827999,  8, 12)
	ROUND2(dl, el, al, bl, cl, 0x6ED9EBA1,  3, 11)
	ROUND2(cl, dl, el, al, bl, 0x6ED9EBA1, 10, 13)
	ROUND2(bl, cl, dl, el, al, 0x6ED9EBA1, 14,  6)
	ROUND2(al, bl, cl, dl, el, 0x6ED9EBA1,  4,  7)
	ROUND2(el, al, bl, cl, dl, 0x6ED9EBA1,  9, 14)
	ROUND2(dl, el, al, bl, cl, 0x6ED9EBA1, 15,  9)
	ROUND2(cl, dl, el, al, bl, 0x6ED9EBA1,  8, 13)
	ROUND2(bl, cl, dl, el, al, 0x6ED9EBA1,  1, 15)
	ROUND2(al, bl, cl, dl, el, 0x6ED9EBA1,  2, 14)
	ROUND2(el, al, bl, cl, dl, 0x6ED9EBA1,  7,  8)
	ROUND2(dl, el, al, bl, cl, 0x6ED9EBA1,  0, 13)
	ROUND2(cl, dl, el, al, bl, 0x6ED9EBA1,  6,  6)
	ROUND2(bl, cl, dl, el, al, 0x6ED9EBA1, 13,  5)
	ROUND2(al, bl, cl, dl, el, 0x6ED9EBA1, 11, 12)
	ROUND2(el, al, bl, cl, dl, 0x6ED9EBA1,  5,  7)
	ROUND2(dl, el, al, bl, cl, 0x6ED9EBA1, 12,  5)
	ROUND3(cl, dl, el, al, bl, 0x8F1BBCDC,  1, 11)
	ROUND3(bl, cl, dl, el, al, 0x8F1BBCDC,  9, 12)
	ROUND3(al, bl, cl, dl, el, 0x8F1BBCDC, 11, 14)
	ROUND3(el, al, bl, cl, dl, 0x8F1BBCDC, 10, 15)
	ROUND3(dl, el, al, bl, cl, 0x8F1BBCDC,  0, 14)
	ROUND3(cl, dl, el, al, bl, 0x8F1BBCDC,  8, 15)
	ROUND3(bl, cl, dl, el, al, 0x8F1BBCDC, 12,  9)
	ROUND3(al, bl, cl, dl, el, 0x8F1BBCDC,  4,  8)
	ROUND3(el, al, bl, cl, dl, 0x8F1BBCDC, 13,  9)
	ROUND3(dl, el, al, bl, cl, 0x8F1BBCDC,  3, 14)
	ROUND3(cl, dl, el, al, bl, 0x8F1BBCDC,  7,  5)
	ROUND3(bl, cl, dl, el, al, 0x8F1BBCDC, 15,  6)
	ROUND3(al, bl, cl, dl, el, 0x8F1BBCDC, 14,  8)
	ROUND3(el, al, bl, cl, dl, 0x8F1BBCDC,  5,  6)
	ROUND3(dl, el, al, bl, cl, 0x8F1BBCDC,  6,  5)
	ROUND3(cl, dl, el, al, bl, 0x8F1BBCDC,  2, 12)
	ROUND4(bl, cl, dl, el, al, 0xA953FD4E,  4,  9)
	ROUND4(al, bl, cl, dl, el, 0xA953FD4E,  0, 15)
	ROUND4(el, al, bl, cl, dl, 0xA953FD4E,  5,  5)
	ROUND4(dl, el, al, bl, cl, 0xA953FD4E,  9, 11)
	ROUND4(cl, dl, el, al, bl, 0xA953FD4E,  7,  6)
	ROUND4(bl, cl, dl, el, al, 0xA953FD4E, 12,  8)
	ROUND4(al, bl, cl, dl, el, 0xA953FD4E,  2, 13)
	ROUND4(el, al, bl, cl, dl, 0xA953FD4E, 10, 12)
	ROUND4(dl, el, al, bl, cl, 0xA953FD4E, 14,  5)
	ROUND4(cl, dl, el, al, bl, 0xA953FD4E,  1, 12)
	ROUND4(bl, cl, dl, el, al, 0xA953FD4E,  3, 13)
	ROUND4(al, bl, cl, dl, el, 0xA953FD4E,  8, 14)
	ROUND4(el, al, bl, cl, dl, 0xA953FD4E, 11, 11)
	ROUND4(dl, el, al, bl, cl, 0xA953FD4E,  6,  8)
	ROUND4(cl, dl, el, al, bl, 0xA953FD4E, 15,  5)
	ROUND4(bl, cl, dl, el, al, 0xA953FD4E, 13,  6)
	
	ROUND4(ar, br, cr, dr, er, 0x50A28BE6,  5,  8)
	ROUND4(er, ar, br, cr, dr, 0x50A28BE6, 14,  9)
	ROUND4(dr, er, ar, br, cr, 0x50A28BE6,  7,  9)
	ROUND4(cr, dr, er, ar, br, 0x50A28BE6,  0, 11)
	ROUND4(br, cr, dr, er, ar, 0x50A28BE6,  9, 13)
	ROUND4(ar, br, cr, dr, er, 0x50A28BE6,  2, 15)
	ROUND4(er, ar, br, cr, dr, 0x50A28BE6, 11, 15)
	ROUND4(dr, er, ar, br, cr, 0x50A28BE6,  4,  5)
	ROUND4(cr, dr, er, ar, br, 0x50A28BE6, 13,  7)
	ROUND4(br, cr, dr, er, ar, 0x50A28BE6,  6,  7)
	ROUND4(ar, br, cr, dr, er, 0x50A28BE6, 15,  8)
	ROUND4(er, ar, br, cr, dr, 0x50A28BE6,  8, 11)
	ROUND4(dr, er, ar, br, cr, 0x50A28BE6,  1, 14)
	ROUND4(cr, dr, er, ar, br, 0x50A28BE6, 10, 14)
	ROUND4(br, cr, dr, er, ar, 0x50A28BE6,  3, 12)
	ROUND4(ar, br, cr, dr, er, 0x50A28BE6, 12,  6)
	ROUND3(er, ar, br, cr, dr, 0x5C4DD124,  6,  9)
	ROUND3(dr, er, ar, br, cr, 0x5C4DD124, 11, 13)
	ROUND3(cr, dr, er, ar, br, 0x5C4DD124,  3, 15)
	ROUND3(br, cr, dr, er, ar, 0x5C4DD124,  7,  7)
	ROUND3(ar, br, cr, dr, er, 0x5C4DD124,  0, 12)
	ROUND3(er, ar, br, cr, dr, 0x5C4DD124, 13,  8)
	ROUND3(dr, er, ar, br, cr, 0x5C4DD124,  5,  9)
	ROUND3(cr, dr, er, ar, br, 0x5C4DD124, 10, 11)
	ROUND3(br, cr, dr, er, ar, 0x5C4DD124, 14,  7)
	ROUND3(ar, br, cr, dr, er, 0x5C4DD124, 15,  7)
	ROUND3(er, ar, br, cr, dr, 0x5C4DD124,  8, 12)
	ROUND3(dr, er, ar, br, cr, 0x5C4DD124, 12,  7)
	ROUND3(cr, dr, er, ar, br, 0x5C4DD124,  4,  6)
	ROUND3(br, cr, dr, er, ar, 0x5C4DD124,  9, 15)
	ROUND3(ar, br, cr, dr, er, 0x5C4DD124,  1, 13)
	ROUND3(er, ar, br, cr, dr, 0x5C4DD124,  2, 11)
	ROUND2(dr, er, ar, br, cr, 0x6D703EF3, 15,  9)
	ROUND2(cr, dr, er, ar, br, 0x6D703EF3,  5,  7)
	ROUND2(br, cr, dr, er, ar, 0x6D703EF3,  1, 15)
	ROUND2(ar, br, cr, dr, er, 0x6D703EF3,  3, 11)
	ROUND2(er, ar, br, cr, dr, 0x6D703EF3,  7,  8)
	ROUND2(dr, er, ar, br, cr, 0x6D703EF3, 14,  6)
	ROUND2(cr, dr, er, ar, br, 0x6D703EF3,  6,  6)
	ROUND2(br, cr, dr, er, ar, 0x6D703EF3,  9, 14)
	ROUND2(ar, br, cr, dr, er, 0x6D703EF3, 11, 12)
	ROUND2(er, ar, br, cr, dr, 0x6D703EF3,  8, 13)
	ROUND2(dr, er, ar, br, cr, 0x6D703EF3, 12,  5)
	ROUND2(cr, dr, er, ar, br, 0x6D703EF3,  2, 14)
	ROUND2(br, cr, dr, er, ar, 0x6D703EF3, 10, 13)
	ROUND2(ar, br, cr, dr, er, 0x6D703EF3,  0, 13)
	ROUND2(er, ar, br, cr, dr, 0x6D703EF3,  4,  7)
	ROUND2(dr, er, ar, br, cr, 0x6D703EF3, 13,  5)
	ROUND1(cr, dr, er, ar, br, 0x7A6D76E9,  8, 15)
	ROUND1(br, cr, dr, er, ar, 0x7A6D76E9,  6,  5)
	ROUND1(ar, br, cr, dr, er, 0x7A6D76E9,  4,  8)
	ROUND1(er, ar, br, cr, dr, 0x7A6D76E9,  1, 11)
	ROUND1(dr, er, ar, br, cr, 0x7A6D76E9,  3, 14)
	ROUND1(cr, dr, er, ar, br, 0x7A6D76E9, 11, 14)
	ROUND1(br, cr, dr, er, ar, 0x7A6D76E9, 15,  6)
	ROUND1(ar, br, cr, dr, er, 0x7A6D76E9,  0, 14)
	ROUND1(er, ar, br, cr, dr, 0x7A6D76E9,  5,  6)
	ROUND1(dr, er, ar, br, cr, 0x7A6D76E9, 12,  9)
	ROUND1(cr, dr, er, ar, br, 0x7A6D76E9,  2, 12)
	ROUND1(br, cr, dr, er, ar, 0x7A6D76E9, 13,  9)
	ROUND1(ar, br, cr, dr, er, 0x7A6D76E9,  9, 12)
	ROUND1(er, ar, br, cr, dr, 0x7A6D76E9,  7,  5)
	ROUND1(dr, er, ar, br, cr, 0x7A6D76E9, 10, 15)
	ROUND1(cr, dr, er, ar, br, 0x7A6D76E9, 14,  8)
	ROUND0(br, cr, dr, er, ar, 0x00000000, 12,  8)
	ROUND0(ar, br, cr, dr, er, 0x00000000, 15,  5)
	ROUND0(er, ar, br, cr, dr, 0x00000000, 10, 12)
	ROUND0(dr, er, ar, br, cr, 0x00000000,  4,  9)
	ROUND0(cr, dr, er, ar, br, 0x00000000,  1, 12)
	ROUND0(br, cr, dr, er, ar, 0x00000000,  5,  5)
	ROUND0(ar, br, cr, dr, er, 0x00000000,  8, 14)
	ROUND0(er, ar, br, cr, dr, 0x00000000,  7,  6)
	ROUND0(dr, er, ar, br, cr, 0x00000000,  6,  8)
	ROUND0(cr, dr, er, ar, br, 0x00000000,  2, 13)
	ROUND0(br, cr, dr, er, ar, 0x00000000, 13,  6)
	ROUND0(ar, br, cr, dr, er, 0x00000000, 14,  5)
	ROUND0(er, ar, br, cr, dr, 0x00000000,  0, 15)
	ROUND0(dr, er, ar, br, cr, 0x00000000,  3, 13)
	ROUND0(cr, dr, er, ar, br, 0x00000000,  9, 11)
	ROUND0(br, cr, dr, er, ar, 0x00000000, 11, 11)
	
	uint32_t temp = state[1] + cl + dr;
	state[1] = state[2] + dl + er;
	state[2] = state[3] + el + ar;
	state[3] = state[4] + al + br;
	state[4] = state[0] + bl + cr;
	state[0] = temp;
}
