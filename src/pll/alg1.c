/*******************************************************************************

  alg1.c: Generic implementation to access PLL functions
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
#include <unistd.h>

#include "../include/types.h"
#include "../include/log.h"
#include "../include/smb.h"
#include "../include/alg1.h"

u8 get_key(u8 fs5, u8 fs4, u8 fs3, u8 fs2, u8 fs1, u8 fs0)
{
	u8 key;
	key = fs0;
	key += fs1 << 1;
	key += fs2 << 2;
	key += fs3 << 3;
	key += fs4 << 4;
	key += fs5 << 5;
	return key;
}

int alg1_set_fsb(const pll_data *pll, float fsb, float pci, bool test)
{
	int i, res = -1;
	u8 key = 0xFF;
	u8 fs5, fs4, fs3, fs2, fs1, fs0;
	u8 buf[pll->byte_count];
	for(i=0; i<pll->byte_count; i++)
		buf[i] = pll->pll_reg[i];  

	if(pll->byte_count_byte != -1)
	{
		buf[pll->byte_count_byte] = pll->byte_count;
		log_debug("%s: Writing BYTE_COUNT_BYTE(%i) (hex bin): %02X ",pll->name, pll->byte_count_byte, buf[pll->byte_count_byte]);
		log_bits(buf[pll->byte_count_byte],8);
		log_debug("\n");
	}

	for(i=0; i<pll->fsb_tbl_size; i++)
	{
		if(pll->fsb_tbl[i].fsb == fsb)
		{
			if(!pci || pll->fsb_tbl[i].pci == pci)
			{
				key = pll->fsb_tbl[i].fsb_key;
				break;
			}
		}
	}
	if(key == 0xFF)
		return -1;
	log_debug("%s: Found key for FSB(%.2f/%.2f) (hex bin): %02X ",pll->name, fsb, pci, key);
	log_bits(key,5);
	log_debug("\n");
	fs0 = get_bit(key, 0);
	fs1 = get_bit(key, 1);
	fs2 = get_bit(key, 2);
	fs3 = get_bit(key, 3);
	fs4 = get_bit(key, 4);
	fs5 = get_bit(key, 5);

	log_debug("%s: FS5 FS4 FS3 FS2 FS1 FS0 (bits): %i %i %i %i %i %i\n", pll->name, fs5, fs4, fs3, fs2, fs1, fs0);

	if(pll->fs_sel_bit != -1)
		buf[pll->fsb_byte] = set_bit(buf[pll->fsb_byte], pll->fs_sel_bit, 1);
	buf[pll->fsb_byte] = set_bit(buf[pll->fsb_byte], pll->fs0_bit, fs0);
	buf[pll->fsb_byte] = set_bit(buf[pll->fsb_byte], pll->fs1_bit, fs1);
	buf[pll->fsb_byte] = set_bit(buf[pll->fsb_byte], pll->fs2_bit, fs2);
	if(pll->fs3_bit != -1)
		buf[pll->fsb_byte] = set_bit(buf[pll->fsb_byte], pll->fs3_bit, fs3);
	if(pll->fs4_bit != -1)
		buf[pll->fsb_byte] = set_bit(buf[pll->fsb_byte], pll->fs4_bit, fs4);
	if(pll->fs5_bit != -1)
		buf[pll->fsb_byte] = set_bit(buf[pll->fsb_byte], pll->fs5_bit, fs5);
	log_debug("%s: Writing FSB_BYTE(%i) (hex bin): %02X ",pll->name, pll->fsb_byte, buf[pll->fsb_byte]);
	log_bits(buf[pll->fsb_byte],8);
	log_debug("\n");
	log_debug("%s: Writing %i bytes (hex): ", pll->name, pll->byte_count);
	for(i=0; i<pll->byte_count; i++) log_debug("%02X ", buf[i]);
	log_debug("\n");
	if(!test)
		res = smb_write_block_data(PLL_ADDR, CMD, pll->byte_count, buf);

	if(res < 0) return -1;

	return 0;
}

int alg1_get_fsb(const pll_data *pll, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	int i, res;
	u8 fs5, fs4, fs3, fs2, fs1, fs0;
	u8 key;
	u8 buf[pll->byte_count];
	for(i=0; i<pll->byte_count; i++)
		buf[i] = pll->pll_reg[i];  

	res = smb_read_block_data(PLL_ADDR, CMD, pll->byte_count, buf);

	if(res < 0) return -1;

	log_debug("%s: Read %i bytes (hex): ", pll->name, res);
	for(i=0; i<res; i++) log_debug("%02X ", buf[i]);
	log_debug("\n");
	log_debug("%s: FSB_BYTE(%i) read (hex bin): %02X ",pll->name, pll->fsb_byte, buf[pll->fsb_byte]);
	log_bits(buf[pll->fsb_byte],8);
	log_debug("\n");
	
	if(pll->fs_sel_bit == -1 || get_bit(buf[pll->fsb_byte], pll->fs_sel_bit))
	{
		log_debug("%s: FS_SEL_BIT(%i) is set in FSB_BYTE(%i). Getting FSB from FSB_BYTE...\n",pll->name, pll->fs_sel_bit, pll->fsb_byte);
		if(pll->fs5_bit != -1)
			fs5 = get_bit(buf[pll->fsb_byte], pll->fs5_bit);
		else
			fs5 = 0;
		if(pll->fs4_bit != -1)
			fs4 = get_bit(buf[pll->fsb_byte], pll->fs4_bit);
		else
			fs4 = 0;
		if(pll->fs3_bit != -1)
			fs3 = get_bit(buf[pll->fsb_byte], pll->fs3_bit);
		else
			fs3 = 0;
		fs2 = get_bit(buf[pll->fsb_byte], pll->fs2_bit);
		fs1 = get_bit(buf[pll->fsb_byte], pll->fs1_bit);
		fs0 = get_bit(buf[pll->fsb_byte], pll->fs0_bit);
		log_debug("%s: FS5 FS4 FS3 FS2 FS1 FS0 (bits): %i %i %i %i %i %i\n", pll->name, fs5, fs4, fs3, fs2, fs1, fs0);
	} else
	{
		log_debug("%s: FS_SEL_BIT(%i) is NOT set in FSB_BYTE(%i). Getting FSB from Latches...\n",pll->name, pll->fs_sel_bit, pll->fsb_byte);
		if(pll->lfs5_bit != -1)
			fs5 = get_bit(buf[pll->lfs5_byte], pll->lfs5_bit);
		else
			fs5 = 0;
		if(pll->lfs4_bit != -1)
			fs4 = get_bit(buf[pll->lfs4_byte], pll->lfs4_bit);
		else
			fs4 = 0;
		if(pll->lfs3_bit != -1)
			fs3 = get_bit(buf[pll->lfs3_byte], pll->lfs3_bit);
		else
			fs3 = 0;
		fs2 = get_bit(buf[pll->lfs2_byte], pll->lfs2_bit);
		fs1 = get_bit(buf[pll->lfs1_byte], pll->lfs1_bit);
		fs0 = get_bit(buf[pll->lfs0_byte], pll->lfs0_bit);
		log_debug("%s: FS5 FS4 FS3 FS2 FS1 FS0 (bits): %i %i %i %i %i %i\n", pll->name, fs5, fs4, fs3, fs2, fs1, fs0);
		if(pll->lfs_inv)
		{
			if(pll->lfs5_bit != -1)
				fs5 = !fs5;
			if(pll->lfs4_bit != -1)
				fs4 = !fs4;
			if(pll->lfs3_bit != -1)
				fs3 = !fs3;
			fs2 = !fs2;
			fs1 = !fs1;
			fs0 = !fs0;
			log_debug("%s: FS5 FS4 FS3 FS2 FS1 FS0 (inverted bits): %i %i %i %i %i %i\n", pll->name, fs5, fs4, fs3, fs2, fs1, fs0);
		}
	}

	key = get_key(fs5, fs4, fs3, fs2, fs1, fs0);
	log_debug("%s: Got key for (%i %i %i %i %i %i) (hex bin): %02X ",pll->name, fs5, fs4, fs3 ,fs2, fs1, fs0, key);
	log_bits(key,5);
	log_debug("\n");

	for(i=0; pll->fsb_tbl[i].fsb; i++)
	{
		if(pll->fsb_tbl[i].fsb_key == key) 
		{
			*fsb = pll->fsb_tbl[i].fsb;
			*pci = pll->fsb_tbl[i].pci;
			*fsb_key = pll->fsb_tbl[i].fsb_key;
			*pci_div = pll->fsb_tbl[i].pci_div;
			return 1;
		}
	}
	return 0;
}

int alg1_get_supp_fsb(const pll_data *pll, int idx, float *fsb, float *pci, u8 *fsb_key, int *pci_div)
{
	if(idx < 0 || idx >= pll->fsb_tbl_size)
		return 0;
	*fsb = pll->fsb_tbl[idx].fsb;
	*pci = pll->fsb_tbl[idx].pci;
	*fsb_key = pll->fsb_tbl[idx].fsb_key;
	*pci_div = pll->fsb_tbl[idx].pci_div;
	return 1;
}

int alg1_get_supp_fsb_size(const pll_data *pll)
{
	return pll->fsb_tbl_size;
}

bool alg1_can_test(const pll_data *pll)
{
	return pll->can_test;
}

bool alg1_can_read(const pll_data *pll)
{
	return pll->can_read;
}

bool alg1_test(const pll_data *pll)
{
	int ret;
	ret = smb_write_quick(PLL_ADDR, CMD); 	
	return ret;
}

void alg1_print_cfg(const pll_data *pll)
{
	int fs_size;
	u8 buf[pll->byte_count];
	for(int i=0; i<pll->byte_count; i++)
		buf[i] = pll->pll_reg[i];  
	if(pll->byte_count_byte != -1)
		buf[pll->byte_count_byte] = pll->byte_count;

	if(pll->fs5_bit != -1)
		fs_size = 6;
	else if(pll->fs4_bit != -1)
		fs_size = 5;
	else if(pll->fs3_bit != -1)
		fs_size = 4;
	else 
		fs_size = 3; 
	
	printf("PLL: %s\n", pll->name);
	printf("Can Read: %c\t", pll->can_read?'Y':'N');
	printf("Can Test: %c\n", pll->can_test?'Y':'N');
	printf("Frequency Table:\n");
	for(int i=fs_size-1; i>=0; i--)
		printf("FS%i\t",i);
	printf("CPU\tPCI\n");
	for(int key=0; key<pll->fsb_tbl_size; key++)
	{
		for(int i=fs_size-1; i>=0; i--)
		{
			printf("%i\t",get_bit(key, i));
		}
		for(int i=0; pll->fsb_tbl[i].fsb; i++)
		{
			if(pll->fsb_tbl[i].fsb_key == key) 
			{
				printf("%.2f\t%.2f",pll->fsb_tbl[i].fsb,pll->fsb_tbl[i].pci);
				break;
			}
		}
		printf("\n");
	}
	printf("I2C Registers:\n");	
	printf("Byte\tPWD\t");
	for(int i=7; i>0; i--)
		printf("%i\t",i);
	printf("0\n");
	for(int i=0; i<pll->byte_count; i++)
	{
		printf("%i\t%02X\t",i,buf[i]);
		for(int j=7; j>=0; j--)
		{
			if(i == pll->fsb_byte && pll->fs_sel_bit == j)
					printf("FSEL");
			else if(i == pll->fsb_byte && pll->fs0_bit == j)
					printf("FS0");
			else if(i == pll->fsb_byte && pll->fs1_bit == j)
					printf("FS1");
			else if(i == pll->fsb_byte && pll->fs2_bit == j)
					printf("FS2");
			else if(i == pll->fsb_byte && pll->fs3_bit == j)
					printf("FS3");
			else if(i == pll->fsb_byte && pll->fs4_bit == j)
					printf("FS4");
			else if(i == pll->fsb_byte && pll->fs5_bit == j)
					printf("FS5");
			else if(i == pll->lfs0_byte && pll->lfs0_bit == j)
					pll->lfs_inv?printf("!LFS0"):printf("LFS0");
			else if(i == pll->lfs1_byte && pll->lfs1_bit == j)
					pll->lfs_inv?printf("!LFS1"):printf("LFS1");
			else if(i == pll->lfs2_byte && pll->lfs2_bit == j)
					pll->lfs_inv?printf("!LFS2"):printf("LFS2");
			else if(i == pll->lfs3_byte && pll->lfs3_bit == j)
					pll->lfs_inv?printf("!LFS3"):printf("LFS3");
			else if(i == pll->lfs4_byte && pll->lfs4_bit == j)
					pll->lfs_inv?printf("!LFS4"):printf("LFS4");
			else if(i == pll->lfs5_byte && pll->lfs5_bit == j)
					pll->lfs_inv?printf("!LFS5"):printf("LFS5");
			else if(pll->byte_count_byte != -1 && i == pll->byte_count_byte)
				printf("BC%i",j);
			else 
				printf("%i", get_bit(buf[i],j));
			if(j>0)
				printf("\t");
		}
		printf("\n");
	}
}
