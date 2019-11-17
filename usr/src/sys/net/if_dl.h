/*
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)if_dl.h	8.1 (Berkeley) 6/10/93
 */

/* 
 * A Link-Level Sockaddr may specify the interface in one of two
 * ways: either by means of a system-provided index number (computed
 * anew and possibly differently on every reboot), or by a human-readable
 * string such as "il0" (for managerial convenience).
 * 
 * Census taking actions, such as something akin to SIOCGCONF would return
 * both the index and the human name.
 * 
 * High volume transactions (such as giving a link-level ``from'' address
 * in a recvfrom or recvmsg call) may be likely only to provide the indexed
 * form, (which requires fewer copy operations and less space).
 * 
 * The form and interpretation  of the link-level address is purely a matter
 * of convention between the device driver and its consumers; however, it is
 * expected that all drivers for an interface of a given if_type will agree.
 */

/**
 * @brief 
 * 链路层结构定义
 * 
 */

/*
 * Structure of a Link-Level sockaddr:
 */
//链路层地址包含接口的一个逻辑地址和一个硬件地址【网络支持】
//arp协议和OSI协议中使用硬件地址
struct sockaddr_dl {
	//指明了整个地址的长度
	u_char	sdl_len;	/* Total length of sockaddr */
	//指明了地址族类，例如AF_LINK
	u_char	sdl_family;	/* AF_DLI */
	//保存接口的索引值，在内核中唯一标识该接口
	u_short	sdl_index;	/* if != 0, system given index for interface */
	//根据ifp中的if_type赋值，表示了接口的协议类型
	u_char	sdl_type;	/* interface type */
	//这个长度表示的是接口名字存在sdl_data中的长度
	u_char	sdl_nlen;	/* interface name length, no trailing 0 reqd. */
	//这个长度表示硬件地址存在sdl_data中的长度
	u_char	sdl_alen;	/* link level address length */
	//Net/3中没有使用
	u_char	sdl_slen;	/* link layer selector length */
	//存储接口名和数据链路地址，先存储链路地址，之后是接口名
	char	sdl_data[12];	/* minimum work area, can be larger;
				   contains both if name and ll address */
};

//指向接口名第一个指针
#define LLADDR(s) ((caddr_t)((s)->sdl_data + (s)->sdl_nlen))

#ifndef KERNEL

#include <sys/cdefs.h>

__BEGIN_DECLS
void	link_addr __P((const char *, struct sockaddr_dl *));
char	*link_ntoa __P((const struct sockaddr_dl *));
__END_DECLS

#endif /* !KERNEL */
