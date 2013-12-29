/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtomcrypt.com
 */
#include "tomcrypt.h"

/**
  @file crypt_hash_descriptor.c
  Stores the hash descriptor table, Tom St Denis  
*/

struct ltc_hash_descriptor hash_descriptor[TAB_SIZE] = {
{ NULL, 0, 0, 0, { 0 }, 0, NULL, NULL, NULL, NULL, NULL }
};

LTC_MUTEX_GLOBAL(ltc_hash_mutex)


/* $Source: /home/cvsroot/RT288x_SDK/source/user/dropbear-0.52/libtomcrypt/src/misc/crypt/crypt_hash_descriptor.c,v $ */
/* $Revision: 1.1 $ */
/* $Date: 2010-07-02 06:29:17 $ */
