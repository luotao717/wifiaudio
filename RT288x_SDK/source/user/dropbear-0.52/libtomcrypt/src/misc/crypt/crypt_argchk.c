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
#include <signal.h>

/**
  @file crypt_argchk.c
  Perform argument checking, Tom St Denis
*/  

#if (ARGTYPE == 0)
void crypt_argchk(char *v, char *s, int d)
{
 fprintf(stderr, "LTC_ARGCHK '%s' failure on line %d of file %s\n",
         v, d, s);
 (void)raise(SIGABRT);
}
#endif

/* $Source: /home/cvsroot/RT288x_SDK/source/user/dropbear-0.52/libtomcrypt/src/misc/crypt/crypt_argchk.c,v $ */
/* $Revision: 1.1 $ */
/* $Date: 2010-07-02 06:29:16 $ */
