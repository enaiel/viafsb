/*******************************************************************************

  ics94211.c: ICS94211 implementation to access PLL functions
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

#define BYTE_COUNT	21
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	8
#define FS_SEL_BIT	3
#define LFS0_BYTE	3
#define LFS1_BYTE	4
#define LFS2_BYTE	1
#define LFS3_BYTE	4
#define LFS4_BYTE	-1
#define LFS5_BYTE	-1
#define FS0_BIT		4
#define FS1_BIT		5
#define FS2_BIT		6
#define FS3_BIT		7
#define FS4_BIT		2
#define FS5_BIT		-1
#define LFS0_BIT	6
#define LFS1_BIT	3
#define LFS2_BIT	7
#define LFS3_BIT	1
#define LFS4_BIT	-1
#define LFS5_BIT	-1
#define LFS_INV		1
#define CAN_READ	1
#define CAN_TEST	1
#define FSB_TBL_SIZE 	32
#define FNAME	"ICS94211"

static const fsb_rec fsb_tbl[] =
{
	{ 66.82, 33.41, 0x03, 2},
	{ 68.01, 34.01, 0x06, 2},
	{ 75.00, 37.50, 0x01, 2},
	{ 80.00, 40.00, 0x00, 2},
	{ 83.31, 41.65, 0x02, 2},
	{ 85.01, 28.34, 0x17, 3},
	{ 90.00, 30.00, 0x16, 3},
	{ 95.00, 31.67, 0x15, 3},
	{ 100.23, 33.41, 0x07, 3},
	{ 103.00, 34.33, 0x04, 3},
	{ 105.00, 35.00, 0x0B, 3},
	{ 109.99, 36.66, 0x0A, 3},
	{ 112.01, 37.34, 0x05, 3},
	{ 114.99, 38.33, 0x09, 3},
	{ 115.98, 38.66, 0x14, 3},
	{ 118.00, 39.33, 0x13, 3},
	{ 120.00, 40.00, 0x08, 3},
	{ 124.00, 31.00, 0x0E, 4},
	{ 126.00, 31.50, 0x12, 4},
	{ 129.99, 32.50, 0x11, 4},
	{ 132.99, 33.25, 0x0F, 4},
	{ 135.00, 33.75, 0x10, 4},
	{ 138.01, 34.50, 0x1F, 4},
	{ 140.00, 35.00, 0x0C, 4},
	{ 141.99, 35.50, 0x1E, 4},
	{ 143.98, 35.99, 0x1D, 4},
	{ 145.98, 36.50, 0x1C, 4},
	{ 147.95, 36.99, 0x1B, 4},
	{ 150.00, 37.50, 0x0D, 4},
	{ 154.99, 38.75, 0x1A, 4},
	{ 160.01, 40.00, 0x19, 4},
	{ 166.00, 41.50, 0x18, 4}
};

static u8 pll_reg[] = 
{ 
	/* 0x02, 0x7F, 0xFF, 0xBF, 0xF7, 0xFF, 0x06, 0x20, 
  	0x15, 0x00, 0x10, 0x23, 0xFF, 0x04, 0x6C, 0x00, 
	0xFF, 0xEA, 0xAA, 0xFF, 0xFF */
	0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 0x3F, 
  	0x08, 0x00, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
	0x3F, 0x00, 0x00, 0xFF, 0xFF 
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
	CAN_READ,
};

int ics94211_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int ics94211_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int ics94211_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int ics94211_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool ics94211_can_test()
{
	return alg1_can_test(&pll);
}

bool ics94211_can_read()
{
	return alg1_can_read(&pll);
}

