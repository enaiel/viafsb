VIAFSB - DOS FSB utility for VIA chipsets.
=================================================
VIAFSB is a little DOS utility I wrote for changing the FSB on the couple of 
VIA motherboards that I own. Why should only owners of 440BX motherboards have 
all the fun? Sure, 440BX motherboards are faster, more stable, and more 
cooler... but at least now I can change the FSB on my VIA motherboards in DOS 
just like they can! Anyway, I thought I'll share it in case it might be useful 
to someone else. It initially only supported the 2 PLL that I had, but I have 
since added support for more. If you can find the datasheet for your PLL, I 
can try to add support for it.

SUPPORTED
---------
```  
VIA chipsets: VT82C596/A/B VT82C686/A/B VT8231 VT8233/A/C VT8235 VT8237/R/A/S 
              VT8251
PLL: CY28316 ICS9148-37 ICS9248-127 ICS94211 ICS94215 ICS94241 ICS950405 
     ICS950908 PLL205-03 W124 W156C W230-03H W83194BR-39B W83195R-08

	Usage:   VIAFSB pll_name [fsb_freq[/pci_freq]] [-u|--unsafe]
	Example: VIAFSB ICS94211		   / Get FSB
	         VIAFSB ICS94211 100.23		   / Set FSB
	         VIAFSB ICS94211 100.23/33.41	   / Set FSB/PCI
	         VIAFSB ICS94211 150.00/37.50 -u   / Set FSB/PCI in UNSAFE MODE
```

PARAMETERS
----------
```
-h|--help	Print the Help screen.	
[pll_name]	Select the PLL to use. If not supported, will list all 
		supported PLL.
[fsb_freq]	Select the FSB frequency to set. If not supported by PLL, will
		list all FSB frequencies supported by the selected PLL.
[pci_freq]	Select the PCI frequency for the selected FSB frequency. Will
                determine the PCI divider.
-u|--unsafe	Run in UNSAFE MODE and allow FSB frequency changes across all 
		PCI dividers. Otherwise, tool will restrict FSB frequency 
		changes to those within the current PCI divider.
```

FEATURES
--------
* Allows change of FSB frequency without requiring a restart.
* Runs on a 386 or later x86 processor.
* Compatible with several VIA chipsets.
* Compatible with DOS, with or without EMM386.
* Compatible with Windows 95, 98 and ME.
* Requires CWSDPMI.exe or a compatible DPMI host.
* Can be run from a batch file, or even from autoexec.bat.

DISCLAIMER
----------
WARNING: USE AT YOUR OWN RISK! This software should not be used for mission
critical applications. Author is not responsible for any damage caused to your
computer from using this software.

ALTERNATIVES
------------
CPUFSB/SetFSB/SoftFSB on Windows.

RayeR's SMB for Intel chipsets on DOS.

CREDITS
-------
Author: Enaiel \<enaiel@gmail.com\> (c) 2022. 

Support: http://vogons.org/viewtopic.php?f=46&t=87013

Thanks to:
* Vogons.org member @EduBat for help decoding PLL datasheets.
* Vogons.org members @jheronimus, @Zeerex, @elcrys, @havli, @GremFive, and 
  @Falcosoft for testing the program.
* Linux contributors for Linux i2c bus interface (i2c) source code.
* Linux contributors for Linux i2c VIA Pro driver (i2c-viapro) source code.
* Nikolay Kislitsa \<deusexbeer@gmail.com\> for Linux FSB overclocking tool (lfsb)  source code.  

SOURCE
------
Source is now available at https://github.com/enaiel/viafsb.

License is GPL due to all the hours I spent pouring over Linux and LFSB code 
trying to understand how all of this works.

Built with DJGPP. You can obtain your copy from http://www.delorie.com/djgpp.

TESTED
------

Motherboard               | Southbridge  | PLL         
--------------------------|--------------|--------------
MSI MS-6369     	  | VT82C686/A/B | ICS94211
QDI P6V694T/A10T	  | VT82C686/A/B | CY28316 
QDI P6V694T/A10T	  | VT82C686/A/B | ICS94241 
ABIT KT7A       	  | VT82C686/A/B | PLL-205-03
Unbranded MVP4		  | VT82C686/A/B | W156C
PCPartner AP133TAS3-T205C | VT82C686/A/B | ICS94211

