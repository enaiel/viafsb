/*******************************************************************************

  w230-03h.c: W230-03H implementation to access PLL functions
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

#define BYTE_COUNT	8
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	-1
#define FS_SEL_BIT	3
#define LFS0_BYTE	-1
#define LFS1_BYTE	-1
#define LFS2_BYTE	-1
#define LFS3_BYTE	-1
#define LFS4_BYTE	-1	
#define LFS5_BYTE	-1	
#define FS0_BIT		4
#define FS1_BIT		5
#define FS2_BIT		6
#define FS3_BIT		1
#define FS4_BIT		2
#define FS5_BIT		-1
#define LFS0_BIT	-1
#define LFS1_BIT	-1
#define LFS2_BIT	-1
#define LFS3_BIT	-1
#define LFS4_BIT	-1	
#define LFS5_BIT	-1	
#define LFS_INV		0	
#define CAN_READ	0	
#define CAN_TEST	0	
#define FSB_TBL_SIZE 	32
#define FNAME	"W230-03H"

static const fsb_rec fsb_tbl[] =
{
	{ 95.00, 31.70, 0x1C, 3},
	{ 100.00, 33.30, 0x1F, 3},
	{ 100.00, 33.30, 0x1E, 3},
	{ 100.00, 33.30, 0x1D, 3},
	{ 102.00, 34.00, 0x18, 4},
	{ 104.00, 34.60, 0x17, 4},
	{ 106.00, 35.30, 0x16, 4},
	{ 107.00, 35.60, 0x15, 3},
	{ 108.00, 36.00, 0x14, 3},
	{ 109.00, 36.30, 0x13, 3},
	{ 110.00, 36.60, 0x12, 3},
	{ 111.00, 37.00, 0x11, 3},
	{ 112.00, 37.30, 0x10, 3},
	{ 113.00, 37.60, 0x0F, 3},
	{ 114.00, 38.00, 0x0E, 3},
	{ 115.00, 38.30, 0x0D, 3},
	{ 116.00, 38.60, 0x0C, 3},
	{ 118.00, 39.30, 0x0B, 3},
	{ 120.00, 40.00, 0x0A, 3},
	{ 124.00, 31.00, 0x09, 3},
	{ 127.00, 31.70, 0x08, 3},
	{ 130.00, 32.50, 0x07, 3},
	{ 133.30, 33.30, 0x1B, 4},
	{ 133.30, 33.30, 0x1A, 4},
	{ 133.30, 33.30, 0x19, 4},
	{ 136.00, 34.00, 0x06, 4},
	{ 140.00, 35.00, 0x05, 4},
	{ 145.00, 36.20, 0x04, 4},
	{ 150.00, 37.50, 0x03, 4},
	{ 155.00, 38.70, 0x02, 4},
	{ 160.00, 40.00, 0x01, 4},
	{ 166.00, 41.60, 0x00, 4}
} ;

static u8 pll_reg[] = 
{
	0x04, 0x0F, 0x5F, 0x37, 0x00, 0x13, 0x00, 0x00 
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

int w230_03h_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int w230_03h_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int w230_03h_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int w230_03h_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool w230_03h_can_test()
{
	return alg1_can_test(&pll);
}

bool w230_03h_can_read()
{
	return alg1_can_read(&pll);
}

