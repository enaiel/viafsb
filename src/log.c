/*******************************************************************************

  log.c: Log wrapper implementation to provide standard or debug outputs
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
#include<stdarg.h>

#include"include/types.h"
#include"include/log.h"

bool debug = FALSE;

void log_set_debug(bool val)
{
	debug = val;
}

void log_bits(int num,int size) 
{
	if(!debug) return;
	for (int i=size;i;i--,putchar('0'|((num>>i)&1)));
}

int log_all(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	int ret = vprintf(msg, args);
	va_end(args);
	return ret;
}

int log_debug(const char *msg, ...)
{
	if(!debug) return -1;
	va_list args;
	va_start(args, msg);
	int ret = vprintf(msg, args);
	va_end(args);
	return ret;
}

int log_no_debug(const char *msg, ...)
{
	if(debug) return -1;
	va_list args;
	va_start(args, msg);
	int ret = vprintf(msg, args);
	va_end(args);
	fflush(stdout);
	return ret;
}

