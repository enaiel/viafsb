/*******************************************************************************

  smb.c: SMBus implementation to communicate with devices on SMBus
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
#include <stdlib.h>
#include <unistd.h>
#include <dos.h>
#include <inlines/pc.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "include/types.h"
#include "include/log.h"
#include "include/smb.h"

#define FNAME	"SMB"

u32 smb_addr;

void smb_set_addr(u32 addr)
{
#ifdef DEBUG
	log_debug("%s: smb_set_addr(0x%04X)\n",FNAME,addr);
#endif
	smb_addr = addr;
}

int smb_txn(u8 size)
{
	int temp;
	int result = 0;
	int timeout = 0;
	smb_dump_regs("txn pre");

	/* Make sure the SMBus host is ready to start transmitting */
	if ((temp = inportb(smb_addr + SMB_HST_STS)) & 0x1F) {
#ifdef DEBUG
		log_debug("%s: SMBus busy (0x%02X). Resetting...\n", FNAME, temp); 
#endif
		outportb(smb_addr + SMB_HST_STS, temp);
		if ((temp = inportb(smb_addr + SMB_HST_STS)) & 0x1F) {
#ifdef DEBUG
			log_debug("%s: SMBus reset failed! (0x%02X)\n", FNAME, temp);
#endif
			return -ERRSMB01;
		}
	}

	/* Start the transaction by setting bit 6 */
	outportb(smb_addr + SMB_HST_CNT, 0x40 | size); 

	/* Sleep for 100 ms otherwise SMBus will be busy */
	do {
		delay(100);
		temp = inportb(smb_addr + SMB_HST_STS);
	} while ((temp & 0x01) && (++timeout < SMB_TIMEOUT));

	/* If the SMBus is still busy, we give up */
	if (timeout == SMB_TIMEOUT) {
		result = -ERRSMB02;
#ifdef DEBUG
		log_debug("%s: SMBus timeout!\n", FNAME);
#endif
	}

	if (temp & 0x10) {
		result = -ERRSMB03;
#ifdef DEBUG
		log_debug("%s: Transaction failed (0x%02X)\n",FNAME,
			size);
#endif
	}

	if (temp & 0x08) {
		result = -ERRSMB04;
#ifdef DEBUG
		log_debug("%s: SMBus collision!\n", FNAME);
#endif
	}

	if (temp & 0x04) {
		result = -ERRSMB05;
#ifdef DEBUG
		log_debug("%s: No response\n", FNAME);
#endif
	}

	/* Resetting status register */
	if (temp & 0x1F)
		outportb(smb_addr + SMB_HST_STS, temp);

	smb_dump_regs("txn post");
	return result;
}

int smb_read_byte(u8 addr, u8 cmd)
{
#ifdef DEBUG
	log_debug("%s: smb_read_byte(0x%04X,0x%02X,0x%02X)\n",FNAME, smb_addr,addr,cmd);
#endif
	int status;
	u8 size = SMB_BYTE;
	u8 read_write = SMB_READ;

	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	return 1;
}

int smb_write_byte(u8 addr, u8 cmd)
{
#ifdef DEBUG
	log_debug("%s: smb_write_byte(0x%04X,0x%02X,0x%02X)\n",FNAME,smb_addr,addr,cmd);
#endif
	int status;
	u8 size = SMB_BYTE;
	u8 read_write = SMB_WRITE;
	outportb(smb_addr + SMB_HST_CMD, cmd);

	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	return 1;
}

int smb_read_byte_data(u8 addr, u8 cmd, u8 *val)
{
#ifdef DEBUG
	log_debug("%s: smb_read_byte_data(0x%04X,0x%02X,0x%02X)\n",FNAME,smb_addr,addr,cmd);
#endif
	int status;
	u8 size = SMB_BYTE_DATA;
	u8 read_write = SMB_READ;

	outportb(smb_addr + SMB_HST_CMD, cmd);
	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	*val = inportb(smb_addr + SMB_HST_DAT_0);
#ifdef DEBUG
	log_debug("%s: smb_read_byte_data(0x%04X,0x%02X,0x%02X,%02X)\n",FNAME,smb_addr,addr,cmd,*val);
#endif
	return 1;
}

int smb_write_byte_data(u8 addr, u8 cmd, u8 val)
{
#ifdef DEBUG
	log_debug("%s: smb_write_byte_data(0x%04X,0x%02X,0x%02X,0x%02X)\n",FNAME,smb_addr,addr,cmd,val);
#endif
	int status;
	u8 size = SMB_BYTE_DATA;
	u8 read_write = SMB_WRITE;

	outportb(smb_addr + SMB_HST_CMD, cmd);
	outportb(smb_addr + SMB_HST_DAT_0, val);
	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	return 1;
}

int smb_read_word_data(u8 addr, u8 cmd, u16 *val)
{
#ifdef DEBUG
	log_debug("%s: smb_read_word_data(0x%04X,0x%02X,0x%02X)\n",FNAME,smb_addr,addr,cmd);
#endif
	int status;
	u8 size = SMB_WORD_DATA;
	u8 read_write = SMB_READ;

	outportb(smb_addr + SMB_HST_CMD, cmd);
	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	*val = inportb(smb_addr + SMB_HST_DAT_0) + 
		(inportb(smb_addr + SMB_HST_DAT_1) << 8);

#ifdef DEBUG
	log_debug("%s: smb_read_word_data(0x%04X,0x%02X,0x%02X,%04X)\n",FNAME,smb_addr,addr,cmd,*val);
#endif
	return 1;
}

