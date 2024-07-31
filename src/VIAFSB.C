/*******************************************************************************

  viafsb.c: Main program for VIAFSB
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
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<ctype.h>
#include<dos.h>

#include "include/types.h"
#include "include/log.h"
#include "include/smb.h"
#include "include/pci.h"
#include "include/pll.h"

/* VIA PCI IDs */
#define PCI_VENDOR_ID_VIA		0x1106
#define PCI_DEVICE_ID_VIA_82C596A	0x3050
#define PCI_DEVICE_ID_VIA_82C596B	0x3051
#define PCI_DEVICE_ID_VIA_82C686	0x3057
#define PCI_DEVICE_ID_VIA_8231		0x8235
#define PCI_DEVICE_ID_VIA_8233		0x3074
#define PCI_DEVICE_ID_VIA_8233A		0x3147
#define PCI_DEVICE_ID_VIA_8233C		0x3109
#define PCI_DEVICE_ID_VIA_8235		0x3177
#define PCI_DEVICE_ID_VIA_8237		0x3227
#define PCI_DEVICE_ID_VIA_8237A		0x3337
#define PCI_DEVICE_ID_VIA_8237S		0x3372
#define PCI_DEVICE_ID_VIA_8251		0x3287

/* VIA SMB Addresses */
#define SMB_ADDR_1	0x90 
#define SMB_ADDR_2	0x80 
#define SMB_ADDR_3	0xD0 

#define SMB_DEF_ADDR	0x5000
#define PLL_SLAVE_ADDR	0x69

/* VIA SMBus Registers */
#define SMB_HST_CFG	0xD2
#define SMB_REV_ID	0xD6

#define FNAME		"VIAFSB"
#define VIAFSB_VER	"0.3.0"

/* VIA SMB Error Codes */
#define ERRVIAFSB	200
#define ERRVIAFSB01	201
#define ERRVIAFSB02	202
#define ERRVIAFSB03	203
#define ERRVIAFSB04	204
#define ERRVIAFSB05	205
#define ERRVIAFSB06	206
#define ERRVIAFSB07	207
#define ERRVIAFSB08	208
#define ERRVIAFSB09	209
#define ERRVIAFSB10	210
#define ERRVIAFSB11	211


/* VIA SMBus */
struct via_smb {
	u16 bus;
	u16 dev;
	u16 fun;
	u32 addr;
	u16 vendor_id;
	u16 device_id;
	u16 smb_cfg_addr;
	u16 smb_addr;
	u16 smb_rev_id;
};

const u16 supp_sb[] = {
	PCI_DEVICE_ID_VIA_82C596A, 
	PCI_DEVICE_ID_VIA_82C596B,
	PCI_DEVICE_ID_VIA_82C686,
	PCI_DEVICE_ID_VIA_8231,	
	PCI_DEVICE_ID_VIA_8233,	
	PCI_DEVICE_ID_VIA_8233A,
	PCI_DEVICE_ID_VIA_8233C,
	PCI_DEVICE_ID_VIA_8235,	
	PCI_DEVICE_ID_VIA_8237,	
	PCI_DEVICE_ID_VIA_8237A,
	PCI_DEVICE_ID_VIA_8237S,
	PCI_DEVICE_ID_VIA_8251	
};	

static const pll_rec *curr_pll = NULL;


bool is_supp_via_sb(u16 vendor_id, u16 device_id)
{
	if(vendor_id != PCI_VENDOR_ID_VIA)
		return FALSE;
	int size = sizeof supp_sb / sizeof supp_sb[0];
	for(int i=0; i<size; i++)
	{
		if(device_id == supp_sb[i])
			return TRUE;
	}
	return FALSE;
}

