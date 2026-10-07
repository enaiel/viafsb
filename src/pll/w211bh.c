/*******************************************************************************

  w211bh.c: W211BH implementation to access PLL functions
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
#define FNAME	"W211BH"

static const fsb_rec fsb_tbl[] =
{
  { 66.80, 33.40, 0x1C, 2},  /* SS +-0.5% */
  { 66.80, 33.40, 0x14, 2},  /* SS -0.5% */
  { 75.00, 37.50, 0x1E, 2},
  { 79.00, 39.50, 0x1B, 2},
  { 83.00, 27.70, 0x16, 3},
  { 85.00, 28.30, 0x0F, 3},
  { 87.50, 29.20, 0x0E, 3},
  { 90.00, 30.00, 0x0D, 3},
  { 92.50, 30.80, 0x0C, 3},
  { 95.00, 31.70, 0x10, 3},  /* SS -0.5% */
  { 95.00, 31.70, 0x0B, 3},
  { 100.20, 33.30, 0x1D, 3}, /* SS +-0.5% */
  { 100.20, 33.30, 0x15, 3}, /* SS -0.5% */
  { 110.00, 36.70, 0x1A, 3},
  { 115.00, 38.30, 0x19, 3},
  { 120.00, 30.00, 0x18, 4},
  { 122.00, 30.50, 0x13, 4}, /* SS -0.5% */
  { 129.00, 32.30, 0x12, 4},
  { 133.30, 33.30, 0x1F, 4}, /* SS +-0.5% */
  { 133.30, 33.30, 0x17, 4}, /* SS -0.5% */
  { 138.00, 34.50, 0x11, 4},
  { 147.00, 36.80, 0x0A, 4},
  { 152.00, 30.40, 0x09, 5},
  { 154.00, 30.80, 0x08, 5},
  { 157.00, 31.40, 0x07, 5},
  { 159.00, 31.80, 0x06, 5},
  { 162.00, 32.40, 0x05, 5},
  { 166.00, 33.20, 0x04, 5},
  { 171.00, 34.20, 0x03, 5},
  { 180.00, 36.00, 0x02, 5},
  { 190.00, 38.00, 0x01, 5},
  { 200.00, 40.00, 0x00, 5}
} ;

static u8 pll_reg[] = 
{
	0x04, 0x0B, 0x5F, 0x37, 0x00, 0x13, 0x00, 0x00 
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

int w211bh_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int w211bh_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int w211bh_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int w211bh_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool w211bh_can_test()
{
	return alg1_can_test(&pll);
}

bool w211bh_can_read()
{
	return alg1_can_read(&pll);
}

bool w211bh_test()
{
	return alg1_test(&pll);
}

void w211bh_print_cfg()
{
	alg1_print_cfg(&pll);
}
