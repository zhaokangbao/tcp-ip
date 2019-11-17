/*
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *	@(#)if.h	8.1 (Berkeley) 6/10/93
 */

/*
 * Structures defining a network interface, providing a packet
 * transport mechanism (ala level 0 of the PUP protocols).
 *
 * Each interface accepts output datagrams of a specified maximum
 * length, and provides higher level routines with input datagrams
 * received from its medium.
 *
 * Output occurs when the routine if_output is called, with three parameters:
 *	(*ifp->if_output)(ifp, m, dst, rt)
 * Here m is the mbuf chain to be sent and dst is the destination address.
 * The output routine encapsulates the supplied datagram if necessary,
 * and then transmits it on its medium.
 *
 * On input, each interface unwraps the data received by it, and either
 * places it on the input queue of a internetwork datagram routine
 * and posts the associated software interrupt, or passes the datagram to a raw
 * packet input routine.
 *
 * Routines exist for locating interfaces by their addresses
 * or for locating a interface on a certain network, as well as more general
 * routing and gateway routines maintaining information used to locate
 * interfaces.  These routines live in the files if.c and route.c
 */

/**
 * @brief 
 * 接口结构ifnet定义头文件
 * 
 */

#ifndef _TIME_ /*  XXX fast fix for SNMP, going away soon */
#include <sys/time.h>
#endif

#ifdef __STDC__
/*
 * Forward structure declarations for function prototypes [sic].
 */
struct	mbuf;
struct	proc;
struct	rtentry;	
struct	socket;
struct	ether_header;
#endif
/*
 * Structure describing information about an interface
 * which may be of interest to management entities.
 */
/*
 * Structure defining a queue for a network interface.
 *
 * (Would like to call this struct ``if'', but C isn't PL/1.)
 */

/**
 * @brief 
 * 系统初始化为每个网络设备分配一个ifnet结构（这里我理解为每个三层接口）
 */
struct ifnet {
	//表示接口的前缀名，例如“loopback”
	char	*if_name;		/* name, e.g. ``en'' or ``lo'' */
	//next用来构建ifnet的单链表，ifnet在函数if_attach中上链
	struct	ifnet *if_next;		/* all struct ifnets are chained */
	//存放地址信息，是一个链表，一个接口可以有多个地址，甚至是不同协议的地址
	struct	ifaddr *if_addrlist;	/* linked list of addresses per if */
	//这两个变量支持BSD分组过滤器(BPF)
	//通过BPF，一个进程能接收由此接口传输或接收的分组的备份
    int	if_pcount;		/* number of promiscuous listeners */
	caddr_t	if_bpf;			/* packet filter structure */
	//每个接口在内核中唯一的标识，在sysctl系统调用以及路由域中使用
	//协议地址并不能唯一标识一个接口，例如几个SLIP连接可以有相同的本地地址
	u_short	if_index;		/* numeric abbreviation for this if  */
	//标识多个相同类型的示例，例如loopback0和loopback1中的0和1
	short	if_unit;		/* sub-unit for lower level driver */
	//以秒为单位记录时间，直到内核为此接口调用if_watchdog为止
	//这个函数用于设备驱动程序定时收集接口统计，或用于复位运行不正确的硬件
	short	if_timer;		/* time 'til if_watchdog called */
	//表明接口的操作状态和属性,具体的flag定义见IFF_UP
	short	if_flags;		/* up/down, broadcast, etc. */
	struct	if_data {
/* generic interface information */
		//指明接口支持的硬件地址类型，在net/if_types.h中定义，IFT_OTHER
		u_char	ifi_type;	/* ethernet, tokenring, etc */
		//数据链路地址的长度
		u_char	ifi_addrlen;	/* media address length */
		//由硬件附加给任何分组的首部的长度
		//例如以太网有长度是6字节的地址和14字节的首部
		u_char	ifi_hdrlen;	/* media header length */
		//接口传输单元的最大值：接口在一次输出操作中能传输的最大数据单元的字节数
		//这是控制网络和传输协议创建分组大小的重要参数，以太网为1500
		u_long	ifi_mtu;	/* maximum transmission unit */
		//通常为0，其他更大的值不利于路由通过此接口
		u_long	ifi_metric;	/* routing metric (external only) */
		//指定接口的传输速率，只有SLIP接口才设置
		u_long	ifi_baudrate;	/* linespeed */
/* volatile statistics */
/*下面这些字段表示的是接口统计数据*/
		u_long	ifi_ipackets;	/* packets received on interface */
		u_long	ifi_ierrors;	/* input errors on interface */
		u_long	ifi_opackets;	/* packets sent on interface */
		u_long	ifi_oerrors;	/* output errors on interface */
		//报文传输被共享媒体上其他传输中断时，该变量加一。（这里不怎么理解）
		u_long	ifi_collisions;	/* collisions on csma interfaces */
		u_long	ifi_ibytes;	/* total number of octets received */
		u_long	ifi_obytes;	/* total number of octets sent */
		u_long	ifi_imcasts;	/* packets received via multicast */
		u_long	ifi_omcasts;	/* packets sent via multicast */
		//该变量仅被SLIP设备驱动程序访问（这个也没有理解意义）
		u_long	ifi_iqdrops;	/* dropped on input, this interface */
		//由于协议不被系统或接口支持的报文数（例如在IP系统中接收到osi报文）
		u_long	ifi_noproto;	/* destined for unsupported protocol */
		//记录任何统计改变的最近时间
		struct	timeval ifi_lastchange;/* last updated */
	}	if_data;
/* procedure handles */
/* 这些是指向标准接口层函数的指针，将设备专用的细节从网络层分离出来 */
	//初始化接口
	int	(*if_init)		/* init routine */
		__P((int));
	//对要传输的输出报文进行排序
	int	(*if_output)		/* output routine (enqueue) */
		__P((struct ifnet *, struct mbuf *, struct sockaddr *,
		     struct rtentry *));
	//启动报文的传输
	int	(*if_start)		/* initiate output routine */
		__P((struct ifnet *));
	//传输完成后的清除，未使用
	int	(*if_done)		/* output complete routine */
		__P((struct ifnet *));	/* (XXX not used; fake prototype) */
	//处理I/O控制命令，这个个人理解，将上层的一些配置通知给设备驱动处理
	int	(*if_ioctl)		/* ioctl routine */
		__P((struct ifnet *, int, caddr_t));
	//复位接口设备
	int	(*if_reset)	
		__P((int));		/* new autoconfig will permit removal */
	//周期性接口例程
	int	(*if_watchdog)		/* timer routine */
		__P((int));
	//接口的输出队列，每个接口都有自己的输出队列,一个mbuf的链表
	struct	ifqueue {
		//指向队列的第一个报文（下一个要输出的分组）
		struct	mbuf *ifq_head;
		//指向队列最后的报文
		struct	mbuf *ifq_tail;
		//当前队列中报文的数目
		int	ifq_len;
		//队列允许缓存的最大个数，默认值为50，来自于全局变量ifqmaxlen=IFQ_MAXLEN
		int	ifq_maxlen;
		//统计因为队列满丢弃的报文
		int	ifq_drops;
	} if_snd;			/* output queue */
};