bool get_via_smb_cfg_addr(struct via_smb *smb)
{
	switch(smb->device_id)
	{
		case PCI_DEVICE_ID_VIA_82C596A:
		case PCI_DEVICE_ID_VIA_82C596B:
		case PCI_DEVICE_ID_VIA_82C686:
		case PCI_DEVICE_ID_VIA_8231:
			smb->smb_cfg_addr=SMB_ADDR_1;
			return TRUE;
		case PCI_DEVICE_ID_VIA_8233:
		case PCI_DEVICE_ID_VIA_8233A:
		case PCI_DEVICE_ID_VIA_8233C:
		case PCI_DEVICE_ID_VIA_8235:
		case PCI_DEVICE_ID_VIA_8237:
		case PCI_DEVICE_ID_VIA_8237A:
		case PCI_DEVICE_ID_VIA_8237S:
		case PCI_DEVICE_ID_VIA_8251:
			smb->smb_cfg_addr=SMB_ADDR_3;
			return TRUE;
		default:
			return FALSE;
	}
	return FALSE;
}

bool get_via_smb_addr(struct via_smb *smb)
{
	u16 val;
	pci_read_cfg_word(0,smb->dev,smb->fun,smb->smb_cfg_addr,&val);
	if(val == 0xffff || val == 0)
		return FALSE;
	smb->smb_addr = val;
	return TRUE;
}

bool is_via_smb_enabled(struct via_smb *smb)
{
	u8 val;
	pci_read_cfg_byte(0,smb->dev,smb->fun,SMB_HST_CFG,&val);
	if(val == 0xff || val == 0)
	{	
		log_debug("VIA SMBus is not enabled. Force enabling...\n");
		val= 0x01;
		pci_write_cfg_byte(0,smb->dev,smb->fun,SMB_HST_CFG,val);
		pci_read_cfg_byte(0,smb->dev,smb->fun,SMB_HST_CFG,&val);
		if(val == 0xff || val == 0)
			return FALSE;
	}
	pci_read_cfg_byte(0,smb->dev,smb->fun,SMB_REV_ID,&val);
	if(val != 0xff)
	{	
		smb->smb_rev_id = val;
	}
	return TRUE;
}

const char* get_via_sb_desc(u16 device_id)
{
	switch(device_id)
	{
		case PCI_DEVICE_ID_VIA_82C596A:
			return "VT82C596/A";
		case PCI_DEVICE_ID_VIA_82C596B:
			return "VT82C596B";
		case PCI_DEVICE_ID_VIA_82C686:
			return "VT82C686/A/B";
		case PCI_DEVICE_ID_VIA_8231:
			return "VT8231";
		case PCI_DEVICE_ID_VIA_8233:
			return "VT8233";
		case PCI_DEVICE_ID_VIA_8233A:
			return "VT8233A";
		case PCI_DEVICE_ID_VIA_8233C:
			return "VT8233C";
		case PCI_DEVICE_ID_VIA_8235:
			return "VT8235";
		case PCI_DEVICE_ID_VIA_8237:
			return "VT8237/R";
		case PCI_DEVICE_ID_VIA_8237A:
			return "VT8237A";
		case PCI_DEVICE_ID_VIA_8237S:
			return "VT8237S";
		case PCI_DEVICE_ID_VIA_8251:
			return "VT8251";
		default:
			return "";
	}
	return "";
}

bool find_via(struct via_smb *smb)
{
	u16 bus,dev,fun;
	u32 addr, val;
	u16 vendor_id, device_id;
#ifdef DEBUG
	log_debug("bus#\tdev#\tfun#\taddr#\t\tval\t\tvendor\tdevice\n");
#endif
	for(bus = 0; bus < PCI_MAX_BUS; bus++)
		for(dev = 0; dev < PCI_MAX_DEV; dev++)
			for(fun = 0; fun < PCI_MAX_FUN; fun++)
			{	
				addr = pci_get_addr(bus, dev, fun, 0);
				pci_read_cfg_int(bus, dev, fun, 0, &val);
				if((val!= 0xffffffff)&&(val!= 0))
				{
					vendor_id = val& 0x0000ffff;
					device_id = (val& 0xffff0000) >> 16;
					if(is_supp_via_sb(vendor_id, device_id))
					{
#ifdef DEBUG
						log_debug("0x%02X\t0x%02X\t0x%02X\t0x%08X\t0x%08X\t0x%04X\t0x%04X\n",bus,dev,fun,addr,val,vendor_id,device_id); 
#endif
						smb->bus = bus;
						smb->dev = dev;
						smb->fun = fun;
						smb->addr = addr;
						smb->vendor_id = vendor_id;
						smb->device_id = device_id;
						return TRUE;
					}
				}
			}
	return FALSE;
}

