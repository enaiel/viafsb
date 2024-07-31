/*******************************************************************************

  pll205-03.c: PLL205-03 implementation to access PLL functions
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

#define BYTE_COUNT	9
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	-1
#define FS_SEL_BIT	3
#define LFS0_BYTE	5
#define LFS1_BYTE	5
#define LFS2_BYTE	5
#define LFS3_BYTE	5
#define LFS4_BYTE	-1
#define LFS5_BYTE	-1
#define FS0_BIT		4
#define FS1_BIT		5
#define FS2_BIT		6
#define FS3_BIT		7
#define FS4_BIT		2
#define FS5_BIT		-1
#define LFS0_BIT	4
#define LFS1_BIT	5
#define LFS2_BIT	6
#define LFS3_BIT	7
#define LFS4_BIT	-1
#define LFS5_BIT	-1
#define LFS_INV		1
#define CAN_READ	1
#define CAN_TEST	1
#define FSB_TBL_SIZE 	32
#define FNAME	"PLL205-03"

static const fsb_rec fsb_tbl[] =
{
	{ 66.80, 33.40, 0x03, 2},
	{ 75.00, 37.50, 0x01, 2},
	{ 83.30, 41.70, 0x02, 2},
	{ 90.00, 30.00, 0x10, 3},
	{ 92.50, 30.80, 0x11, 3},
	{ 95.00, 31.70, 0x12, 3},
	{ 97.50, 32.50, 0x13, 3},
	{ 100.00, 33.30, 0x17, 3},
	{ 100.00, 33.30, 0x07, 3},
	{ 101.50, 33.80, 0x14, 3},
	{ 103.00, 34.30, 0x04, 3},
	{ 105.00, 35.00, 0x0B, 3},
	{ 107.50, 35.80, 0x1B, 3},
	{ 110.00, 36.70, 0x0A, 3},
	{ 112.00, 37.30, 0x05, 3},
	{ 115.00, 38.30, 0x09, 3},
	{ 117.50, 39.20, 0x19, 3},
	{ 120.00, 40.00, 0x18, 3},
	{ 120.00, 40.00, 0x08, 3},
	{ 122.00, 40.70, 0x1A, 3},
	{ 124.00, 31.00, 0x0E, 4},
	{ 124.00, 41.30, 0x00, 3},
	{ 127.00, 42.30, 0x15, 3},
	{ 130.00, 32.50, 0x1E, 4},
	{ 133.30, 33.30, 0x1F, 4},
	{ 133.30, 33.30, 0x0F, 4},
	{ 133.30, 44.40, 0x06, 3},
	{ 136.50, 34.10, 0x16, 4},
	{ 140.00, 35.00, 0x0C, 4},
	{ 145.00, 36.30, 0x1C, 4},
	{ 150.00, 37.50, 0x0D, 4},
	{ 155.00, 38.70, 0x1D, 4}
};

static u8 pll_reg[] = 
{ 
	/* 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  	0x00 */
	0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 
  	0x02
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

int pll205_03_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int pll205_03_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int pll205_03_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int pll205_03_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool pll205_03_can_test()
{
	return alg1_can_test(&pll);
}

bool pll205_03_can_read()
{
	return alg1_can_read(&pll);
}

