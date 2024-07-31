/*******************************************************************************

  types.h: Common type definitions
  VIAFSB - DOS FSB Utility For VIA Chipsets

  Author: Enaiel <enaiel@gmail.com> (c) 2022

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/

#ifndef	__TYPES_H_
#define __TYPES_H_

#include<stdio.h>

/* Define Types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef int bool;

#define FALSE 0
#define TRUE 1

__attribute__((weak)) bool get_bit(u8 byte, u8 bit)
{
	return (byte >> bit) & (u8)1;
}

__attribute__((weak)) u8 set_bit(u8 byte, u8 bit, bool val)
{
	return (byte & ~((u8)1 << bit)) | ((u8)val << bit);
}

#endif	// __TYPES_H_
