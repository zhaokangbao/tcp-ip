/*-
 * Copyright (c) 1991, 1993
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
 *	@(#)if_slvar.h	8.3 (Berkeley) 2/1/94
 *
 * $Header: if_slvar.h,v 1.3 89/05/31 02:25:18 van Exp $
 */

//依赖标准异步串行设备的SLIP接口在调用cpu_startup时初始化
//当main直接通过SLIP的pdevinit结构中的指针pdev_attach调用slattach时
//SLIP伪设备被初始化【这个伪设备没有理解】

/*
 * Definitions for SLIP interface data structures
 * 
 * (This exists so programs like slstats can get at the definition
 *  of sl_softc.)
 */
//每个SLIP接口有一个该结构【SLIP是一个串行协议】
struct sl_softc {
	struct	ifnet sc_if;		/* network-visible interface */
	//除了ifp中的输出队列，sc_fastq用于要求低时延服务的分组---典型的由交互应用产生
	struct	ifqueue sc_fastq;	/* interactive output queue */
	//指向关联的终端设备
	struct	tty *sc_ttyp;		/* pointer to tty structure */
	//指向下一个接收字节的地址，并在另一个字节到达时向前移动
	u_char	*sc_mp;			/* pointer to next available buf char */
	//指向一个接收SLIP分组的缓存的最后一个字节
	u_char	*sc_ep;			/* pointer to last available buf char */
	//指向一个接收SLIP分组的缓存的第一个字节
	u_char	*sc_buf;		/* input buffer */
	u_int	sc_flags;		/* see below */
	//用于串行线的IP封装机制
	u_int	sc_escape;	/* =1 if last char input was FRAME_ESCAPE */
	long	sc_lasttime;		/* last time a char arrived */
	long	sc_abortcount;		/* number of abort esacpe chars */
	long	sc_starttime;		/* time of first abort in window */
	//保存TCP首部压缩信息
#ifdef INET				/* XXX */
	struct	slcompress sc_comp;	/* tcp compression data */
#endif
	//SLIP设备的BPF信息
	caddr_t	sc_bpf;			/* BPF data */
};

/* internal flags */
//这个标志在sc_flags
//检测到错误，丢弃接收帧
#define	SC_ERROR	0x0001		/* had an input error */

/* visible flags */
//这三个标志在sc_if.if_flags中
//压缩TCP通信【这个没怎么理解什么意思】
#define	SC_COMPRESS	IFF_LINK0	/* compress TCP traffic */
//禁止ICMP通信
#define	SC_NOICMP	IFF_LINK1	/* supress ICMP traffic */
//允许TCP自动压缩
#define	SC_AUTOCOMP	IFF_LINK2	/* auto-enable TCP compression */

#ifdef KERNEL
void	slattach __P((void));
void	slclose __P((struct tty *));
void	slinput __P((int, struct tty *));
int	slioctl __P((struct ifnet *, int, caddr_t));
int	slopen __P((dev_t, struct tty *));
int	sloutput __P((struct ifnet *,
	    struct mbuf *, struct sockaddr *, struct rtentry *));
void	slstart __P((struct tty *));
int	sltioctl __P((struct tty *, int, caddr_t, int));
#endif /* KERNEL */