//重定义了if_data的成员
#define	if_mtu		if_data.ifi_mtu
#define	if_type		if_data.ifi_type
#define	if_addrlen	if_data.ifi_addrlen
#define	if_hdrlen	if_data.ifi_hdrlen
#define	if_metric	if_data.ifi_metric
#define	if_baudrate	if_data.ifi_baudrate
#define	if_ipackets	if_data.ifi_ipackets
#define	if_ierrors	if_data.ifi_ierrors
#define	if_opackets	if_data.ifi_opackets
#define	if_oerrors	if_data.ifi_oerrors
#define	if_collisions	if_data.ifi_collisions
#define	if_ibytes	if_data.ifi_ibytes
#define	if_obytes	if_data.ifi_obytes
#define	if_imcasts	if_data.ifi_imcasts
#define	if_omcasts	if_data.ifi_omcasts
#define	if_iqdrops	if_data.ifi_iqdrops
#define	if_noproto	if_data.ifi_noproto
#define	if_lastchange	if_data.ifi_lastchange

/**
 * @brief 
 * 内核专用的标记使用命令
 * SIOCGIFFLAGS和SIOCSIFFLAGS命令访问
 */
//接口正在工作，(接口处于UP状态，可以使用)
#define	IFF_UP		0x1		/* interface is up */
//接口用于广播网，内核专用，和IFF_POINTOPOINT互斥
#define	IFF_BROADCAST	0x2		/* broadcast address valid */
//这个接口允许调试
#define	IFF_DEBUG	0x4		/* turn on debugging */
//接口用于环回网络
#define	IFF_LOOPBACK	0x8		/* is a loopback net */
//接口用于点对点网络，内核专用，和IFF_BROADCAST互斥
#define	IFF_POINTOPOINT	0x10		/* interface is point-to-point link */
//避免使用尾部封装，(这里理解是通过该接口的报文应当不使用尾部封装)
#define	IFF_NOTRAILERS	0x20		/* avoid use of trailers */
//资源已分配给这个接口，内核专用
#define	IFF_RUNNING	0x40		/* resources allocated */
//在这个接口上不使用ARP协议
#define	IFF_NOARP	0x80		/* no address resolution protocol */
//接口接收所有网络分组，(这个应该是接口混杂模式))
#define	IFF_PROMISC	0x100		/* receive all packets */
//接口正在接收所有多播分组
#define	IFF_ALLMULTI	0x200		/* receive all multicast packets */
//正在传输数据。内核专用
#define	IFF_OACTIVE	0x400		/* transmission in progress */
//接口不能接受它自己发送的数据
#define	IFF_SIMPLEX	0x800		/* can't hear own transmissions */
//这三个标记由设备驱动程序定义，有的接口驱动可修改，有些不可以
#define	IFF_LINK0	0x1000		/* per link layer defined bit */
#define	IFF_LINK1	0x2000		/* per link layer defined bit */
#define	IFF_LINK2	0x4000		/* per link layer defined bit */
//接口支持多播，内核专用
#define	IFF_MULTICAST	0x8000		/* supports multicast */

