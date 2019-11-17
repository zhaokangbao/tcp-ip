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
 * �ӿڽṹifnet����ͷ�ļ�
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
 * ϵͳ��ʼ��Ϊÿ�������豸����һ��ifnet�ṹ������������Ϊÿ������ӿڣ�
 */
struct ifnet {
	//��ʾ�ӿڵ�ǰ׺�������硰loopback��
	char	*if_name;		/* name, e.g. ``en'' or ``lo'' */
	//next��������ifnet�ĵ�������ifnet�ں���if_attach������
	struct	ifnet *if_next;		/* all struct ifnets are chained */
	//��ŵ�ַ��Ϣ����һ��������һ���ӿڿ����ж����ַ�������ǲ�ͬЭ��ĵ�ַ
	struct	ifaddr *if_addrlist;	/* linked list of addresses per if */
	//����������֧��BSD���������(BPF)
	//ͨ��BPF��һ�������ܽ����ɴ˽ӿڴ������յķ���ı���
    int	if_pcount;		/* number of promiscuous listeners */
	caddr_t	if_bpf;			/* packet filter structure */
	//ÿ���ӿ����ں���Ψһ�ı�ʶ����sysctlϵͳ�����Լ�·������ʹ��
	//Э���ַ������Ψһ��ʶһ���ӿڣ����缸��SLIP���ӿ�������ͬ�ı��ص�ַ
	u_short	if_index;		/* numeric abbreviation for this if  */
	//��ʶ�����ͬ���͵�ʾ��������loopback0��loopback1�е�0��1
	short	if_unit;		/* sub-unit for lower level driver */
	//����Ϊ��λ��¼ʱ�䣬ֱ���ں�Ϊ�˽ӿڵ���if_watchdogΪֹ
	//������������豸��������ʱ�ռ��ӿ�ͳ�ƣ������ڸ�λ���в���ȷ��Ӳ��
	short	if_timer;		/* time 'til if_watchdog called */
	//�����ӿڵĲ���״̬������,�����flag�����IFF_UP
	short	if_flags;		/* up/down, broadcast, etc. */
	struct	if_data {
/* generic interface information */
		//ָ���ӿ�֧�ֵ�Ӳ����ַ���ͣ���net/if_types.h�ж��壬IFT_OTHER
		u_char	ifi_type;	/* ethernet, tokenring, etc */
		//������·��ַ�ĳ���
		u_char	ifi_addrlen;	/* media address length */
		//��Ӳ�����Ӹ��κη�����ײ��ĳ���
		//������̫���г�����6�ֽڵĵ�ַ��14�ֽڵ��ײ�
		u_char	ifi_hdrlen;	/* media header length */
		//�ӿڴ��䵥Ԫ�����ֵ���ӿ���һ������������ܴ����������ݵ�Ԫ���ֽ���
		//���ǿ�������ʹ���Э�鴴�������С����Ҫ��������̫��Ϊ1500
		u_long	ifi_mtu;	/* maximum transmission unit */
		//ͨ��Ϊ0�����������ֵ������·��ͨ���˽ӿ�
		u_long	ifi_metric;	/* routing metric (external only) */
		//ָ���ӿڵĴ������ʣ�ֻ��SLIP�ӿڲ�����
		u_long	ifi_baudrate;	/* linespeed */
/* volatile statistics */
/*������Щ�ֶα�ʾ���ǽӿ�ͳ������*/
		u_long	ifi_ipackets;	/* packets received on interface */
		u_long	ifi_ierrors;	/* input errors on interface */
		u_long	ifi_opackets;	/* packets sent on interface */
		u_long	ifi_oerrors;	/* output errors on interface */
		//���Ĵ��䱻����ý�������������ж�ʱ���ñ�����һ�������ﲻ��ô���⣩
		u_long	ifi_collisions;	/* collisions on csma interfaces */
		u_long	ifi_ibytes;	/* total number of octets received */
		u_long	ifi_obytes;	/* total number of octets sent */
		u_long	ifi_imcasts;	/* packets received via multicast */
		u_long	ifi_omcasts;	/* packets sent via multicast */
		//�ñ�������SLIP�豸����������ʣ����Ҳû���������壩
		u_long	ifi_iqdrops;	/* dropped on input, this interface */
		//����Э�鲻��ϵͳ��ӿ�֧�ֵı�������������IPϵͳ�н��յ�osi���ģ�
		u_long	ifi_noproto;	/* destined for unsupported protocol */
		//��¼�κ�ͳ�Ƹı�����ʱ��
		struct	timeval ifi_lastchange;/* last updated */
	}	if_data;
/* procedure handles */
/* ��Щ��ָ���׼�ӿڲ㺯����ָ�룬���豸ר�õ�ϸ�ڴ������������ */
	//��ʼ���ӿ�
	int	(*if_init)		/* init routine */
		__P((int));
	//��Ҫ�����������Ľ�������
	int	(*if_output)		/* output routine (enqueue) */
		__P((struct ifnet *, struct mbuf *, struct sockaddr *,
		     struct rtentry *));
	//�������ĵĴ���
	int	(*if_start)		/* initiate output routine */
		__P((struct ifnet *));
	//������ɺ�������δʹ��
	int	(*if_done)		/* output complete routine */
		__P((struct ifnet *));	/* (XXX not used; fake prototype) */
	//����I/O�����������������⣬���ϲ��һЩ����֪ͨ���豸��������
	int	(*if_ioctl)		/* ioctl routine */
		__P((struct ifnet *, int, caddr_t));
	//��λ�ӿ��豸
	int	(*if_reset)	
		__P((int));		/* new autoconfig will permit removal */
	//�����Խӿ�����
	int	(*if_watchdog)		/* timer routine */
		__P((int));
	//�ӿڵ�������У�ÿ���ӿڶ����Լ����������,һ��mbuf������
	struct	ifqueue {
		//ָ����еĵ�һ�����ģ���һ��Ҫ����ķ��飩
		struct	mbuf *ifq_head;
		//ָ��������ı���
		struct	mbuf *ifq_tail;
		//��ǰ�����б��ĵ���Ŀ
		int	ifq_len;
		//���������������������Ĭ��ֵΪ50��������ȫ�ֱ���ifqmaxlen=IFQ_MAXLEN
		int	ifq_maxlen;
		//ͳ����Ϊ�����������ı���
		int	ifq_drops;
	} if_snd;			/* output queue */
};