bool find_pll()
{
	int ret;
	u8 cmd = 0x00;
	u8 addr = PLL_SLAVE_ADDR;
	ret = smb_write_quick(addr, cmd); 	
#ifdef DEBUG
	log_debug("addr\tret\n");
	log_debug("0x%02X\t%2i\n",addr,ret);			
#endif
	if(ret >= 0)
		return TRUE;
	else
		return FALSE;
}

bool set_pll(const char *name)
{
	int i;
	int size = sizeof pll_tbl / sizeof pll_tbl[0];
	for(i=0; i<size; i++)
	{
		if(!strcasecmp(name, pll_tbl[i].name))
		{
			curr_pll = &pll_tbl[i];
			return TRUE;
		}
	}
	return FALSE;
}

void list_sb()
{
	int size = sizeof supp_sb / sizeof supp_sb[0]; 
	for(int i=0; i< size; i++)
		log_all(" %s", get_via_sb_desc(supp_sb[i]));
	log_all("\n");
}

void list_pll()
{
	int size = sizeof pll_tbl / sizeof pll_tbl[0]; 
	for(int i=0; i< size; i++)
		log_all(" %s", pll_tbl[i].name);
	log_all("\n");
}

int get_pci_div(float fsb, float pci)
{
	return (int)roundl(fsb / pci);
}

void list_fsb(float fsb, float pci, bool unsafe)
{
	float fsb_t, pci_t;
	u8 fsb_key_t;
	int pci_div_t;
	int pci_div = get_pci_div(fsb, pci);
	int size = curr_pll->get_supp_fsb_size();
	bool found = FALSE;
	for (int i=0; i<size; i++)
	{	curr_pll->get_supp_fsb(i, &fsb_t, &pci_t, &fsb_key_t, &pci_div_t); 
		if(unsafe || !pci || pci_div == pci_div_t)
		{
			if(found) log_all("\t");
			log_all("%.2f[/%.2f]", fsb_t, pci_t);
			if(!found) found = TRUE;
		}
	}
	log_all("\n");
}

bool is_supp_fsb(float fsb_p, float *pci_p, float fsb, float pci, bool unsafe)
{
	float fsb_t, pci_t;
	u8 fsb_key_t;
	int pci_div_t;
	int pci_div = get_pci_div(fsb, pci);
	int size = curr_pll->get_supp_fsb_size();
	for (int i=0; i<size; i++)
	{	curr_pll->get_supp_fsb(i, &fsb_t, &pci_t, &fsb_key_t, &pci_div_t); 
		if(fsb_p == fsb_t)
		{
			if((!*pci_p || *pci_p == pci_t) && (unsafe || pci_div == pci_div_t))
			{
				if(!*pci_p) *pci_p = pci_t;
				return TRUE;
			}
		}
	}
	return FALSE;	
}

int get_fsb_pci(char *argv, float *fsb_p, float *pci_p)
{
	char *tok = strtok(argv, " /");
	if(tok)
	{
		*fsb_p = atof(tok);	
	}
	tok = strtok(NULL, " /");
	if(tok)
	{
		*pci_p = atof(tok);	
	}
	return *fsb_p;
}

