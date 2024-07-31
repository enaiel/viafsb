/*******************************************************************************

  ics950405.c: ICS950405 implementation to access PLL functions
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

#define BYTE_COUNT	15
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	6
#define FS_SEL_BIT	-1
#define LFS0_BYTE	0
#define LFS1_BYTE	0
#define LFS2_BYTE	0
#define LFS3_BYTE	0
#define LFS4_BYTE	-1	
#define LFS5_BYTE	-1	
#define FS0_BIT		0
#define FS1_BIT		1
#define FS2_BIT		2
#define FS3_BIT		3
#define FS4_BIT		-1
#define FS5_BIT		-1
#define LFS0_BIT	0
#define LFS1_BIT	1
#define LFS2_BIT	2
#define LFS3_BIT	3
#define LFS4_BIT	-1	
#define LFS5_BIT	-1	
#define LFS_INV		0	
#define CAN_READ	1	
#define CAN_TEST	1	
#define FSB_TBL_SIZE 	16
#define FNAME	"ICS950405"

static const fsb_rec fsb_tbl[] =
{
	{ 100.20, 33.40, 0x04, 3},
	{ 100.90, 33.63, 0x00, 3},
	{ 133.50, 33.38, 0x05, 4},
	{ 133.90, 33.48, 0x01, 4},
	{ 150.00, 30.00, 0x08, 5},
	{ 166.70, 33.34, 0x06, 5},
	{ 168.00, 33.60, 0x02, 5},
	{ 180.00, 30.00, 0x09, 6},
	{ 200.40, 33.40, 0x07, 6},
	{ 202.00, 33.67, 0x03, 6},
	{ 210.00, 35.00, 0x0A, 6},
	{ 233.33, 33.33, 0x0D, 7},
	{ 240.00, 30.00, 0x0B, 8},
	{ 266.67, 33.33, 0x0E, 8},
	{ 270.00, 33.75, 0x0C, 8},
	{ 300.00, 37.50, 0x0F, 8}
};

static u8 pll_reg[] = 
{
	0xB0, 0xFF, 0xFF, 0xF5, 0x7F, 0xFF, 0x06, 0x01, 0xCC, 0x77, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF 
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

int ics950405_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int ics950405_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int ics950405_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int ics950405_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool ics950405_can_test()
{
	return alg1_can_test(&pll);
}

bool ics950405_can_read()
{
	return alg1_can_read(&pll);
}