//�ض�����if_data�ĳ�Ա
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
 * �ں�ר�õı��ʹ������
 * SIOCGIFFLAGS��SIOCSIFFLAGS�������
 */
//�ӿ����ڹ�����(�ӿڴ���UP״̬������ʹ��)
#define	IFF_UP		0x1		/* interface is up */
//�ӿ����ڹ㲥�����ں�ר�ã���IFF_POINTOPOINT����
#define	IFF_BROADCAST	0x2		/* broadcast address valid */
//����ӿ���������
#define	IFF_DEBUG	0x4		/* turn on debugging */
//�ӿ����ڻ�������
#define	IFF_LOOPBACK	0x8		/* is a loopback net */
//�ӿ����ڵ�Ե����磬�ں�ר�ã���IFF_BROADCAST����
#define	IFF_POINTOPOINT	0x10		/* interface is point-to-point link */
//����ʹ��β����װ��(����������ͨ���ýӿڵı���Ӧ����ʹ��β����װ)
#define	IFF_NOTRAILERS	0x20		/* avoid use of trailers */
//��Դ�ѷ��������ӿڣ��ں�ר��
#define	IFF_RUNNING	0x40		/* resources allocated */
//������ӿ��ϲ�ʹ��ARPЭ��
#define	IFF_NOARP	0x80		/* no address resolution protocol */
//�ӿڽ�������������飬(���Ӧ���ǽӿڻ���ģʽ))
#define	IFF_PROMISC	0x100		/* receive all packets */
//�ӿ����ڽ������жಥ����
#define	IFF_ALLMULTI	0x200		/* receive all multicast packets */
//���ڴ������ݡ��ں�ר��
#define	IFF_OACTIVE	0x400		/* transmission in progress */
//�ӿڲ��ܽ������Լ����͵�����
#define	IFF_SIMPLEX	0x800		/* can't hear own transmissions */
//������������豸���������壬�еĽӿ��������޸ģ���Щ������
#define	IFF_LINK0	0x1000		/* per link layer defined bit */
#define	IFF_LINK1	0x2000		/* per link layer defined bit */
#define	IFF_LINK2	0x4000		/* per link layer defined bit */
//�ӿ�֧�ֶಥ���ں�ר��
#define	IFF_MULTICAST	0x8000		/* supports multicast */

