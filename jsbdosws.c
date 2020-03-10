/*
 * JSB Virtual socket library virtual device driver
 * Core engine module
 *
 * leecher@dose.0wnz.at   02/2020
 *
 * https://github.com/leecher1337/jsbdosws
 */
#include "vsl.h"
#include "jsbdosws.h"

#ifdef WIN32
#include "vslfunc_win32.h"
#else
#include "vslfunc_linux.h"
#endif

u_short wSharedMemSize = 0;
u_long  X86Address = 0;
u_char *pLinearPointer = 0;


/* Prototypes utility functions*/
#define MvwLen strlen
void MvwMove(char *dest, char *src, int len);

void SetGeneralReturnAndErrorValues(TVSL_Params_Header *pHdr, int RetCode, 
									int ErrNo, int ExpectedRet, BOOL bError);
void SetDatabaseReturnAndErrorValues(TVSL_Params_Header *pHdr, int RetCode, 
									int ErrNo, BOOL bError);
void OurCopyHostent(TVSLHostEnt *dest, struct hostent *src);
void OurCopyProtoent(TVSLProtEnt *dest, struct protoent *src);
void OurCopyServent(TVSLServEnt *dest, struct servent *src);

/* Prototypes dispatch functions */
void jsbdoswsAccept(TVSL_Accept_Params *p);
void jsbdoswsBind(TVSL_Bind_Params *p);
void jsbdoswsConnect(TVSL_Connect_Params *p);
void jsbdoswsListen(TVSL_Listen_Params *p);
void jsbdoswsRecv(TVSL_Recv_Params *p);
void jsbdoswsSend(TVSL_Send_Params *p);
void jsbdoswsClose(TVSL_CloseSocket_Params *p);
void jsbdoswsShutdown(TVSL_Shutdown_Params *p);
void jsbdoswsSocket(TVSL_Socket_Params *p);
void jsbdoswsSelect(TVSL_Select_Params *p);
void jsbdoswsIoctlsocket(TVSL_IOCTL_Params *p);
void jsbdoswsGetsockopt(TVSL_GetSockOpt_Params *p);
void jsbdoswsSetsockopt(TVSL_SetSockOpt_Params *p);
void jsbdoswsRecvfrom(TVSL_RecvFrom_Params *p);
void jsbdoswsSendto(TVSL_SendTo_Params *p);
void jsbdoswsGetpeername(TVSL_GetPeerName_Params *p);
void jsbdoswsGetsockname(TVSL_GetSockName_Params *p);
void jsbdoswsGethostbyaddr(TVSL_GetHostByAddr_Params *p);
void jsbdoswsGethostname(TVSL_GetHostByName_Params *p);
void jsbdoswsGetprotobyname(TVSL_GetProtByName_Params *p);
void jsbdoswsGetprotobynumber(TVSL_GetProtByNumber_Params *p);
void jsbdoswsGetservbyname(TVSL_GetServByName_Params *p);
void jsbdoswsGetservbyport(TVSL_GetServByPort_Params *p);
void jsbdoswsGethostbyname(TVSL_GetHostByName_Params *p);

/*------------------------------------------------------*
 * Implementation                                       *
 *------------------------------------------------------*/

void MvwMove(char *dest, char *src, int len)
{
	if (len) memmove(dest, src, len); else strcpy(dest, src);
}

void SetGeneralReturnAndErrorValues(TVSL_Params_Header *pHdr, int RetCode, 
									int ErrNo, int ExpectedRet, BOOL bError)
{
	if ( bError || ExpectedRet != RetCode )
		pHdr->ErrNo = ErrNo;
	else if (WSAGetLastError())
		pHdr->ErrNo = WSAGetLastError() - WSABASEERR;
	else
		pHdr->ErrNo = -1;
	pHdr->RetCode = RetCode;
}

void SetDatabaseReturnAndErrorValues(TVSL_Params_Header *pHdr, int RetCode, 
									int ErrNo, BOOL bError)
{
	if (bError)
		pHdr->ErrNo = ErrNo;
	else if ((pHdr->ErrNo = WSAGetLastError()))
	{
#ifdef WIN32
		if (pHdr->ErrNo < WSAHOST_NOT_FOUND)
		{
			if (pHdr->ErrNo < WSABASEERR) pHdr->ErrNo = -1;
			else pHdr->ErrNo -= WSABASEERR;
		}
		else
			pHdr->ErrNo -= (WSAHOST_NOT_FOUND-1);
#endif
	}
	else
		pHdr->ErrNo = -1;
	pHdr->RetCode = RetCode;
}


