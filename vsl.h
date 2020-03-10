/************************************************************
 * JSB Virtual socket library                               *
 *                                                          *
 * Attempt to reconstruct a header file, as original SDKs   *
 * seems to have vanished from the web and weren't archived *
 * by archive.org                                           *
 *                                                          *
 * (c) leecher@dose.0wnz.at                        07/2020  *
 ************************************************************/
 // http://web.archive.org/web/20000521220056/http://www.multiview.jsb.com/support/vsl/vsldocs.html

 // If you have the headers archives somewhere, please drop me a line

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#endif

#define VSLSOCKVERSION  0x101

#define VSL_FIRSTINT   0x66
#define VSL_LASTINT                0x60

#define VSL_MAXSHAREDMEMSIZE       4096

/* General functions */
#define VSL_CMD_ACCEPT             0x01
#define VSL_CMD_BIND               0x02
#define VSL_CMD_CONNECT            0x03
#define VSL_CMD_LISTEN             0x04
#define VSL_CMD_RECV               0x05
#define VSL_CMD_SEND               0x06
#define VSL_CMD_CLOSESOCKET        0x07
#define VSL_CMD_SHUTDOWN           0x08
#define VSL_CMD_SOCKET             0x09
#define VSL_CMD_SELECT             0x0A
#define VSL_CMD_FCNTL              0x0B
#define VSL_CMD_GETSOCKOPT         0x0C
#define VSL_CMD_SETSOCKOPT         0x0D
#define VSL_CMD_RECVFROM           0x0E
#define VSL_CMD_SENDTO             0x0F
#define VSL_CMD_RECVMSG            0x10
#define VSL_CMD_SENDMSG            0x11
#define VSL_CMD_GETPEERNAME        0x12
#define VSL_CMD_GETSOCKNAME        0x13
#define VSL_CMD_IOCTLSOCKET        0x14

/* Database functions */
#define VSL_CMD_GETHOSTBYNAME      0x21
#define VSL_CMD_GETHOSTBYADDR      0x22
#define VSL_CMD_SETHOSTENT         0x23
#define VSL_CMD_ENDHOSTENT         0x24
#define VSL_CMD_GETHOSTNAME        0x25
#define VSL_CMD_SETHOSTNAME        0x26
#define VSL_CMD_GETNETENT          0x27
#define VSL_CMD_GETNETBYNAME       0x28
#define VSL_CMD_GETNETBYADDR       0x29
#define VSL_CMD_SETNETENT          0x2A
#define VSL_CMD_ENDNETENT          0x2B
#define VSL_CMD_GETPROTOENT        0x2C
#define VSL_CMD_GETPROTOBYNAME     0x2D
#define VSL_CMD_GETPROTOBYNUMBER   0x2E
#define VSL_CMD_SETPROTOENT        0x2F
#define VSL_CMD_ENDPROTOENT        0x30
#define VSL_CMD_GETSERVENT         0x31
#define VSL_CMD_GETSERVBYNAME      0x32
#define VSL_CMD_GETSERVBYPORT      0x33
#define VSL_CMD_SETSERVENT         0x34
#define VSL_CMD_ENDSERVENT         0x35
#define VSL_CMD_PGETHOSTBYNAME     0x36
#define VSL_CMD_VSLCALL            0x37
#define VSL_CMD_GETDIAGNOSTICS     0xFD
#define VSL_CMD_GETSHAREDMEMPTR    0xFE
#define VSL_CMD_GETSIGNATURE       0xFF

