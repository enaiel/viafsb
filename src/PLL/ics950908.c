/*******************************************************************************

  ics950908.c: ICS950908 implementation to access PLL functions
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

#define BYTE_COUNT	24
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	8
#define FS_SEL_BIT	3
#define LFS0_BYTE	4
#define LFS1_BYTE	4
#define LFS2_BYTE	4
#define LFS3_BYTE	4
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
#define LFS_INV		0	
#define CAN_READ	1	
#define CAN_TEST	1	
#define FSB_TBL_SIZE 	32
#define FNAME	"ICS950908"

static const fsb_rec fsb_tbl[] =
{
	{ 66.80, 33.40, 0x18, 2},
	{ 66.60, 32.30, 0x1C, 2},
	{ 100.90, 33.63, 0x19, 3},
	{ 100.00, 33.30, 0x1D, 3},
	{ 102.00, 34.00, 0x00, 3},
	{ 105.00, 35.00, 0x01, 3},
	{ 108.00, 36.00, 0x02, 3},
	{ 111.00, 27.00, 0x03, 4},
	{ 114.00, 38.00, 0x04, 3},
	{ 117.00, 39.00, 0x05, 3},
	{ 120.00, 40.00, 0x06, 3},
	{ 123.00, 41.00, 0x07, 3},
	{ 126.00, 36.00, 0x08, 4},
	{ 130.00, 37.10, 0x09, 4},
	{ 133.90, 33.48, 0x0A, 4},
	{ 133.30, 33.30, 0x1F, 4},
	{ 133.60, 33.40, 0x1A, 4},
	{ 140.00, 35.00, 0x0B, 4},
	{ 144.00, 36.00, 0x0C, 4},
	{ 148.00, 37.00, 0x0D, 4},
	{ 152.00, 38.00, 0x0E, 4},
	{ 156.00, 39.00, 0x0F, 4},
	{ 160.00, 40.00, 0x10, 4},
	{ 164.00, 41.00, 0x11, 4},
	{ 166.60, 33.30, 0x12, 5},
	{ 170.00, 34.00, 0x13, 5},
	{ 175.00, 35.00, 0x14, 5},
	{ 180.00, 36.00, 0x15, 5},
	{ 185.00, 37.00, 0x16, 5},
	{ 190.00, 38.00, 0x17, 5},
	{ 200.40, 33.40, 0x1B, 6},
	{ 200.00, 33.30, 0x1E, 6}
};

static u8 pll_reg[] = 
{ 
	/* 0x0A, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0x01, 0x17, 
	0x18, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 
	0x50, 0x09, 0xA8, 0x88, 0x88, 0x55, 0x55, 0x55 */
	0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x17, 
	0x0F, 0x10, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x55, 
	0x50, 0x09, 0xAB, 0x88, 0x88, 0x55, 0x55, 0x55 
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

int ics950908_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int ics950908_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int ics950908_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int ics950908_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool ics950908_can_test()
{
	return alg1_can_test(&pll);
}

bool ics950908_can_read()
{
	return alg1_can_read(&pll);
}