void OurCopyHostent(TVSLHostEnt *dest, struct hostent *src)
{
	int len, i, addr_offs, cplen;

	memset(dest, 0, sizeof(TVSLHostEnt));

	if ((len = MvwLen(src->h_name))>=VSL_NAMESIZE)
	{
		dest->namebuf[VSL_NAMESIZE] = 0;
		len = VSL_NAMESIZE;
	}
	MvwMove(dest->namebuf, src->h_name, len);

	for (i=0, addr_offs=0; src->h_aliases[i] && i<VSL_MAXALIAS; i++)
	{
		len = MvwLen(src->h_aliases[i]) + 1;
		if (addr_offs + len <= VSL_NAMESIZE) cplen = 0;
		else cplen = len = VSL_NAMESIZE - addr_offs;
		MvwMove(&dest->aliasbuf[addr_offs], src->h_aliases[i], cplen);
		addr_offs += len;
		if (cplen)
		{
			dest->aliasbuf[addr_offs-1]=0;
			break;
		}
	}
	dest->aliasbuf[addr_offs]=0;

	for (i=0; src->h_addr_list[i] && i<VSL_MAXADDRS; i++)
		MvwMove((char*)&((u_long*)dest->addrbuf)[i], src->h_addr_list[i], sizeof(u_long));
}

void OurCopyProtoent(TVSLProtEnt *dest, struct protoent *src)
{
	int len, i, addr_offs, cplen;

	memset(dest, 0, sizeof(TVSLProtEnt));

	if ((len = MvwLen(src->p_name))>=VSL_NAMESIZE)
	{
		dest->namebuf[VSL_NAMESIZE] = 0;
		len = VSL_NAMESIZE;
	}
	MvwMove(dest->namebuf, src->p_name, len);

	for (i=0, addr_offs=0; src->p_aliases[i] && i<VSL_MAXALIAS; i++)
	{
		len = MvwLen(src->p_aliases[i]) + 1;
		if (addr_offs + len <= VSL_NAMESIZE) cplen = 0;
		else cplen = len = VSL_NAMESIZE - addr_offs;
		MvwMove(&dest->aliasbuf[addr_offs], src->p_aliases[i], cplen);
		addr_offs += len;
		if (cplen)
		{
			dest->aliasbuf[addr_offs-1]=0;
			break;
		}
	}
	dest->aliasbuf[addr_offs]=0;
}

void OurCopyServent(TVSLServEnt *dest, struct servent *src)
{
	int len, i, addr_offs, cplen;

	memset(dest, 0, sizeof(TVSLProtEnt));
	dest->s_port = src->s_port;

	if ((len = MvwLen(src->s_name))>=VSL_NAMESIZE)
	{
		dest->namebuf[VSL_NAMESIZE] = 0;
		len = VSL_NAMESIZE;
	}
	MvwMove(dest->namebuf, src->s_name, len);

	if ((len = MvwLen(src->s_proto))>=VSL_NAMESIZE)
	{
		dest->protobuf[VSL_NAMESIZE] = 0;
		len = VSL_NAMESIZE;
	}
	MvwMove(dest->protobuf, src->s_proto, len);

	for (i=0, addr_offs=0; src->s_aliases[i] && i<VSL_MAXALIAS; i++)
	{
		len = MvwLen(src->s_aliases[i]) + 1;
		if (addr_offs + len <= VSL_NAMESIZE) cplen = 0;
		else cplen = len = VSL_NAMESIZE - addr_offs;
		MvwMove(&dest->aliasbuf[addr_offs], src->s_aliases[i], cplen);
		addr_offs += len;
		if (cplen)
		{
			dest->aliasbuf[addr_offs-1]=0;
			break;
		}
	}
	dest->aliasbuf[addr_offs]=0;
}

// ---------------------------------------------------------------------------

