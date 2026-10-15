/*******************************************************************************

  w196.c: PLL205-03 implementation to access PLL functions
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

#define BYTE_COUNT	7
#define FSB_BYTE	3
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
#define FS3_BIT		7
#define FS4_BIT		1
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
#define FNAME	"W196"
#define SS_BYTE		-1
#define SS_EN_BIT	-1
#define SS0_BIT		-1
#define SS1_BIT		-1
#define SS_TBL_SIZE	0

static const fsb_rec fsb_tbl[] =
{
	{ 78.00,	39.00,	0x00,	2,	"OFF"		},
	{ 81.00,	40.50,	0x01,	2,	"OFF"		},
	{ 113.50,	37.80,	0x02,	3,	"OFF"		},
	{ 66.80,	33.40,	0x03,	2,	"OFF"		},
	{ 117.00,	39.00,	0x04,	3,	"OFF"		},
	{ 118.50,	39.50,	0x05,	3,	"OFF"		},
	{ 122.00,	37.30,	0x06,	3,	"OFF"		},
	{ 100.00,	33.30,	0x07,	3,	"OFF"		},
	{ 126.00,	31.50,	0x08,	4,	"OFF"		},
	{ 135.00,	33.75,	0x09,	4,	"OFF"		},
	{ 137.00,	34.25,	0x0A,	4,	"OFF"		},
	{ 138.50,	34.62,	0x0B,	4,	"OFF"		},
	{ 142.00,	35.50,	0x0C,	4,	"OFF"		},
	{ 144.00,	36.00,	0x0D,	4,	"OFF"		},
	{ 155.00,	38.75,	0x0E,	4,	"OFF"		},
	{ 133.30,	33.30,	0x0F,	4,	"OFF"		},
	{ 124.00,	41.30,	0x10,	3,	"+-0.50%"	},
	{ 75.00,	37.50,	0x11,	2,	"+-0.50%"	},
	{ 83.30,	41.65,	0x12,	2,	"+-0.50%"	},
	{ 66.80,	33.40,	0x13,	2,	"+-0.50%"	},
	{ 90.00,	30.00,	0x14,	3,	"+-0.50%"	},
	{ 112.00,	37.30,	0x15,	3,	"+-0.50%"	},
	{ 95.00,	31.67,	0x16,	3,	"+-0.50%"	},
	{ 100.00,	33.30,	0x17,	3,	"+-0.50%"	},
	{ 120.00,	40.00,	0x18,	3,	"+-0.50%"	},
	{ 115.00,	38.30,	0x19,	3,	"+-0.50%"	},
	{ 110.00,	36.67,	0x1A,	3,	"+-0.50%"	},
	{ 105.00,	35.00,	0x1B,	3,	"+-0.50%"	},
	{ 140.00,	35.00,	0x1C,	4,	"+-0.50%"	},
	{ 150.00,	37.50,	0x1D,	4,	"+-0.50%"	},
	{ 124.00,	31.00,	0x1E,	4,	"+-0.50%"	},
	{ 133.30,	33.30,	0x1F,	4,	"+-0.50%"	}
};

static const ss_rec ss_tbl[] =
{
};

static u8 pll_reg[] = 
{ 
	0xFF, 0xFF, 0xFF, 0x00, 0x45, 0xEF, 0x23
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
	ss_tbl,
	SS_TBL_SIZE,
	SS_BYTE,
	SS_EN_BIT,
	SS0_BIT,
	SS1_BIT
};

int w196_set_fsb(float fsb, float pci, bool test)
{
	return alg1_set_fsb(&pll, fsb, pci, test);
}

int w196_get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_fsb(&pll, fsb, pci, fsb_key, pci_div);
}

int w196_get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	return alg1_get_supp_fsb(&pll, idx, fsb, pci, fsb_key, pci_div);
}

int w196_get_supp_fsb_size()
{
	return alg1_get_supp_fsb_size(&pll);
}

bool w196_can_test()
{
	return alg1_can_test(&pll);
}

bool w196_can_read()
{
	return alg1_can_read(&pll);
}

bool w196_test()
{
	return alg1_test(&pll);
}

void w196_print_cfg()
{
	alg1_print_cfg(&pll);
}

void w196_init()
{
	alg1_init(&pll);
}
