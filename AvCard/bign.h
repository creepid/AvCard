#ifndef __BIGN_H_
#define __BIGN_H_

#include "point.h"

typedef struct _bign_privkey {
	BigInt d;
} bign_privkey;

typedef struct _bign_pubkey {
	PointECAf Q;
} bign_pubkey;

EXTERN_C BOOL bign_valpubkey(byte *Q);

EXTERN_C void bign_sign(byte *H, byte *d, byte* to);

EXTERN_C BOOL bign_verify(byte *H, byte *Q, byte *S, uint32 size);

EXTERN_C void bign_keywrap(byte* X, byte *Q, byte *token);

EXTERN_C void bign_keyunwrap(byte *X, byte *d, byte *untoken);

EXTERN_C void bign_dh(byte *k, uint32 kSize, byte *P, uint32 pSize, byte* to);

EXTERN_C void bign_prng(uint32 n, byte* Sigma, byte *S, byte *to);

EXTERN_C void bign_hmac(byte *Sigma, byte *S, byte *to);


#endif