void jsbdoswsAccept(TVSL_Accept_Params *p)
{
	SOCKET s = p->Socket;
	int i, AddrLen;

	VALIDATE_SOCKET
	AddrLen = p->AddrLen;
	if ((s = accept(s, (struct sockaddr *)&p->Addr, &AddrLen)) != INVALID_SOCKET)
	{
#ifdef WIN32
		for (i=0; i<VSL_FD_SETSIZE; i++)
			if (SocketArray[i] == INVALID_SOCKET) break;
		if (i==VSL_FD_SETSIZE)
		{
			closesocket(s);
			SetGeneralReturnAndErrorValues(&p->Header, VSL_INVALID_SOCKET, VSL_ERR_NOBUFS, -1, TRUE);
			return;
		}
		SocketArray[i] = s;
#endif
		p->AddrLen = AddrLen;
	} else i = -1;
	SetGeneralReturnAndErrorValues(&p->Header, i, 0, -1, FALSE);
}

void jsbdoswsBind(TVSL_Bind_Params *p)
{
	SOCKET s = p->Socket;

	VALIDATE_SOCKET
	SetGeneralReturnAndErrorValues(&p->Header, bind(s, (struct sockaddr *)&p->Addr, p->AddrLen), 0, -1, FALSE);
}

void jsbdoswsConnect(TVSL_Connect_Params *p)
{
	SOCKET s = p->Socket;

	VALIDATE_SOCKET
	SetGeneralReturnAndErrorValues(&p->Header, connect(s, (struct sockaddr *)&p->Addr, p->AddrLen), 0, -1, FALSE);
}

void jsbdoswsListen(TVSL_Listen_Params *p)
{
	SOCKET s = p->Socket;

	VALIDATE_SOCKET
	SetGeneralReturnAndErrorValues(&p->Header, listen(s, p->BackLog), 0, -1, FALSE);
}

void jsbdoswsRecv(TVSL_Recv_Params *p)
{
	SOCKET s = p->Socket;

	VALIDATE_SOCKET
	SetGeneralReturnAndErrorValues(&p->Header, recv(s, p->Buffer, p->Len, p->Flags), 0, -1, FALSE);
}

void jsbdoswsSend(TVSL_Send_Params *p)
{
	SOCKET s = p->Socket;

	VALIDATE_SOCKET
	SetGeneralReturnAndErrorValues(&p->Header, send(s, p->Buffer, p->Len, p->Flags), 0, -1, FALSE);
}

void jsbdoswsClose(TVSL_CloseSocket_Params *p)
{
	SOCKET s = p->Socket;
	int RetCode;

	VALIDATE_SOCKET
	RetCode = closesocket(s);
#ifdef WIN32
	SocketArray[p->Socket] = INVALID_SOCKET;
#endif
	SetGeneralReturnAndErrorValues(&p->Header, RetCode, 0, -1, FALSE);
}

void jsbdoswsShutdown(TVSL_Shutdown_Params *p)
{
	SOCKET s = p->Socket;

	VALIDATE_SOCKET
	SetGeneralReturnAndErrorValues(&p->Header, shutdown(s, p->How), 0, -1, FALSE);
}

void jsbdoswsSocket(TVSL_Socket_Params *p)
{
	SOCKET s;
	int i;

	if ((s=socket(p->Domain, p->SType, p->Protocol)) != INVALID_SOCKET)
	{
#ifdef WIN32
		for (i=0; i<VSL_FD_SETSIZE; i++)
			if (SocketArray[i] == INVALID_SOCKET) break;
		if (i==VSL_FD_SETSIZE)
		{
			closesocket(s);
			SetGeneralReturnAndErrorValues(&p->Header, VSL_INVALID_SOCKET, VSL_ERR_NOBUFS, -1, TRUE);
			return;
		}
		SocketArray[i] = s;
#endif
	} else i = -1;
	SetGeneralReturnAndErrorValues(&p->Header, i, 0, -1, FALSE);
}