FAQ
---
Q. I do not know what PLL is on my motherboard. The util does not support my 
   PLL. Can I use any PLL that has a similar name to mine?

A. You cannot use just any PLL, even if they are similarly named, because each 
   PLL uses different bits to set the FSB. Also, you cannot google for this 
   information as motherboard manufacturers use different PLL chips with 
   different board revisions, sometimes with the same revision as well! You need
   to find the PLL on your motherboard and identify it. If this util doesn't 
   support your PLL, please provide me the datasheet for it, and I will try to 
   add support for it. 

Q. Why does my computer crash when I use this utility to change the FSB?

A. Stability when changing FSB depends on your motherboard. If the selected FSB 
   hangs the computer when changed from the BIOS, or using programs like 
   CPUFSB/SetFSB/SoftFSB, it will also hang when changed using this program. 
   Also, FSB that is latched by hardware jumpers are inherently more stable than
   those that are selected in the BIOS or set using this program, as the latches
   are not trying to change the FSB on a running CPU.

Q. How do I choose the right FSB frequencies when changing the FSB?

A. With these VIA chipsets, only FSB frequencies that are within the current 
   PCI divider work. Going across PCI dividers crashes the computer, requiring a
   hard reset. To find the divider, divide the FSB frequency by the PCI 
   frequency. Even within the same PCI divider, incrementally change the fsb
   for more stability. The only way to change the PCI divider is to change the 
   boot FSB frequency using hardware jumpers or from the BIOS. Not all 
   motherboards have these jumpers or this ability in their BIOS. VIAFSB will
   now by default restrict the FSB to only those within the current PCI divider.
   Use the new UNSAFE MODE to override this behaviour. 

Q. How do I check if the FSB has actually changed?

A. I use ChkCPU to confirm FSB changes in DOS.

Q. How do I change the multiplier on my CPU?

A. I use SetMul to change the multiplier on my VIA C3 CPU in DOS.

HISTORY
-------
2022-03-11: v0.1.0
* Initial version with support for PLL ICS94211.

2022-03-14: v0.1.1
* Changes to output logging.

2022-03-14: v0.1.2
* Changes to build process.

2022-03-21: v0.1.3
* Added support for PLL CY28316.

2022-03-24: v0.1.4
* Bundled CWSDPMI with program.

2022-03-24: v0.1.5
* Added initial support for PLLs ICS94215, PLL205-03, W156C.

2022-03-28: v0.1.6
* Added initial support for PLL ICS950908.
* Changed logic for PLL W156C since it's write-only. 
* Changed FSB to float and added ability to choose PCI frequency. 

2022-03-29: v0.1.7
* Minor change to hard code SMBus address since not able to get right address 
  when querying VIA Southbridge. 

2022-03-31: v0.1.8
* Reverted previous change, another attempt to fix SMBus address.
* Restrict to current PCI divider by default, unless run in new UNSAFE MODE.

2022-04-01: v0.1.9
* Refactored code to make it easier to add support for new PLLs.
* Added intial support for PLL ICS9148-37.
* Made minor chages to PLL ICS950908.

2022-04-04: v0.2.0
* Added initial support for PLLs W124, W83194BR-39B, W831915R-08. 
* Minor fix for PLL ICS9148-37.

2024-07-31: v0.2.1
* Improved performance by decreasing delay when waiting for SMBus response.
* Sorted FSB table to make it easier to select FSB frequencies.
* Updated command bytes for all PLLs.
* Improved handling of read-only PLLs.
* Improved handling of same FSB frequencies with different PCI values.
* Added initial support for VIA chipsets VT8231 VT8233C VT8237/R VT8237A VT8237S VT8251.
* Added initial support for PPLs ICS9248-127 ICS950405 W230-03H.

2024-07-31: v0.3.0
* Initial Github release.