/* Error codes */
#define VSL_ERR_WOULDBLOCK      35  // Operation would block
#define VSL_ERR_INPROGRESS      36  // Operation now in progress
#define VSL_ERR_ALREADY            37  // Operation already in progress
#define VSL_ERR_NOTSOCK            38  // Socket operation on non-socket
#define VSL_ERR_DESTADDRREQ     39  // Destination address required
#define VSL_ERR_MSGSIZE            40  // Message too long    
#define VSL_ERR_PROTOTYPE       41  // Protocol wrong type for socket
#define VSL_ERR_NOPROTOOPT      42  // Bad protocol option    
#define VSL_ERR_PROTONOSUPPORT  43  // Protocol not supported
#define VSL_ERR_SOCKTNOSUPPORT  44  // Socket type not supported
#define VSL_ERR_OPNOTSUPP       45  // Operation not supported on socket
#define VSL_ERR_PFNOSUPPORT        46  // Protocol family not supported
#define VSL_ERR_AFNOSUPPORT        47  // Addr family not supported by prot family
#define VSL_ERR_ADDRINUSE       48  // Address already in use
#define VSL_ERR_ADDRNOTAVAIL    49  // Can't assign requested address
#define VSL_ERR_NETDOWN            50  // Network is down    
#define VSL_ERR_NETUNREACH      51  // Network is unreachable
#define VSL_ERR_NETRESET        52  // Network dropped connection or reset
#define VSL_ERR_CONNABORTED     53  // Software caused connection abort
#define VSL_ERR_CONNRESET       54  // Connection reset by peer
#define VSL_ERR_NOBUFS          55  // No buffer space available
#define VSL_ERR_ISCONN          56  // Socket is already connected
#define VSL_ERR_NOTCONN            57  // Socket is not connected
#define VSL_ERR_SHUTDOWN        58  // Can't send after socket shutdown
#define VSL_ERR_TOOMANYREFS     59  // Too many references: can't splice

#define VSL_ERR_TIMEDOUT        60  // Connection timed out 
#define VSL_ERR_CONNREFUSED     61  // Connection refused    
#define VSL_ERR_LOOP            62  // Too many levels of symbolic links
#define VSL_ERR_HOSTDOWN        64  // Host is down        
#define VSL_ERR_HOSTUNREACH     65  // Host is unreachable    

#define VSL_ERR_PROCLIM         67  // Too many processes
#define VSL_ERR_USERS           68  // Too many users
#define VSL_ERR_DQUOT           69  // Disc quota exceeded
#define VSL_ERR_STALE           70  // Stale NFS file handle
#define VSL_ERR_REMOTE          71  // Too many levels of remote in path
#define VSL_ERR_NOSTR           72  // Device is not a stream
#define VSL_ERR_TIME            73  // Timer expired
#define VSL_ERR_NOSR            74  // Out of streams resources
#define VSL_ERR_NOMSG           75  // No message of desired type
#define VSL_ERR_BADMSG          76  // Trying to read unreadable message
#define VSL_ERR_IDRM            77  // Identifier removed

#define VSL_ERR_BADVERSION      80  // Library/driver version mismatch
#define VSL_ERR_INVALSOCK       81  // Invalid argument

#define VSL_ERR_TOOMANYSOCK     82  // Too many open sockets
#define VSL_ERR_FAULTSOCK       83  // Bad address in sockets call

#define VSL_ERR_RESET           84  // The socket has reset       
#define VSL_ERR_NOTUNIQUE       85  // Unique parameter required      
#define VSL_ERR_NOGATEADDR      86  // Gateway address required      
#define VSL_ERR_SENDERR         87  // The packet could not be sent   
#define VSL_ERR_NOETHDRVR       88  // No driver or card failed init  
#define VSL_ERR_WRITPENDING     89  // Queued write operation      
#define VSL_ERR_READPENDING     90  // Queued read operation      
#define VSL_ERR_NOTCPIP         91  // TCPIP not loaded          
#define VSL_ERR_DRVBUSY         92  // TCPIP busy              

#define VSL_ERR_UNKNOWN         255 // Unknown native TCP/IP error 