void jsbdoswsSelect(TVSL_Select_Params *p)
{
	int nfds = 0;

#ifdef WIN32
	if (p->nfds)
	{
		struct fd_set readfds, writefds, exceptfds;
		
		if (setSelectInputFds(p->nfds, p->readfds, &readfds) &&
			setSelectInputFds(p->nfds, p->writefds, &writefds) &&
			setSelectInputFds(p->nfds, p->exceptfds, &exceptfds))
		{
			if (readfds.fd_count   > 0 || 
				writefds.fd_count  > 0 || 
				exceptfds.fd_count > 0)
			{
				struct timeval tv;

				tv.tv_sec = p->timeout.tv_sec;
				tv.tv_usec = p->timeout.tv_usec;
				if ((nfds = select(p->nfds, &readfds, &writefds, &exceptfds, &tv)) > 0)
				{
					memset(p->readfds, 0, sizeof(p->readfds));
					memset(p->writefds, 0, sizeof(p->writefds));
					memset(p->exceptfds, 0, sizeof(p->exceptfds));
					setSelectOutputFds(nfds, p->readfds, &readfds);
					setSelectOutputFds(nfds, p->writefds, &writefds);
					setSelectOutputFds(nfds, p->exceptfds, &exceptfds);
				}
			}
		}
		else
		{
			SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_NOTSOCK, -1, TRUE);
			return;
		}
	}
#else
	nfds = select(p->nfds, (fd_set*)&p->readfds, (fd_set*)&p->writefds, 
		(fd_set*)&p->exceptfds, (struct timeval *)&p->timeout);
#endif
	SetGeneralReturnAndErrorValues(&p->Header, nfds, 0, -1, FALSE);
}

void jsbdoswsIoctlsocket(TVSL_IOCTL_Params *p)
{
	SOCKET s = p->Socket;
	u_long opt;

	VALIDATE_SOCKET
	if (p->Cmd != 4 || *(u_short*)p->Buffer != 4)
	{
		SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
		return;
	}
	opt = 1;
	SetGeneralReturnAndErrorValues(&p->Header, ioctlsocket(s, FIONBIO, &opt), 0, -1, FALSE);
}

void jsbdoswsGetsockopt(TVSL_GetSockOpt_Params *p)
{
	SOCKET s = p->Socket;
	int Level, OptName, RetCode, OptLen = 4;

	VALIDATE_SOCKET
	if (p->Level == VSL_IPPROTO_TCP)
	{
		Level = IPPROTO_TCP;
		if (p->OptName != VSL_TCP_NODELAY)
		{
			SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
			return;
		}
		OptName = TCP_NODELAY;
	}
	else if (p->Level == VSL_SOL_SOCKET)
	{
		Level = SOL_SOCKET;
		switch (p->OptName)
		{
		case VSL_SO_SNDBUF:
			OptName = SO_SNDBUF;
			break;
		case VSL_SO_RCVBUF:
			OptName = SO_RCVBUF;
			break;
		case VSL_SO_SNDLOWAT:
			OptName = SO_SNDLOWAT;
			break;
		case VSL_SO_RCVLOWAT:
			OptName = SO_RCVLOWAT;
			break;
		case VSL_SO_SNDTIMEO:
			OptName = SO_SNDTIMEO;
			break;
		case VSL_SO_RCVTIMEO:
			OptName = SO_RCVTIMEO;
			break;
		case VSL_SO_ERROR:
			OptName = SO_ERROR;
			break;
		case VSL_SO_TYPE:
			OptName = SO_TYPE;
			break;
#ifdef SO_USELOOPBACK
		case VSL_SO_USELOOPBACK:
			OptName = SO_USELOOPBACK;
			break;
#endif
		case VSL_SO_OOBINLINE:
			OptName = SO_OOBINLINE;
			break;
		case VSL_SO_LINGER:
			OptName = SO_LINGER;
			break;
		case VSL_SO_DONTLINGER:
			OptName = SO_DONTLINGER;
			break;
		case VSL_SO_BROADCAST:
			OptName = SO_BROADCAST;
			break;
		case VSL_SO_DONTROUTE:
			OptName = SO_DONTROUTE;
			break;
		case VSL_SO_KEEPALIVE:
			OptName = SO_KEEPALIVE;
			break;
		case VSL_SO_REUSEADDR:
			OptName = SO_REUSEADDR;
			break;
		case VSL_SO_ACCEPTCONN:
			OptName = SO_ACCEPTCONN;
			break;
		case VSL_SO_DEBUG:
			OptName = SO_DEBUG;
			break;
		default:
			SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
			return;
		}
	}
	else
	{
		SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
		return;
	}

	if ((RetCode = getsockopt(s, Level, OptName, p->Buffer, &OptLen)) != -1)
	{
		switch (OptName)
		{
		case VSL_SO_DEBUG:
		case VSL_SO_ACCEPTCONN:
		case VSL_SO_REUSEADDR:
		case VSL_SO_BROADCAST:
		case VSL_SO_DONTLINGER:
		case VSL_SO_OOBINLINE:
		case VSL_SO_DONTROUTE:
		case VSL_SO_KEEPALIVE:
		case VSL_SO_USELOOPBACK:
			p->OptLen = 2;
			*(u_short*)p->Buffer = (*(u_long*)p->Buffer)?1:0;
			break;
		case VSL_SO_LINGER:
			p->OptLen = 4;
			break;
		case VSL_SO_SNDBUF:
		case VSL_SO_RCVBUF:
		case VSL_SO_SNDLOWAT:
		case VSL_SO_RCVLOWAT:
		case VSL_SO_SNDTIMEO:
		case VSL_SO_RCVTIMEO:
		case VSL_SO_ERROR:
		case VSL_SO_TYPE:
			p->OptLen = 2;
			break;
		default:
			SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
			return;
		}
	}
	SetGeneralReturnAndErrorValues(&p->Header, RetCode, 0, -1, FALSE);
}

