/*******************************************************************************

  smb.h: SMBus interface to communicate with devices on SMBus
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

#ifndef __SMB_H_
#define __SMB_H_

#include "types.h"

/* SMB Registers */
#define SMB_HST_STS 0
#define SMB_HST_CNT 2
#define SMB_HST_CMD 3
#define SMB_HST_ADD 4
#define SMB_HST_DAT_0 5
#define SMB_HST_DAT_1 6
#define SMB_BLK_DAT 7

/* SMB Size Constants */
#define SMB_QUICK		0x00
#define SMB_BYTE		0x04
#define SMB_BYTE_DATA		0x08
#define SMB_WORD_DATA		0x0C
#define SMB_BLOCK_DATA		0x14

#define SMB_BLOCK_MAX		32

/* SMB Bit Masks */
#define SMB_READ	0x01
#define SMB_WRITE	0x00

/* SMB Error Codes */
#define ERRSMB		100
#define ERRSMB01	101
#define ERRSMB02	102
#define ERRSMB03	103
#define ERRSMB04	104
#define ERRSMB05	105

#define SMB_TIMEOUT 500

void smb_set_addr(u32 addr);

int smb_read_byte(u8 addr, u8 cmd);

int smb_write_byte(u8 addr, u8 cmd);

int smb_read_quick(u8 addr, u8 cmd);

int smb_write_quick(u8 addr, u8 cmd);

int smb_read_byte_data(u8 addr, u8 cmd, u8 *val);

int smb_write_byte_data(u8 addr, u8 cmd, u8 val);

int smb_read_word_data(u8 addr, u8 cmd, u16 *val);

int smb_write_word_data(u8 addr, u8 cmd, u16 val);

int smb_read_block_data(u8 addr, u8 cmd, int len, u8 val[]);

int smb_write_block_data(u8 addr, u8 cmd, int len,u8 val[]);

int smb_read_block_data_emu(u8 addr, u8 cmd, int len, u8 val[]);

int smb_write_block_data_emu(u8 addr, u8 cmd, int len,u8 val[]);

void smb_dump_regs();

void smb_list();

#endif // __SMB_H_