#define VSL_ERR_ENAMETOOLONG    63  // File name too long    
#define VSL_ERR_ENOTEMPTY       66  // Directory not empty    
#define VSL_ERR_DEADLK          78  // Deadlock condition.
#define VSL_ERR_NOLCK           79  // No record locks available.

// #defines for EnumVSLNets() error codes:
#define VSL_ERR_NET_MODULE_NOT_LOADED     -1
#define VSL_ERR_NET_TRANSPORT_NOT_LOADED  -2
#define VSL_ERR_LIBRARY_NOT_INITIALISED   -3
#define VSL_ERR_ALREADY_INITIALISED       -4
#define VSL_ERR_UNKNOWN_NETKEY            -5
#define VSL_ERR_UNSUPPORTED_NET           -6
#define VSL_ERR_UNKNOWN_NET_INTERFACE     -7
#define VSL_ERR_UNKNOWN_NET_MODULE        -8
#define VSL_ERR_UNSUPPORTED_COMMAND       -9

#define VSL_INVALID_SOCKET       -1

#define VSL_FD_SETSIZE           256

#define VSL_FIRSTSOCKET          0
#define VSL_LASTSOCKET           VSL_FD_SETSIZE-1

#define VSL_MAXADDRS             5
#define VSL_ADDRSIZE            20
#define VSL_PROTOSIZE           20
#define VSL_NAMESIZE            80
#define VSL_MAXALIAS             4
#define VSL_ALIASLEN            20
#define VSL_ALIASSIZE           VSL_MAXALIAS * VSL_ALIASLEN


/* Socket constants that may differ between BSD and Linux */
#define VSL_IPPROTO_TCP     6               /* tcp */
#define VSL_SOL_SOCKET      0xffff          /* options for socket level */
#define VSL_TCP_NODELAY     0x0001

/*
 * Option flags per-socket.
 */
#define VSL_SO_DEBUG        0x0001          /* turn on debugging info recording */
#define VSL_SO_ACCEPTCONN   0x0002          /* socket has had listen() */
#define VSL_SO_REUSEADDR    0x0004          /* allow local address reuse */
#define VSL_SO_KEEPALIVE    0x0008          /* keep connections alive */
#define VSL_SO_DONTROUTE    0x0010          /* just use interface addresses */
#define VSL_SO_BROADCAST    0x0020          /* permit sending of broadcast msgs */
#define VSL_SO_DONTLINGER   0x0040
#define VSL_SO_LINGER       0x0080          /* linger on close if data present */
#define VSL_SO_OOBINLINE    0x0100          /* leave received OOB data in line */

/*
 * Additional options.
 */
#define VSL_SO_SNDBUF       0x1001          /* send buffer size */
#define VSL_SO_RCVBUF       0x1002          /* receive buffer size */
#define VSL_SO_SNDLOWAT     0x1003          /* send low-water mark */
#define VSL_SO_RCVLOWAT     0x1004          /* receive low-water mark */
#define VSL_SO_SNDTIMEO     0x1005          /* send timeout */
#define VSL_SO_RCVTIMEO     0x1006          /* receive timeout */
#define VSL_SO_ERROR        0x1007          /* get error status and clear */
#define VSL_SO_TYPE         0x1008          /* get socket type */
#define VSL_SO_USELOOPBACK  0x2000          /* bypass hardware when possible */


#pragma pack(1)
typedef u_short TVSLSocket, *PVSLSockAddr;

typedef struct {
	u_short sa_family;    /* address family */
	char    sa_addr[14];  /* up to 14 bytes of direct address */
} TVSLSockAddr;

// each bit means one socket. bitpos = sockhandle
typedef u_char TVSLFDSET[VSL_FD_SETSIZE/8], *PTVSLFDSET;
     
typedef struct {
	long    tv_sec;       /* seconds */
	long    tv_usec;      /* and microseconds */
} TVSLTimeVal, *PVSLTimeVal;

