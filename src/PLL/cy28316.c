/*******************************************************************************

  cy28316.c: CY28316 implementation to access PLL functions
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

#define BYTE_COUNT	18
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	-1
#define FS_SEL_BIT	3
#define LFS0_BYTE	1
#define LFS1_BYTE	1
#define LFS2_BYTE	1
#define LFS3_BYTE	1
#define LFS4_BYTE	1
#define LFS5_BYTE	-1
#define FS0_BIT		4
#define FS1_BIT		5
#define FS2_BIT		6
#define FS3_BIT		1
#define FS4_BIT		2
#define FS5_BIT		-1
#define LFS0_BIT	3
#define LFS1_BIT	4
#define LFS2_BIT	5
#define LFS3_BIT	6
#define LFS4_BIT	7
#define LFS5_BIT	-1
#define LFS_INV		0
#define CAN_TEST	1
#define CAN_READ	1
#define FSB_TBL_SIZE 	32
#define FNAME	"CY28316"

static const fsb_rec fsb_tbl[] =
{
	{ 67.20, 33.60, 0x0C, 2},
	{ 67.00, 33.50, 0x10, 2},
	{ 66.60, 33.30, 0x1C, 2},
	{ 66.80, 33.40, 0x14, 2},
	{ 75.00, 37.50, 0x1E, 2},
	{ 78.00, 39.00, 0x1B, 2},
	{ 85.00, 28.30, 0x1A, 3},
	{ 90.00, 30.00, 0x19, 3},
	{ 100.20, 33.40, 0x15, 3},
	{ 100.50, 33.50, 0x11, 3},
	{ 100.80, 33.60, 0x0D, 3},
	{ 100.00, 33.30, 0x1D, 3},
	{ 105.00, 35.00, 0x18, 3},
	{ 110.00, 36.70, 0x16, 3},
	{ 115.00, 38.30, 0x12, 3},
	{ 118.00, 39.30, 0x0E, 3},
	{ 124.00, 31.00, 0x0B, 4},
	{ 130.00, 32.50, 0x0A, 4},
	{ 133.60, 33.40, 0x17, 4},
	{ 134.00, 33.50, 0x13, 4},
	{ 134.40, 33.60, 0x0F, 4},
	{ 133.30, 33.30, 0x1F, 4},
	{ 136.00, 34.00, 0x09, 4},
	{ 140.00, 35.00, 0x08, 4},
	{ 145.00, 36.30, 0x07, 4},
	{ 150.00, 37.50, 0x06, 4},
	{ 160.00, 32.00, 0x05, 5},
	{ 166.00, 33.20, 0x04, 5},
	{ 170.00, 34.00, 0x03, 5},
	{ 180.00, 36.00, 0x02, 5},
	{ 190.00, 38.00, 0x01, 5},
	{ 200.00, 33.30, 0x00, 6}
};

static u8 pll_reg[] = 
{
	/* 0x81, 0xFE, 0xCB, 0xB3, 0x00, 0x03, 0x1C, 0x60, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0xBD */
	0x00, 0xFE, 0xFF, 0xBF, 0x00, 0x03, 0x3E, 0x60, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00
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

int cy28316_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int cy28316_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int cy28316_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int cy28316_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool cy28316_can_test()
{
	return alg1_can_test(&pll);
}

bool cy28316_can_read()
{
	return alg1_can_read(&pll);
}