int check_smb(struct via_smb *smb)
{
	log_no_debug("VIA Southbridge: Checking... ");
	if(!find_via(smb))
	{	
		log_no_debug("ERROR\nNo supported VIA Southbridge found\n");
		log_no_debug("Supported VIA Southbridge are");
		
		log_debug("%s: No supported VIA Southbridge found\n",FNAME);
		log_debug("%s: Listing supported VIA Southbridge:",FNAME);
		list_sb();
		return -ERRVIAFSB01;
	}
	log_no_debug("Detected ");
	log_no_debug(get_via_sb_desc(smb->device_id));
	log_no_debug("\n");
	log_debug("%s: Found supported VIA Southbridge: ",FNAME);
	log_debug(get_via_sb_desc(smb->device_id));
	log_debug("\n");
	log_no_debug("SMBus: Checking... ");
	if(!get_via_smb_cfg_addr(smb))
	{
		log_no_debug("ERROR\nNo SMBus Cofig Address found\n");
		log_debug("%s: No SMBus Config Address found\n",FNAME);
		return -ERRVIAFSB02;
	}
	log_debug("%s: Using SMBus Config Address: 0x%02X\n", FNAME,smb->smb_cfg_addr);
	if(!get_via_smb_addr(smb))
	{
		log_no_debug("ERROR\nNo SMBus Address found\n");
		log_debug("%s: No SMBus Address found\n", FNAME);
		return -ERRVIAFSB03;
	}
	log_debug("%s: Found SMBus Address: 0x%04X\n", FNAME, smb->smb_addr);
	smb->smb_addr -= 1;	
	log_debug("%s: Using instead SMBus Address: 0x%04X\n", FNAME, smb->smb_addr);
	if(!is_via_smb_enabled(smb))
	{
		log_no_debug("ERROR\nSMBus is not enabled\n");
		log_debug("%s: SMBus is not enabled and cannot be forced\n",FNAME);
		return -ERRVIAFSB04;
	}
	log_no_debug("SMBus is enabled\n");
	log_debug("%s: SMBus is enabled\n", FNAME);
	log_debug("%s: VIA Southbridge Revision ID: 0x%02X\n", FNAME, smb->smb_rev_id);
	smb_set_addr(smb->smb_addr);
	return 1;
}

int check_pll(char *pll_name_p)
{
	log_debug("%s: Using PLL %s...\n", FNAME, pll_name_p);
	log_no_debug("PLL: Using %s... ", pll_name_p);
	if(!set_pll(pll_name_p))
	{
		log_no_debug("ERROR\nRequested PLL %s is not supported\n",pll_name_p);
		log_no_debug("Supported PLL are");
		log_debug("%s: PLL %s is not supported\n", FNAME, pll_name_p);
		log_debug("%s: Listing supported PLL:",FNAME);
		list_pll();
		return -ERRVIAFSB05;
	}
	log_debug("%s: PLL %s is supported\n", FNAME, pll_name_p);
	log_no_debug("Testing... ");
	if(curr_pll->can_test())
	{
		if(!find_pll())
		{
			log_no_debug("ERROR\nCannot contact PLL on SMBus\n");
			log_debug("%s: No PLL found at SMBus Slave Address 0x%02X\n", FNAME, PLL_SLAVE_ADDR);
			return -ERRVIAFSB06;
		}
		log_debug("%s: Found PLL at SMBus Slave Address: 0x%02X\n", FNAME, PLL_SLAVE_ADDR);
	}
	else
	{
		log_no_debug("Skipping... ");
		log_debug("%s: PLL %s does not support testing\n", FNAME, pll_name_p);
	}
	return 1;
}

void print_header(bool unsafe)
{
	log_all("VIAFSB v%s - DOS FSB utility for VIA chipsets.", VIAFSB_VER);
	if(unsafe) log_all(" UNSAFE MODE ENABLED!");
	log_all("\n");
}

