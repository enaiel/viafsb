/*******************************************************************************

  pll.h: PLL interface to access PLL functions
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

#ifndef __PLL_H_
#define __PLL_H_

#include "types.h"

#define PLL_MAKE_FUNCS(name) \
extern int name ## _set_fsb(float fsb, float pci, bool test); \
extern int name ## _get_fsb(float *fsb, float *pci, u8 *fsb_key, int *pci_div); \
extern int name ## _get_supp_fsb(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div); \
extern bool name ## _can_test(); \
extern bool name ## _can_read(); \
extern int name ## _get_supp_fsb_size(); 

#define PLL_MAKE_STRUCT(name, instance) {name, instance ## _set_fsb, instance ## _get_fsb, instance ## _get_supp_fsb, instance ## _can_test, instance ## _can_read, instance ## _get_supp_fsb_size}

typedef struct
{
	char *name;

	int (*set_fsb)(float fsb, float pci, bool test);
	int (*get_fsb)(float *fsb, float *pci, u8 *fsb_key, int *pci_div);
	int (*get_supp_fsb)(int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div);
	bool (*can_test)();
	bool (*can_read)();
	int (*get_supp_fsb_size)();
} pll_rec;

PLL_MAKE_FUNCS(cy28316)
PLL_MAKE_FUNCS(ics9148_37)
PLL_MAKE_FUNCS(ics9248_127)
PLL_MAKE_FUNCS(ics94211)
PLL_MAKE_FUNCS(ics94215)
PLL_MAKE_FUNCS(ics94241)
PLL_MAKE_FUNCS(ics950405)
PLL_MAKE_FUNCS(ics950908)
PLL_MAKE_FUNCS(pll205_03)
PLL_MAKE_FUNCS(w124)
PLL_MAKE_FUNCS(w156c)
PLL_MAKE_FUNCS(w230_03h)
PLL_MAKE_FUNCS(w83194br_39b)
PLL_MAKE_FUNCS(w83195r_08)

const pll_rec pll_tbl[] =
{
	PLL_MAKE_STRUCT("CY28316", cy28316),
	PLL_MAKE_STRUCT("ICS9148-37", ics9148_37),
	PLL_MAKE_STRUCT("ICS9248-127", ics9248_127),
	PLL_MAKE_STRUCT("ICS94211", ics94211),
	PLL_MAKE_STRUCT("ICS94215", ics94215),
	PLL_MAKE_STRUCT("ICS94241", ics94241),
	PLL_MAKE_STRUCT("ICS950405", ics950405),
	PLL_MAKE_STRUCT("ICS950908", ics950908),
	PLL_MAKE_STRUCT("PLL205-03", pll205_03),
	PLL_MAKE_STRUCT("W124", w124),
	PLL_MAKE_STRUCT("W156C", w156c),
	PLL_MAKE_STRUCT("W230-03H", w230_03h),
	PLL_MAKE_STRUCT("W83194BR-39B", w83194br_39b),
	PLL_MAKE_STRUCT("W83195R-08", w83195r_08)
};

#endif //__PLL_H_
