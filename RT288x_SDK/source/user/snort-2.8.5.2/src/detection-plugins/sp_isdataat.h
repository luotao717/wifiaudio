/*
** Copyright (C) 2003-2009 Sourcefire, Inc.
** 
** Brian Caswell <bmc@snort.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* $Id: sp_isdataat.h,v 1.1 2010-02-02 09:42:57 yy Exp $ */

#ifndef __SP_ISDATAAT_H__
#define __SP_ISDATAAT_H__

#define ISDATAAT_RELATIVE_FLAG 0x01
#define ISDATAAT_RAWBYTES_FLAG 0x02
#define ISDATAAT_NOT_FLAG      0x04

typedef struct _IsDataAtData
{
    uint32_t offset;        /* byte location into the packet */
    uint8_t  flags;         /* relative to the doe_ptr? */
                             /* rawbytes buffer? */
} IsDataAtData;

void SetupIsDataAt(void);
uint32_t IsDataAtHash(void *d);
int IsDataAtCompare(void *l, void *r);

#endif  /* __SP_ISDATAAT_H__ */