void jsbdoswsSetsockopt(TVSL_SetSockOpt_Params *p)
{
	SOCKET s = p->Socket;
	int Level, OptName, OptLen = 4;

	VALIDATE_SOCKET
	if (p->Level == VSL_IPPROTO_TCP)
	{
		Level = IPPROTO_TCP;
		if (p->OptName != VSL_TCP_NODELAY)
		{
			SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
			return;
		}
		OptName = TCP_NODELAY;
	}
	else if (p->Level == VSL_SOL_SOCKET)
	{
		Level = SOL_SOCKET;
		switch (p->OptName)
		{
		case VSL_SO_SNDBUF:
			OptName = SO_SNDBUF;
			break;
		case VSL_SO_RCVBUF:
			OptName = SO_RCVBUF;
			break;
		case VSL_SO_SNDLOWAT:
			OptName = SO_SNDLOWAT;
			break;
		case VSL_SO_RCVLOWAT:
			OptName = SO_RCVLOWAT;
			break;
		case VSL_SO_SNDTIMEO:
			OptName = SO_SNDTIMEO;
			break;
		case VSL_SO_RCVTIMEO:
			OptName = SO_RCVTIMEO;
			break;
		case VSL_SO_ERROR:
			OptName = SO_ERROR;
			break;
		case VSL_SO_TYPE:
			OptName = SO_TYPE;
			break;
#ifdef SO_USELOOPBACK
		case VSL_SO_USELOOPBACK:
			OptName = SO_USELOOPBACK;
			break;
#endif
		case VSL_SO_OOBINLINE:
			OptName = SO_OOBINLINE;
			break;
		case VSL_SO_LINGER:
			OptName = SO_LINGER;
			break;
		case VSL_SO_DONTLINGER:
			OptName = SO_DONTLINGER;
			break;
		case VSL_SO_BROADCAST:
			OptName = SO_BROADCAST;
			break;
		case VSL_SO_DONTROUTE:
			OptName = SO_DONTROUTE;
			break;
		case VSL_SO_KEEPALIVE:
			OptName = SO_KEEPALIVE;
			break;
		case VSL_SO_REUSEADDR:
			OptName = SO_REUSEADDR;
			break;
		case VSL_SO_ACCEPTCONN:
			OptName = SO_ACCEPTCONN;
			break;
		case VSL_SO_DEBUG:
			OptName = SO_DEBUG;
			break;
		default:
			SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
			return;
		}
	}
	else
	{
		SetGeneralReturnAndErrorValues(&p->Header, -1, VSL_ERR_OPNOTSUPP, -1, TRUE);
		return;
	}

	SetGeneralReturnAndErrorValues(&p->Header, setsockopt(s, Level, OptName, p->Buffer, OptLen), 
		0, -1, FALSE);
}