typedef struct {
	char   *h_name;       /* official name of host */
	char  **h_aliases;    /* alias list */
	short   h_addrtype;   /* host address type */
	short   h_length;     /* length of address */
	char  **h_addr_list;  /* list of addresses */

	char    namebuf[VSL_NAMESIZE+1];
	char   *aliaslist[VSL_MAXALIAS+1];
	char   *addrlist[VSL_MAXADDRS+1];
	char    addrbuf[VSL_ADDRSIZE+1];
	char    aliasbuf[VSL_ALIASSIZE+1];
} TVSLHostEnt;

typedef struct {
    char   *p_name;
    char  **p_aliases;
    short   p_proto;

	char    namebuf[VSL_NAMESIZE+1];
	char   *aliaslist[VSL_MAXALIAS+1];
	char    aliasbuf[VSL_ALIASSIZE+1];
} TVSLProtEnt;

typedef struct {
    char   *s_name;
    char  **s_aliases;
    short   s_port;
    char   *s_proto;

	char    namebuf[VSL_NAMESIZE+1];
	char   *aliaslist[VSL_MAXALIAS+1];
	char    aliasbuf[VSL_ALIASSIZE+1];
	char    protobuf[VSL_PROTOSIZE+1];
} TVSLServEnt;
 

typedef struct {
	short RetCode;
	short ErrNo;
} TVSL_Params_Header;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
	u_short            AddrLen;
	TVSLSockAddr       Addr;
} TVSL_Accept_Params, TVSL_Bind_Params, TVSL_Connect_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
	u_short            BackLog;
} TVSL_Listen_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
	u_short            Len;
	u_short            Flags;
	u_char             Buffer[1];
} TVSL_Recv_Params, TVSL_Send_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_short            nfds;
	TVSLFDSET          readfds;
	TVSLFDSET          writefds;
	TVSLFDSET          exceptfds;
	TVSLTimeVal        timeout;
} TVSL_Select_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_short            Domain;
	u_short            SType;
	u_short            Protocol;
} TVSL_Socket_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
} TVSL_CloseSocket_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
	u_short            How;
} TVSL_Shutdown_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
    u_short            Cmd;
	u_short            Arg;
} TVSL_FCntl_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
    u_short            Cmd;
    u_char             Buffer[1];
} TVSL_IOCTL_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
	u_short            Level;
	u_short            OptName;
	u_short            OptLen;
    u_char             Buffer[1];
} TVSL_GetSockOpt_Params, TVSL_SetSockOpt_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
	u_short            Len;
	u_short            Flags;
	u_short            AddrLen;
	TVSLSockAddr       Addr;
	u_char             Buffer[1];
} TVSL_RecvFrom_Params, TVSL_SendTo_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
    void              *Msg;
	u_short            Flags;
} TVSL_RecvMsg_Params, TVSL_SendMsg_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLSocket         Socket;
	u_short            NameLen;
	TVSLSockAddr       Name;
} TVSL_GetSockName_Params, TVSL_GetPeerName_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_short            NameLen;
	u_char             Buffer[1];
} TVSL_GetHostName_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLProtEnt        Buffer;
} TVSL_GetProtByName_Params;

typedef struct {
	TVSL_Params_Header Header;
	TVSLHostEnt        Buffer;
} TVSL_GetHostByName_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_short            Len;
	u_short            AType;
	TVSLHostEnt        Buffer;
} TVSL_GetHostByAddr_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_short            Number;
	TVSLProtEnt        Buffer;
} TVSL_GetProtByNumber_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_char            *Name;
	TVSLServEnt        Buffer;
} TVSL_GetServByName_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_short            Port;
	TVSLServEnt        Buffer;
} TVSL_GetServByPort_Params;

typedef struct {
	TVSL_Params_Header Header;
	u_short            Command;
	u_short            InLen;
	u_short            OutLen;
	u_short            RunningInPM;
	u_char             Buffer[1];
} TVSL_VSLCall_Params;

#pragma pack()