void print_usage()
{
	print_header(FALSE);
	log_all("Supported VIA chipsets:");
	int size = sizeof supp_sb / sizeof supp_sb[0]; 
	for(int i=0; i< 6; i++)
		log_all(" %s", get_via_sb_desc(supp_sb[i]));
	log_all("\n                       ");
	for(int i=6; i< size; i++)
		log_all(" %s", get_via_sb_desc(supp_sb[i]));
	log_all("\n");
	log_all("Supported PLL:");
	size = sizeof pll_tbl / sizeof pll_tbl[0]; 
	for(int i=0; i< 6; i++)
		log_all(" %s", pll_tbl[i].name);
	log_all("\n              ");
	for(int i=6; i< 13; i++)
		log_all(" %s", pll_tbl[i].name);
	log_all("\n              ");
	for(int i=13; i< size; i++)
		log_all(" %s", pll_tbl[i].name);
	log_all("\n");
	log_all("\n"
		"	Usage:   VIAFSB pll_name [fsb_freq[/pci_freq]] [-u|--unsafe]\n"
		"	Example: VIAFSB ICS94211		   / Get FSB\n"
		"	         VIAFSB ICS94211 100.23		   / Set FSB\n"
		"	         VIAFSB ICS94211 100.23/33.41	   / Set FSB/PCI\n"
		"	         VIAFSB ICS94211 150.00/37.50 -u   / Set FSB/PCI in UNSAFE MODE\n"
		"\n"
	"Author: Enaiel <enaiel@gmail.com> (c) 2022. WARNING: USE AT YOUR OWN RISK!\n");
}

int get_opts(int argc, char* argv[], char **pll_name_p, float *fsb_p, float *pci_p, bool *debug, bool *unsafe)
{
	if(argc < 2 || argc > 5) 
		return 0;
	for (int i=1; i<argc; i++)
	{
		if(!strcasecmp(argv[i], "-h") || !strcasecmp(argv[i], "--help")) 
			return 0;
		else if(!strcasecmp(argv[i], "-d") || !strcasecmp(argv[i], "--debug")) 
		{
			*debug = TRUE;
		}
		else if(!strcasecmp(argv[i], "-u") || !strcasecmp(argv[i], "--unsafe")) 
		{
			*unsafe = TRUE;
		}
		else if (*pll_name_p == NULL)
		{
			*pll_name_p = argv[i];
			for(int i=0; i<strlen(*pll_name_p); i++)
				(*pll_name_p)[i] = toupper((*pll_name_p)[i]);
		}
		else if (!*fsb_p)
			get_fsb_pci(argv[i], fsb_p, pci_p); 
		else
			return 0;
	}
	if((argc < 3 && (*debug || *unsafe)) || (argc < 4 && (*debug && *unsafe)))
		return 0;
	return 1;
}

