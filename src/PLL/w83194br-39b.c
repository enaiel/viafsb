/*******************************************************************************

  w83194br-39b.c: W83194BR-39B implementation to access PLL functions
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

#define BYTE_COUNT	13
#define FSB_BYTE	0
#define BYTE_COUNT_BYTE	-1
#define FS_SEL_BIT	1
#define LFS0_BYTE	4
#define LFS1_BYTE	4
#define LFS2_BYTE	4
#define LFS3_BYTE	4
#define LFS4_BYTE	-1	
#define LFS5_BYTE	-1	
#define FS0_BIT		2
#define FS1_BIT		3
#define FS2_BIT		4
#define FS3_BIT		5
#define FS4_BIT		6
#define FS5_BIT		7
#define LFS0_BIT	3
#define LFS1_BIT	4
#define LFS2_BIT	5
#define LFS3_BIT	6
#define LFS4_BIT	-1	
#define LFS5_BIT	-1	
#define LFS_INV		0	
#define CAN_READ	1	
#define CAN_TEST	1	
#define FSB_TBL_SIZE 	64
#define FNAME		"W83194BR-39B"

static const fsb_rec fsb_tbl[] =
{
	{ 66.82, 33.41, 0x03, 2},
	{ 68.01, 34.01, 0x06, 2},
	{ 75.00, 37.50, 0x01, 2},
	{ 80.00, 40.00, 0x00, 2},
	{ 83.30, 41.65, 0x02, 2},
	{ 100.23, 33.41, 0x07, 3},
	{ 103.00, 34.33, 0x04, 3},
	{ 105.00, 35.00, 0x0B, 3},
	{ 112.00, 37.34, 0x05, 3},
	{ 115.00, 38.33, 0x09, 3},
	{ 120.00, 30.00, 0x08, 4},
	{ 120.00, 40.00, 0x0A, 3},
	{ 124.00, 31.00, 0x0E, 4},
	{ 127.00, 31.75, 0x11, 4},
	{ 130.00, 32.50, 0x12, 4},
	{ 133.30, 33.30, 0x0F, 4},
	{ 135.00, 33.75, 0x13, 4},
	{ 136.00, 34.00, 0x14, 4},
	{ 137.00, 34.25, 0x15, 4},
	{ 139.00, 34.75, 0x16, 4},
	{ 140.00, 35.00, 0x0C, 4},
	{ 140.00, 35.00, 0x17, 4},
	{ 141.00, 35.25, 0x18, 4},
	{ 142.00, 35.50, 0x19, 4},
	{ 143.00, 35.75, 0x1A, 4},
	{ 144.00, 36.00, 0x1B, 4},
	{ 145.00, 36.25, 0x1C, 4},
	{ 146.00, 36.50, 0x1D, 4},
	{ 148.00, 37.00, 0x1E, 4},
	{ 149.00, 37.25, 0x1F, 4},
	{ 151.00, 37.75, 0x20, 4},
	{ 152.00, 38.00, 0x21, 4},
	{ 153.00, 38.25, 0x22, 4},
	{ 154.00, 38.50, 0x23, 4},
	{ 155.00, 38.75, 0x0D, 4},
	{ 155.00, 38.75, 0x24, 4},
	{ 156.00, 39.00, 0x25, 4},
	{ 157.00, 39.25, 0x26, 4},
	{ 158.00, 39.50, 0x27, 4},
	{ 159.00, 39.75, 0x28, 4},
	{ 160.00, 40.00, 0x10, 4},
	{ 162.00, 40.50, 0x29, 4},
	{ 163.00, 32.60, 0x2A, 5},
	{ 164.00, 32.80, 0x2B, 5},
	{ 165.00, 33.00, 0x2C, 5},
	{ 167.00, 33.40, 0x2D, 5},
	{ 168.00, 33.60, 0x2E, 5},
	{ 169.00, 33.80, 0x2F, 5},
	{ 170.00, 34.00, 0x30, 5},
	{ 172.00, 34.40, 0x31, 5},
	{ 174.00, 34.80, 0x32, 5},
	{ 176.00, 35.20, 0x33, 5},
	{ 178.00, 35.60, 0x34, 5},
	{ 180.00, 36.00, 0x35, 5},
	{ 182.00, 36.40, 0x36, 5},
	{ 184.00, 36.80, 0x37, 5},
	{ 186.00, 37.20, 0x38, 5},
	{ 188.00, 37.60, 0x39, 5},
	{ 190.00, 38.00, 0x3A, 5},
	{ 192.00, 38.40, 0x3B, 5},
	{ 194.00, 38.80, 0x3C, 5},
	{ 196.00, 39.20, 0x3D, 5},
	{ 198.00, 39.60, 0x3E, 5},
	{ 200.00, 40.00, 0x3F, 5}
};

static u8 pll_reg[] = 
{ 
	/* 0x00, 0xCF, 0xFF, 0xFF, 0x87, 0x93, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x62, 0x51 */
	0x00, 0xCF, 0xFF, 0xFF, 0xFF, 0x93, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x62, 0x51 
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

int w83194br_39b_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int w83194br_39b_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int w83194br_39b_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int w83194br_39b_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool w83194br_39b_can_test()
{
	return alg1_can_test(&pll);
}

bool w83194br_39b_can_read()
{
	return alg1_can_read(&pll);
}