void jsbdoswsRecvfrom(TVSL_RecvFrom_Params *p)
{
	SOCKET s = p->Socket;
	int AddrLen;

	VALIDATE_SOCKET
	AddrLen = p->AddrLen;
	SetGeneralReturnAndErrorValues(&p->Header, 
		recvfrom(s, p->Buffer, p->Len, p->Flags, (struct sockaddr *)&p->Addr, &AddrLen), 0, -1, FALSE);
}

void jsbdoswsSendto(TVSL_SendTo_Params *p)
{
	SOCKET s = p->Socket;

	VALIDATE_SOCKET
	SetGeneralReturnAndErrorValues(&p->Header, 
		sendto(s, p->Buffer, p->Len, p->Flags, (struct sockaddr *)&p->Addr, p->AddrLen), 0, -1, FALSE);
}

void jsbdoswsGetpeername(TVSL_GetPeerName_Params *p)
{
	SOCKET s = p->Socket;
	int NameLen;

	VALIDATE_SOCKET
	NameLen = p->NameLen;
	SetGeneralReturnAndErrorValues(&p->Header, getpeername(s, (struct sockaddr*)&p->Name, &NameLen), 0, -1, FALSE);
}

void jsbdoswsGetsockname(TVSL_GetSockName_Params *p)
{
	SOCKET s = p->Socket;
	int NameLen;

	VALIDATE_SOCKET
	NameLen = p->NameLen;
	SetGeneralReturnAndErrorValues(&p->Header, getsockname(s, (struct sockaddr*)&p->Name, &NameLen), 0, -1, FALSE);
}

void jsbdoswsGethostbyaddr(TVSL_GetHostByAddr_Params *p)
{
	struct hostent *he;

	if ((he = gethostbyaddr((const char*)&p->Buffer, 4, AF_INET)))
	{
		OurCopyHostent(&p->Buffer, he);
		SetDatabaseReturnAndErrorValues(&p->Header, 0, 0, TRUE);
		return;
	}
	SetDatabaseReturnAndErrorValues(&p->Header, -1, 0, FALSE);
}

void jsbdoswsGethostname(TVSL_GetHostByName_Params *p)
{
	int len, ret;

	if ((len = *(short*)p->Buffer.h_name) <= 0)
	{
		SetDatabaseReturnAndErrorValues(&p->Header, -1, 14, TRUE);
		return;
	}
	if (len > wSharedMemSize - 7) len = wSharedMemSize - 7;
	ret = gethostname((char *)&p->Buffer.h_name + 2, len);
	SetDatabaseReturnAndErrorValues(&p->Header, ret?-1:ret, 0, ret?0:1);
}


void jsbdoswsGetprotobyname(TVSL_GetProtByName_Params *p)
{
	struct protoent *pe;

	if ((pe = getprotobyname((const char*)&p->Buffer)))
	{
		OurCopyProtoent(&p->Buffer, pe);
		SetDatabaseReturnAndErrorValues(&p->Header, 0, 0, TRUE);
		return;
	}
	SetDatabaseReturnAndErrorValues(&p->Header, -1, 0, FALSE);
}

void jsbdoswsGetprotobynumber(TVSL_GetProtByNumber_Params *p)
{
	struct protoent *pe;

	if ((pe = getprotobynumber(p->Number)))
	{
		OurCopyProtoent(&p->Buffer, pe);
		SetDatabaseReturnAndErrorValues(&p->Header, 0, 0, TRUE);
		return;
	}
	SetDatabaseReturnAndErrorValues(&p->Header, -1, 0, FALSE);
}

void jsbdoswsGetservbyname(TVSL_GetServByName_Params *p)
{
	char *pName;
	struct servent *se;

	((char*)p)[wSharedMemSize - 1] = 0;
	if ((pName = (char*)&p->Buffer.s_name[MvwLen(p->Buffer.s_name)+1]) > &((char*)p)[wSharedMemSize - 1])
		pName = NULL;
	if ((se = getservbyname(pName, (const char*)&p->Buffer)))
	{
		OurCopyServent(&p->Buffer, se);
		SetDatabaseReturnAndErrorValues(&p->Header, 0, 0, TRUE);
		return;
	}
	SetDatabaseReturnAndErrorValues(&p->Header, -1, 0, FALSE);
}

