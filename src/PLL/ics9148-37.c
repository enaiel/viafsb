/*******************************************************************************

  ics9148-37.c: ICS9148-37 implementation to access PLL functions
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

#include <stdio.h>

#include "../include/types.h"
#include "../include/alg1.h"

#define BYTE_COUNT	6
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	-1
#define FS_SEL_BIT	3
#define LFS0_BYTE	0
#define LFS1_BYTE	0
#define LFS2_BYTE	0
#define LFS3_BYTE	-1
#define LFS4_BYTE	-1	
#define LFS5_BYTE	-1	
#define FS0_BIT		4
#define FS1_BIT		5
#define FS2_BIT		6
#define FS3_BIT		-1
#define FS4_BIT		-1
#define FS5_BIT		-1
#define LFS0_BIT	4
#define LFS1_BIT	5
#define LFS2_BIT	6
#define LFS3_BIT	-1
#define LFS4_BIT	-1	
#define LFS5_BIT	-1	
#define LFS_INV		0	
#define CAN_READ	1	
#define CAN_TEST	1	
#define FSB_TBL_SIZE 	8
#define FNAME		"ICS9148-37"

static const fsb_rec fsb_tbl[] =
{
	{ 60.00, 30.00, 0x00, 2},
	{ 66.80, 33.40, 0x01, 2},
	{ 68.50, 34.25, 0x02, 2},
	{ 75.00, 30.00, 0x04, 3},
	{ 75.00, 37.50, 0x03, 2},
	{ 83.30, 33.30, 0x05, 3},
	{ 95.25, 31.75, 0x06, 3},
	{ 100.00, 33.30, 0x07, 3}
};

static u8 pll_reg[] = 
{
	/* 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  */
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF 
}; 

static const pll_data pll =
{
	FNAME,
	&fsb_tbl[0],
	&pll_reg[0],
	FSB_TBL_SIZE,
	BYTE_COUNT,
	FSB_BYTE,
	BYTE_COUNT_BYTE,
	FS_SEL_BIT,
	LFS0_BYTE,
	LFS1_BYTE,
	LFS2_BYTE,
	LFS3_BYTE,
	LFS4_BYTE,
	LFS5_BYTE,
	FS0_BIT,
	FS1_BIT,
	FS2_BIT,
	FS3_BIT,
	FS4_BIT,
	FS5_BIT,
	LFS0_BIT,
	LFS1_BIT,
	LFS2_BIT,
	LFS3_BIT,
	LFS4_BIT,
	LFS5_BIT,
	LFS_INV,
	CAN_TEST,
	CAN_READ
};

int ics9148_37_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int ics9148_37_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int ics9148_37_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int ics9148_37_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool ics9148_37_can_test()
{
	return alg1_can_test(&pll);
}

bool ics9148_37_can_read()
{
	return alg1_can_read(&pll);
}