/* flags set internally only: */
//�������ں�ר�õı�־���а�λ�����
#define	IFF_CANTCHANGE \
	(IFF_BROADCAST|IFF_POINTOPOINT|IFF_RUNNING|IFF_OACTIVE|\
	    IFF_SIMPLEX|IFF_MULTICAST|IFF_ALLMULTI)

/*
 * Output queues (ifp->if_snd) and internetwork datagram level (pup level 1)
 * input routines have queues of messages stored on ifqueue structures
 * (defined above).  Entries are added to and deleted from these structures
 * by these macros, which should be called with ipl raised to splimp().
 */

//���ӿ��ϵ��������ifq�Ƿ���
#define	IF_QFULL(ifq)		((ifq)->ifq_len >= (ifq)->ifq_maxlen)
//������ж������ļ�����һ
#define	IF_DROP(ifq)		((ifq)->ifq_drops++)
//������m���뵽�������ifqβ��������ʹ��mbuf�ײ��е�m_nextpkt������һ��
#define	IF_ENQUEUE(ifq, m) { \
	(m)->m_nextpkt = 0; \
	if ((ifq)->ifq_tail == 0) \
		(ifq)->ifq_head = m; \
	else \
		(ifq)->ifq_tail->m_nextpkt = m; \
	(ifq)->ifq_tail = m; \
	(ifq)->ifq_len++; \
}
//�ѱ���m���뵽����ifq��ǰ��
#define	IF_PREPEND(ifq, m) { \
	(m)->m_nextpkt = (ifq)->ifq_head; \
	if ((ifq)->ifq_tail == 0) \
		(ifq)->ifq_tail = (m); \
	(ifq)->ifq_head = (m); \
	(ifq)->ifq_len++; \
}
//�Ӷ���ifq�ײ�ȡ��һ�����ģ�m���ظñ��ģ��ޱ��ķ���NULL
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
	//����һ����ַ
	struct	sockaddr *ifa_addr;	/* address of interface */
	//�����Ա�ǹ��õģ���ʾ��Ե���·�ĶԶ˵�ַ���߹㲥��ַ�����һ���
	//��Ӧifp��flag�е�IFF_POINTOPOINT��IFF_BROADCAST
	struct	sockaddr *ifa_dstaddr;	/* other end of p-to-p link */
#define	ifa_broadaddr	ifa_dstaddr	/* broadcast address interface */
	//��ַ��Ӧ������
	struct	sockaddr *ifa_netmask;	/* used to determine subnet */
	//ָ���ַ��Ӧ�Ľӿڵ�ifp�ṹ
	struct	ifnet *ifa_ifp;		/* back-pointer to interface */
	//next���ָ�룬��֯һ���ӿ������еĵ�ַ��������һ��������
	struct	ifaddr *ifa_next;	/* next address for interface */
	//������������֧�ֽӿڵ�·�ɲ��ң����濴���˲���
	void	(*ifa_rtrequest)();	/* check or clean routes (+ or -)'d */
	u_short	ifa_flags;		/* mostly rt_flags for cloning */
	int	ifa_metric;		/* cost of going out this interface */
	//Ӧ�������ü�����ͳ�ƶԱ��ṹ�����ã����ü���Ϊ������ͷŽṹ
	short	ifa_refcnt;		/* extra to malloc for link info */
	//����������޽��ͣ��ȿ�������ע��
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