int smb_write_word_data(u8 addr, u8 cmd, u16 val)
{
#ifdef DEBUG
	log_debug("%s: smb_write_word_data(0x%04X,0x%02X,0x%02X,0x%04X)\n",FNAME,smb_addr,addr,cmd,val);
#endif
	int status;
	u8 size = SMB_WORD_DATA;
	u8 read_write = SMB_WRITE;

	outportb(smb_addr + SMB_HST_CMD, cmd);
	outportb(smb_addr + SMB_HST_DAT_0, val & 0xFF);
	outportb(smb_addr + SMB_HST_DAT_1, (val & 0xFF00) >> 8);
	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	return 1;
}

int smb_read_block_data(u8 addr, u8 cmd, int len, u8 val[])
{
#ifdef DEBUG
	log_debug("%s: smb_read_block_data(0x%04X,0x%02X,0x%02X,%2i)\n",FNAME,smb_addr,addr,cmd,len);
#endif
	int status;
	int i;
	u8 size = SMB_BLOCK_DATA;
	u8 read_write = SMB_READ;
	outportb(smb_addr + SMB_HST_CMD, cmd);

	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	len = inportb(smb_addr + SMB_HST_DAT_0);
#ifdef DEBUG
	log_debug("%s: Read block size: %d\n",FNAME, len);
#endif
	if(len > SMB_BLOCK_MAX)
		len = SMB_BLOCK_MAX;
	inportb(smb_addr + SMB_HST_CNT); /* Reset SMB_BLK_DAT */
	for(i=0; i<len; i++)
		val[i] = inportb(smb_addr + SMB_BLK_DAT);
#ifdef DEBUG
	log_debug("%s: smb_read_block_data(0x%04X,0x%02X,0x%02X,%2i)\n",FNAME,smb_addr,addr,cmd,len);
#endif
	return len;
}

int smb_write_quick(u8 addr, u8 cmd)
{
#ifdef DEBUG
	log_debug("%s: smb_write_quick(0x%04X,0x%02X,0x%02X)\n",FNAME,smb_addr,addr,cmd);
#endif
	int status;
	u8 size = SMB_QUICK;
	u8 read_write = SMB_WRITE;

	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	return 1;
}

int smb_read_quick(u8 addr, u8 cmd)
{
#ifdef DEBUG
	log_debug("%s: smb_read_quick(0x%04X,0x%02X,0x%02X)\n",FNAME,smb_addr,addr,cmd);
#endif
	int status;
	u8 size = SMB_QUICK;
	u8 read_write = SMB_READ;

	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

	return 1;
}

int smb_write_block_data(u8 addr, u8 cmd, int len, u8 val[])
{
#ifdef DEBUG
	log_debug("%s: smb_write_block_data(0x%04X,0x%02X,0x%02X,%2i)\n",FNAME,smb_addr,addr,cmd,len);
#endif
	int status;
	int i;
	u8 size = SMB_BLOCK_DATA;
	u8 read_write = SMB_WRITE;
	outportb(smb_addr + SMB_HST_CMD, cmd);

	if(len > SMB_BLOCK_MAX)
		len = SMB_BLOCK_MAX;

	outportb(smb_addr + SMB_HST_DAT_0, len);
#ifdef DEBUG
	log_debug("%s: Writing block size: %d\n", FNAME,len);
#endif
	inportb(smb_addr + SMB_HST_CNT); /* Reset SMB_BLK_DAT */
	for(i=0; i<len; i++)
		outportb(smb_addr + SMB_BLK_DAT, val[i]);

	outportb(smb_addr + SMB_HST_ADD, ((addr & 0x7f) << 1) | read_write); 

	status = smb_txn(size);
	if (status < 0)
		return status;

#ifdef DEBUG
	log_debug("%s: smb_write_block_data(0x%04X,0x%02X,0x%02X,%2d)\n",FNAME,smb_addr,addr,cmd,len);
#endif
	return len;
}

void smb_dump_regs(const char *msg)
{
#ifdef DEBUG
	log_debug("%s: %s: STS=0x%02X CNT=0x%02X ADD=0x%02X DAT=0x%02X\n",
		FNAME,
		msg,
		inportb(smb_addr + SMB_HST_STS),	
		inportb(smb_addr + SMB_HST_CNT),	
		inportb(smb_addr + SMB_HST_ADD),	
		inportb(smb_addr + SMB_HST_DAT_0));	
#endif
}

const char* smb_get_err_desc(int errno)
{
	switch(errno)
	{
		case ERRSMB01:
			return "SMBus Reset Failed";
		case ERRSMB02:
			return "SMBus Timeout";
		case ERRSMB03:
			return "SMBus Transaction Failed";
		case ERRSMB04:
			return "SMBus Collision";
		case ERRSMB05:
			return "SMBus No Response";
		default:
			return "Unknown SMBus Error";
	}
}

void smb_list()
{
	log_debug("%s: Listing SMBus Slave Devices on Address 0x%04X...\n",FNAME,smb_addr);
	u16 i, j;
	u16 addr;
	int ret;
	u8 cmd = 0x00;
	log_debug("i#\tj#\taddr\tret\n");
	for (i = 0; i < 128; i+= 16)
	{
		for (j = 0; j < 16; j++)
		{
			addr = i+j;
			ret = -1;
			/* Skip unwanted addresses */
			if (addr < 0x03 || addr > 0x77)
				continue;
	
			if ((addr >= 0x30 && addr <= 0x37)
				|| (addr >= 0x50 && addr <= 0x5F))
			{
				ret = smb_read_byte(addr, cmd);
			} else {

				ret = smb_write_quick(addr, cmd);
			}	

			if(ret >= 0)
			{
				log_debug("0x%02X\t0x%1x\t0x%02X\t%2i\n",i,j,addr,ret);			
			}
			
		}
	}
}

