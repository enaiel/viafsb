/*******************************************************************************

  pci.h: PCI interface to read and write PCI configuration space registers
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

#ifndef	__PCI_H_
#define	__PCI_H_

#include "types.h"

/* PCI Constants */
#define PCI_MAX_BUS	256
#define PCI_MAX_DEV	32
#define PCI_MAX_FUN	8
#define PCI_BASE_ADDR	0x80000000L
#define PCI_CONFIG_ADDR 0xcf8
#define PCI_CONFIG_DATA 0xcfc

u32 pci_get_addr(u16 bus, u16 dev, u16 fun, u16 reg);

int pci_read_cfg_int(u16 bus, u16 dev, u16 fun, u16 reg, u32 *val);

int pci_read_cfg_byte(u16 bus, u16 dev, u16 fun, u16 reg, u8 *val);

int pci_read_cfg_word(u16 bus, u16 dev, u16 fun, u16 reg, u16 *val);

int pci_write_cfg_int(u16 bus, u16 dev, u16 fun, u16 reg, u32 val);

int pci_write_cfg_byte(u16 bus, u16 dev, u16 fun, u16 reg, u8 val);

int pci_write_cfg_word(u16 bus, u16 dev, u16 fun, u16 reg, u16 val);

void pci_list();


#endif	//__PCI_H_