int run(char *pll_name_p, float fsb_p, float pci_p, bool debug, bool unsafe)
{
	float fsb, pci;
	u8 fsb_key;
	int pci_div; 
	int ret = -1;
	log_set_debug(debug);
	print_header(unsafe);
	if(fsb_p)
		log_debug("%s: Trying to set FSB to %.2f/%.2f using PLL %s...\n",FNAME,fsb_p,pci_p,pll_name_p);
	else
		log_debug("%s: Trying to get current FSB using PLL %s...\n",FNAME,pll_name_p);
	struct via_smb smb = {};
	ret = check_smb(&smb);
	if(ret < 0) return ret;
	ret = check_pll(pll_name_p);
	if(ret < 0) return ret;
	log_no_debug("Getting FSB... ");
	if(!curr_pll->can_read())
	{
		unsafe = TRUE;
		if(!fsb_p)
		{
			log_no_debug("ERROR\nUnable to get FSB as PLL %s does not support reading\n",pll_name_p);
			log_debug("%s: Unable to get FSB as PLL %s does not support reading\n", FNAME, pll_name_p);
			log_no_debug("Supported FSB for PLL %s",pll_name_p);
			log_debug("%s: Listing supported FSB for PLL %s",FNAME,curr_pll->name);
			log_all(" (all PCI dividers)"); 
			log_no_debug(" are");
			log_all(":\n");
			list_fsb(fsb, pci, unsafe);
		}
		else
		{
			log_no_debug("Skipping... ");
			log_debug("%s: PLL %s does not support reading\n", FNAME, pll_name_p);
		}
	}
	else
	{
		if(!curr_pll->get_fsb(&fsb, &pci, &fsb_key, &pci_div))
		{
			log_no_debug("ERROR\nError while reading FSB from PLL %s\n",pll_name_p);
			log_debug("%s: Unable to read FSB from PLL %s\n", FNAME, pll_name_p);
			return -ERRVIAFSB08;
		}
		if(!fsb_p)
		{
			log_no_debug("DONE\n"); 
			log_no_debug("FSB currently at %.2f/%.2f MHz\n", fsb, pci);
			log_no_debug("Supported FSB for PLL %s",pll_name_p);
			log_debug("%s: Listing supported FSB for PLL %s",FNAME,curr_pll->name);
			if(fsb && !unsafe)
				log_all(" (PCI divider %i)",get_pci_div(fsb, pci)); 
			else
				log_all(" (all PCI dividers)"); 
			log_no_debug(" are");
			log_all(":\n");
			list_fsb(fsb, pci, unsafe);
		}
		log_debug("%s: Got FSB from PLL %s: %.2f/%.2f\n",FNAME, pll_name_p, fsb, pci);
	}
	if(fsb_p)
	{
		log_no_debug("Setting FSB... ");
		if(!is_supp_fsb(fsb_p, &pci_p, fsb, pci, unsafe))
		{
			log_no_debug("ERROR\nRequested FSB %.2f/%.2f is not supported by PLL %s",fsb_p,pci_p,pll_name_p);
			log_debug("%s: Requested FSB %.2f/%.2f is not supported PLL %s", FNAME, fsb_p, pci_p, pll_name_p);
			if(fsb && !unsafe)
				log_all(" (PCI divider %i)",get_pci_div(fsb, pci)); 
			else
				log_all(" (all PCI dividers)"); 
			log_all("\n");
			log_no_debug("Supported FSB for PLL %s",pll_name_p);
			log_debug("%s: Listing supported FSB for PLL %s",FNAME,curr_pll->name);
			if(fsb && !unsafe)
				log_all(" (PCI divider %i)",get_pci_div(fsb, pci)); 
			else
				log_all(" (all PCI dividers)"); 
			log_no_debug(" are");
			log_all(":\n");
			list_fsb(fsb, pci, unsafe);
			return -ERRVIAFSB09;
		}
		if(curr_pll->can_read())
		{
			if(fsb_p == fsb && (!pci_p || pci_p == pci))
			{
				log_no_debug("ERROR\nRequested FSB %.2f/%.2f is same as current FSB %.2f/%.2f\n",fsb_p, pci_p, fsb, pci);
				log_debug("%s: Requested FSB %.2f/%.2f is same as current FSB %.2f/%.2f\n", FNAME, fsb_p, pci_p, fsb, pci);
				return -ERRVIAFSB10;
			}
		}
		log_debug("%s: Requested FSB %.2f/%.2f is supported by PLL %s", FNAME, fsb_p, pci_p, pll_name_p);
		if(fsb && !unsafe)
			log_debug(" (PCI divider %i)",get_pci_div(fsb, pci)); 
		else
			log_debug(" (all PCI dividers)"); 
		log_debug("\n");
		ret = curr_pll->set_fsb(fsb_p, pci_p, debug);
		if(ret < 0)
		{
			log_no_debug("ERROR\nError while setting FSB %.2f/%.2f using PLL %s\n", fsb_p, pci_p, pll_name_p);
			log_debug("%s: Unable to set FSB %.2f/%.2f using PLL %s\n", FNAME, fsb_p, pci_p, pll_name_p);
			return -ERRVIAFSB11;
		}
		log_no_debug("DONE\n");
		log_no_debug("FSB set to %.2f/%.2f MHz\n", fsb_p, pci_p);
		log_debug("%s: Successfully set FSB %.2f/%.2f using PLL %s!\n", FNAME, fsb_p, pci_p, pll_name_p);
	}
	fflush(stdout);
	return 0;
}

int main(int argc, char *argv[])
{
	float fsb_p = 0;
	float pci_p = 0;
	char *pll_name_p = NULL;
	bool debug = FALSE;
	bool unsafe = FALSE;
	if(!get_opts(argc, argv, &pll_name_p, &fsb_p, &pci_p, &debug, &unsafe))
	{
		print_usage();
		return -1;
	}
	return run(pll_name_p, fsb_p, pci_p, debug, unsafe);
}
