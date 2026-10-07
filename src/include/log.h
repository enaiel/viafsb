/*******************************************************************************

  log.h: Log wrapper interface to log standard or debug output
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

#ifndef	__LOG_H_
#define __LOG_H_

#include<stdio.h>
#include<stdarg.h>

#include"types.h"

void log_set_debug(bool debug);
void log_bits(int num, int size);
int log_all(const char *msg, ...);
int log_debug(const char *msg, ...);
int log_no_debug(const char *msg, ...);

#endif	// __LOG_H_
