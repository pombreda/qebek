/*
 * 
 *
 * Copyright (C) 2009 Chengyu Song
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <stdio.h>
#include "qebek-common.h"
#include "qebek-op.h"
#include "qebek-console.h"


void qebek_get_service_address(CPUX86State *env)
{
	if(qebek_service_init)
		return;
	
	target_ulong pkthread = 0xffdff124, psdt, pssdt; //structure pointer
	target_ulong kthread, sdt, ssdt; //virtual address

	if(!qebek_read_ulong(env, pkthread, &kthread))
	{
		qemu_printf("qebek_get_service_address: failed to read kthread\n");
		return;
	}
		
	psdt = kthread + 0xe0;
	if(!qebek_read_ulong(env, psdt, &sdt))
	{
		qemu_printf("qebek_get_service_address: failed to read SDT\n");
		return;
	}

	pssdt = sdt;
	if(!qebek_read_ulong(env, pssdt, &ssdt))
	{
		qemu_printf("qebek_get_service_address: failed to read SSDT\n");
		return;
	}

	index_NtRequestWaitReplyPort = 0x0c5;
	index_NtSecureConnectPort = 0x0d2;
	index_NtClose = 0x019;
	index_NtReadFile = 0x0b7;
	index_NtWriteFile = 0x112;
	
	qebek_read_ulong(env, ssdt + index_NtRequestWaitReplyPort * 4, &NtRequestWaitReplyPort);
	qebek_read_ulong(env, ssdt + index_NtSecureConnectPort * 4, &NtSecureConnectPort);
	qebek_read_ulong(env, ssdt + index_NtClose * 4, &NtClose);
	qebek_read_ulong(env, ssdt + index_NtReadFile * 4, &NtReadFile);
	qebek_read_ulong(env, ssdt + index_NtWriteFile * 4, &NtWriteFile);
	qemu_printf("NtWriteFile: %x\n", NtWriteFile);

	qebek_service_init = 1;
}