void jsbdoswsGetservbyport(TVSL_GetServByPort_Params *p)
{
	struct servent *se;

	((char*)p)[wSharedMemSize - 1] = 0;
	if ((se = getservbyport(p->Port, (const char*)&p->Buffer)))
	{
		OurCopyServent(&p->Buffer, se);
		SetDatabaseReturnAndErrorValues(&p->Header, 0, 0, TRUE);
		return;
	}
	SetDatabaseReturnAndErrorValues(&p->Header, -1, 0, FALSE);
}

void jsbdoswsGethostbyname(TVSL_GetHostByName_Params *p)
{
	struct hostent *he;

	((char*)p)[wSharedMemSize - 1] = 0;
	if ((he = gethostbyname((const char*)&p->Buffer)))
	{
		OurCopyHostent(&p->Buffer, he);
		SetDatabaseReturnAndErrorValues(&p->Header, 0, 0, TRUE);
		return;
	}
	SetDatabaseReturnAndErrorValues(&p->Header, -1, 0, FALSE);
}


void jsbdosws_Dispatch(unsigned short func)
{
	switch (func)
	{
	case VSL_CMD_ACCEPT:
		jsbdoswsAccept((TVSL_Accept_Params *)pLinearPointer);
		break;
	case VSL_CMD_BIND:
		jsbdoswsBind((TVSL_Bind_Params *)pLinearPointer);
		break;
	case VSL_CMD_CONNECT:
		jsbdoswsConnect((TVSL_Connect_Params *)pLinearPointer);
		break;
	case VSL_CMD_LISTEN:
		jsbdoswsListen((TVSL_Listen_Params *)pLinearPointer);
		break;
	case VSL_CMD_RECV:
		jsbdoswsRecv((TVSL_Recv_Params *)pLinearPointer);
		break;
	case VSL_CMD_SEND:
		jsbdoswsSend((TVSL_Send_Params *)pLinearPointer);
		break;
	case VSL_CMD_CLOSESOCKET:
		jsbdoswsClose((TVSL_CloseSocket_Params *)pLinearPointer);
		break;
	case VSL_CMD_SHUTDOWN:
		jsbdoswsShutdown((TVSL_Shutdown_Params *)pLinearPointer);
		break;
	case VSL_CMD_SOCKET:
		jsbdoswsSocket((TVSL_Socket_Params *)pLinearPointer);
		break;
	case VSL_CMD_SELECT:
		jsbdoswsSelect((TVSL_Select_Params *)pLinearPointer);
		break;
	case VSL_CMD_FCNTL:
		jsbdoswsIoctlsocket((TVSL_IOCTL_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETSOCKOPT:
		jsbdoswsGetsockopt((TVSL_GetSockOpt_Params *)pLinearPointer);
		break;
	case VSL_CMD_SETSOCKOPT:
		jsbdoswsSetsockopt((TVSL_SetSockOpt_Params *)pLinearPointer);
		break;
	case VSL_CMD_RECVFROM:
		jsbdoswsRecvfrom((TVSL_RecvFrom_Params *)pLinearPointer);
		break;
	case VSL_CMD_SENDTO:
		jsbdoswsSendto((TVSL_SendTo_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETPEERNAME:
		jsbdoswsGetpeername((TVSL_GetPeerName_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETSOCKNAME:
		jsbdoswsGetsockname((TVSL_GetSockName_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETHOSTBYADDR:
		jsbdoswsGethostbyaddr((TVSL_GetHostByAddr_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETHOSTNAME:
		jsbdoswsGethostname((TVSL_GetHostByName_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETPROTOBYNAME:
		jsbdoswsGetprotobyname((TVSL_GetProtByName_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETPROTOBYNUMBER:
		jsbdoswsGetprotobynumber((TVSL_GetProtByNumber_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETSERVBYNAME:
		jsbdoswsGetservbyname((TVSL_GetServByName_Params *)pLinearPointer);
		break;
	case VSL_CMD_GETSERVBYPORT:
		jsbdoswsGetservbyport((TVSL_GetServByPort_Params *)pLinearPointer);
		break;
	case VSL_CMD_PGETHOSTBYNAME:
		jsbdoswsGethostbyname((TVSL_GetHostByName_Params *)pLinearPointer);
		break;
	default:
		memset(pLinearPointer, 0xFFu, wSharedMemSize);
	}
	return;
}

