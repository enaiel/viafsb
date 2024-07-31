/*******************************************************************************

  pci.c: PCI implementation to read and write PCI configuration space registers
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

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dos.h>

#include "include/types.h"
#include "include/log.h"
#include "include/pci.h"

#define FNAME	"PCI"

u32 pci_get_addr(u16 bus, u16 dev, u16 fun, u16 reg)
{
	u32 addr;
	addr = (bus << 16)|(dev << 11)|(fun << 8)|(reg & 0xFC)|PCI_BASE_ADDR;
	return addr;
}

int pci_read_cfg_int(u16 bus, u16 dev, u16 fun, u16 reg, u32 *val)
{
	u32 addr;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = inportl(PCI_CONFIG_DATA + (reg & 0x03));
#ifdef DEBUG
	log_debug("%s: 0x%04X\t0x%04X\t0x%04X\t0x%04X\t0x%08X\t0x%08X\n",FNAME,bus,dev,fun,reg,addr,*val);
#endif
	return 1;
}

int pci_read_cfg_word(u16 bus, u16 dev, u16 fun, u16 reg, u16 *val)
{
	u32 addr;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = inportb(PCI_CONFIG_DATA + (reg & 0x03));
	reg += 1;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = *val + (inportb(PCI_CONFIG_DATA + (reg & 0x03)) << 8);
#ifdef DEBUG
	log_debug("%s: 0x%04X\t0x%04X\t0x%04X\t0x%04X\t0x%08X\t0x%04X\n",FNAME,bus,dev,fun,reg,addr,*val);
#endif
	return 1;
}

int pci_read_cfg_byte(u16 bus, u16 dev, u16 fun, u16 reg, u8 *val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	*val = inportb(PCI_CONFIG_DATA + (reg & 0x03));
#ifdef DEBUG
	log_debug("%s: 0x%04X\t0x%04X\t0x%04X\t0x%04X\t0x%08X\t0x%02X\n",FNAME,bus,dev,fun,reg,addr,*val);
#endif
	return 1;
}

int pci_write_cfg_byte(u16 bus, u16 dev, u16 fun, u16 reg, u8 val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportb(PCI_CONFIG_DATA + (reg & 0x03), val);
#ifdef DEBUG
	log_debug("%s: 0x%04X\t0x%04X\t0x%04X\t0x%04X\t0x%08X\t0x%02X\n",FNAME,bus,dev,fun,reg,addr,val);
#endif
	return 1;
}

int pci_write_cfg_word(u16 bus, u16 dev, u16 fun, u16 reg, u16 val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportb(PCI_CONFIG_DATA + (reg & 0x03), val);
	reg += 1;
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportb(PCI_CONFIG_DATA + (reg & 0x03), val >> 8);
#ifdef DEBUG
	log_debug("%s: 0x%04X\t0x%04X\t0x%04X\t0x%04X\t0x%08X\t0x%04X\n",FNAME,bus,dev,fun,reg,addr,val);
#endif
	return 1;
}

int pci_write_cfg_int(u16 bus, u16 dev, u16 fun, u16 reg, u32 val)
{
	u32 addr; 
	addr = pci_get_addr(bus, dev, fun, reg);
	outportl(PCI_CONFIG_ADDR, addr);
	outportl(PCI_CONFIG_DATA + (reg & 0x03), val);
#ifdef DEBUG
	log_debug("%s: 0x%04X\t0x%04X\t0x%04X\t0x%04X\t0x%08X\t0x%08X\n",FNAME,bus,dev,fun,reg,addr,val);
#endif
	return 1;
}

void pci_list()
{
	u16 bus,dev,fun;
	u32 addr, val;
	u16 vendor_id, device_id;
	log_debug("%s: Listing PCI Devices...\n",FNAME);
	log_debug("bus#\tdev#\tfun#\taddr\t\tval\t\tvendor\tdevice\n");
	for(bus = 0; bus < PCI_MAX_BUS; bus++)
		for(dev = 0; dev < PCI_MAX_DEV; dev++)
			for(fun = 0; fun < PCI_MAX_FUN; fun++)
			{	
				addr = pci_get_addr(bus, dev, fun, 0);
				pci_read_cfg_int(bus, dev, fun, 0, &val);
				if((val != 0xffffffff)&&(val != 0))
				{
					vendor_id = val & 0x0000ffff;
					device_id = (val & 0xffff0000) >> 16;
					log_debug("0x%02X\t0x%02X\t0x%02X\t0x%08X\t0x%08X\t0x%04X\t0x%04X\n",bus,dev,fun,addr,val,vendor_id,device_id); 
				}
			}
}

