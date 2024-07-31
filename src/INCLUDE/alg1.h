/*******************************************************************************

  alg1.h: Generic interface to access PLL functions
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

#ifndef __ALG1_H_
#define __ALG1_H_

#include "types.h"

#define PLL_ADDR 	0x69
#define CMD		0x00

typedef struct
{
	float fsb;
	float pci;
	u8 fsb_key;
	int pci_div;
} fsb_rec;

typedef struct
{
	char *name;			// FNAME
	const fsb_rec *fsb_tbl; 	// fsb_tbl
	u8 *pll_reg;			// pll_reg
	int fsb_tbl_size;		// FSB_TBL_SIZE
	int byte_count;			// BYTE_COUNT
	int fsb_byte;			// FSB_BYTE
	int byte_count_byte;		// BYTE_COUNT_BYTE
	int fs_sel_bit;			// FS_SEL_BIT
	int lfs0_byte;			// LFS0_BYTE
	int lfs1_byte;			// LFS1_BYTE
	int lfs2_byte;			// LFS2_BYTE
	int lfs3_byte;			// LFS3_BYTE
	int lfs4_byte;			// LFS4_BYTE
	int lfs5_byte;			// LFS5_BYTE
	int fs0_bit;			// FS0_BIT
	int fs1_bit;			// FS1_BIT
	int fs2_bit;			// FS2_BIT
	int fs3_bit;			// FS3_BIT
	int fs4_bit;			// FS4_BIT
	int fs5_bit;			// FS5_BIT
	int lfs0_bit;			// LFS0_BIT
	int lfs1_bit;			// LFS1_BIT
	int lfs2_bit;			// LFS2_BIT
	int lfs3_bit;			// LFS3_BIT
	int lfs4_bit;			// LFS4_BIT
	int lfs5_bit;			// LFS5_BIT
	bool lfs_inv;			// LFS_INV
	bool can_test;			// CAN_TEST
	bool can_read;			// CAN_READ
} pll_data;

int alg1_set_fsb(const pll_data *pll, float fsb, float pci, bool test);

int alg1_get_fsb(const pll_data *pll, float *fsb, float *pci, u8 *fsb_key, int *pci_div);

int alg1_get_supp_fsb(const pll_data *pll, int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div);

int alg1_get_supp_fsb_size(const pll_data *pll);

bool alg1_can_test(const pll_data *pll);

bool alg1_can_read(const pll_data *pll);

#endif //__ALG1_H_