/* flags set internally only: */
//对所有内核专用的标志进行按位或操作
#define	IFF_CANTCHANGE \
	(IFF_BROADCAST|IFF_POINTOPOINT|IFF_RUNNING|IFF_OACTIVE|\
	    IFF_SIMPLEX|IFF_MULTICAST|IFF_ALLMULTI)

/*
 * Output queues (ifp->if_snd) and internetwork datagram level (pup level 1)
 * input routines have queues of messages stored on ifqueue structures
 * (defined above).  Entries are added to and deleted from these structures
 * by these macros, which should be called with ipl raised to splimp().
 */

//检查接口上的输出队列ifq是否满
#define	IF_QFULL(ifq)		((ifq)->ifq_len >= (ifq)->ifq_maxlen)
//输出队列丢弃报文计数加一
#define	IF_DROP(ifq)		((ifq)->ifq_drops++)
//将报文m插入到输出队列ifq尾部，报文使用mbuf首部中的m_nextpkt连接在一起
#define	IF_ENQUEUE(ifq, m) { \
	(m)->m_nextpkt = 0; \
	if ((ifq)->ifq_tail == 0) \
		(ifq)->ifq_head = m; \
	else \
		(ifq)->ifq_tail->m_nextpkt = m; \
	(ifq)->ifq_tail = m; \
	(ifq)->ifq_len++; \
}
//把报文m插入到队列ifq的前面
#define	IF_PREPEND(ifq, m) { \
	(m)->m_nextpkt = (ifq)->ifq_head; \
	if ((ifq)->ifq_tail == 0) \
		(ifq)->ifq_tail = (m); \
	(ifq)->ifq_head = (m); \
	(ifq)->ifq_len++; \
}
//从队列ifq首部取走一个报文，m返回该报文，无报文返回NULL
#define	IF_DEQUEUE(ifq, m) { \
	(m) = (ifq)->ifq_head; \
	if (m) { \
		if (((ifq)->ifq_head = (m)->m_nextpkt) == 0) \
			(ifq)->ifq_tail = 0; \
		(m)->m_nextpkt = 0; \
		(ifq)->ifq_len--; \
	} \
}

#define	IFQ_MAXLEN	50
#define	IFNET_SLOWHZ	1		/* granularity is 1 second */

/*
 * The ifaddr structure contains information about one address
 * of an interface.  They are maintained by the different address families,
 * are allocated and attached when an address is set, and are linked
 * together so all addresses for an interface can be located.
 */
struct ifaddr {
	//保存一个地址
	struct	sockaddr *ifa_addr;	/* address of interface */
	//这个成员是公用的，表示点对点链路的对端地址或者广播地址，并且互斥
	//对应ifp的flag中的IFF_POINTOPOINT和IFF_BROADCAST
	struct	sockaddr *ifa_dstaddr;	/* other end of p-to-p link */
#define	ifa_broadaddr	ifa_dstaddr	/* broadcast address interface */
	//地址对应的掩码
	struct	sockaddr *ifa_netmask;	/* used to determine subnet */
	//指向地址对应的接口的ifp结构
	struct	ifnet *ifa_ifp;		/* back-pointer to interface */
	//next这个指针，组织一个接口上所有的地址，构建成一个单链表
	struct	ifaddr *ifa_next;	/* next address for interface */
	//这三个解释是支持接口的路由查找，后面看到了补充
	void	(*ifa_rtrequest)();	/* check or clean routes (+ or -)'d */
	u_short	ifa_flags;		/* mostly rt_flags for cloning */
	int	ifa_metric;		/* cost of going out this interface */
	//应该是引用计数，统计对本结构的引用，引用计数为零才能释放结构
	short	ifa_refcnt;		/* extra to malloc for link info */
	//这个在书中无解释，等看到在做注释
#ifdef notdef
	struct	rtentry *ifa_rt;	/* XXXX for ROUTETOIF ????? */
#endif
};
#define	IFA_ROUTE	RTF_UP		/* route installed */

/*
 * Message format for use in obtaining information about interfaces
 * from getkerninfo and the routing socket
 */
struct if_msghdr {
	u_short	ifm_msglen;	/* to skip over non-understood messages */
	u_char	ifm_version;	/* future binary compatability */
	u_char	ifm_type;	/* message type */
	int	ifm_addrs;	/* like rtm_addrs */
	int	ifm_flags;	/* value of if_flags */
	u_short	ifm_index;	/* index for associated ifp */
	struct	if_data ifm_data;/* statistics and other data about if */
};

/*
 * Message format for use in obtaining information about interface addresses
 * from getkerninfo and the routing socket
 */
struct ifa_msghdr {
	u_short	ifam_msglen;	/* to skip over non-understood messages */
	u_char	ifam_version;	/* future binary compatability */
	u_char	ifam_type;	/* message type */
	int	ifam_addrs;	/* like rtm_addrs */
	int	ifam_flags;	/* value of ifa_flags */
	u_short	ifam_index;	/* index for associated ifp */
	int	ifam_metric;	/* value of ifa_metric */
};

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
struct	ifreq {
#define	IFNAMSIZ	16
	char	ifr_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		short	ifru_flags;
		int	ifru_metric;
		caddr_t	ifru_data;
	} ifr_ifru;
#define	ifr_addr	ifr_ifru.ifru_addr	/* address */
#define	ifr_dstaddr	ifr_ifru.ifru_dstaddr	/* other end of p-to-p link */
#define	ifr_broadaddr	ifr_ifru.ifru_broadaddr	/* broadcast address */
#define	ifr_flags	ifr_ifru.ifru_flags	/* flags */
#define	ifr_metric	ifr_ifru.ifru_metric	/* metric */
#define	ifr_data	ifr_ifru.ifru_data	/* for use by interface */
};

struct ifaliasreq {
	char	ifra_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	struct	sockaddr ifra_addr;
	struct	sockaddr ifra_broadaddr;
	struct	sockaddr ifra_mask;
};

/*
 * Structure used in SIOCGIFCONF request.
 * Used to retrieve interface configuration
 * for machine (useful for programs which
 * must know all networks accessible).
 */
struct	ifconf {
	int	ifc_len;		/* size of associated buffer */
	union {
		caddr_t	ifcu_buf;
		struct	ifreq *ifcu_req;
	} ifc_ifcu;
#define	ifc_buf	ifc_ifcu.ifcu_buf	/* buffer address */
#define	ifc_req	ifc_ifcu.ifcu_req	/* array of structures returned */
};

#include <net/if_arp.h>

#ifdef KERNEL
#define	IFAFREE(ifa) \
	if ((ifa)->ifa_refcnt <= 0) \
		ifafree(ifa); \
	else \
		(ifa)->ifa_refcnt--;

struct	ifnet	*ifnet;

void	ether_ifattach __P((struct ifnet *));
void	ether_input __P((struct ifnet *, struct ether_header *, struct mbuf *));
int	ether_output __P((struct ifnet *,
	   struct mbuf *, struct sockaddr *, struct rtentry *));
char	*ether_sprintf __P((u_char *));

void	if_attach __P((struct ifnet *));
void	if_down __P((struct ifnet *));
void	if_qflush __P((struct ifqueue *));
void	if_slowtimo __P((void *));
void	if_up __P((struct ifnet *));
#ifdef vax
void	ifubareset __P((int));
#endif
int	ifconf __P((int, caddr_t));
void	ifinit __P((void));
int	ifioctl __P((struct socket *, int, caddr_t, struct proc *));
int	ifpromisc __P((struct ifnet *, int));
struct	ifnet *ifunit __P((char *));

struct	ifaddr *ifa_ifwithaddr __P((struct sockaddr *));
struct	ifaddr *ifa_ifwithaf __P((int));
struct	ifaddr *ifa_ifwithdstaddr __P((struct sockaddr *));
struct	ifaddr *ifa_ifwithnet __P((struct sockaddr *));
struct	ifaddr *ifa_ifwithroute __P((int, struct sockaddr *,
					struct sockaddr *));
struct	ifaddr *ifaof_ifpforaddr __P((struct sockaddr *, struct ifnet *));
void	ifafree __P((struct ifaddr *));
void	link_rtrequest __P((int, struct rtentry *, struct sockaddr *));

int	loioctl __P((struct ifnet *, int, caddr_t));
void	loopattach __P((int));
int	looutput __P((struct ifnet *,
	   struct mbuf *, struct sockaddr *, struct rtentry *));
void	lortrequest __P((int, struct rtentry *, struct sockaddr *));
#endif
