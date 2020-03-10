/*
 * JSB Virtual socket library virtual device driver
 * Platform dependent functions
 *
 * leecher@dose.0wnz.at   02/2020
 *
 * https://github.com/leecher1337/jsbdosws
 */
#include "vsl.h"

#define VSL__NFDBITS (VSL_FD_SETSIZE/8)
#define VSL__FDSET_LONGS (VSL_FD_SETSIZE/VSL__NFDBITS)
 typedef struct {
   unsigned long fds_bits[VSL__FDSET_LONGS];
 } vsl_fd_set;
#define VSL__FDELT(fd) ((fd) / VSL__NFDBITS)
#define VSL__FDMASK(fd) (1UL << ((fd) % VSL__NFDBITS))
#define VSL__FDS_BITS(set) (((vsl_fd_set*)(set))->fds_bits)
#define VSL_FD_CLR(fd, set) (VSL__FDS_BITS(set)[VSL__FDELT(fd)] &= ~VSL__FDMASK(fd))
#define VSL_FD_SET(fd, set) (VSL__FDS_BITS(set)[VSL__FDELT(fd)] |= VSL__FDMASK(fd))
#define VSL_FD_ISSET(fd, set) ((VSL__FDS_BITS(set)[VSL__FDELT(fd)] & VSL__FDMASK(fd)) != 0)

SOCKET  SocketArray[VSL_FD_SETSIZE]={INVALID_SOCKET};

BOOL bValidateVirtualSocket(SOCKET *pSocket)
{
	if (*pSocket < VSL_FD_SETSIZE)
	{
		if (SocketArray[*pSocket] != INVALID_SOCKET) 
		{
			*pSocket = SocketArray[*pSocket];
			return TRUE;
		}
	}
	return FALSE;
}

BOOL bValidateRealSocket(SOCKET *pSocket)
{
	int i;

	for (i=0; i<VSL_FD_SETSIZE; i++)
	{
		if (SocketArray[i] == *pSocket)
		{
			*pSocket = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL setSelectInputFds(int nfds, PTVSLFDSET infds, fd_set *outfds)
{
	int i, sets=0;
	SOCKET s;

	for (i=0; i<nfds && outfds->fd_count<FD_SETSIZE; i++)
	{
		if (VSL_FD_ISSET(i, infds))
		{
			s=i;
			if (bValidateVirtualSocket(&s))
			{
				if (outfds->fd_count < FD_SETSIZE)
					outfds->fd_array[outfds->fd_count++] = s;
				else
					return FALSE;
			}
		}
	}
	return TRUE;
}

void setSelectOutputFds(int nfds, char *infds, fd_set *outfds)
{
	u_int i;
	SOCKET s;

	for (i=outfds->fd_count>FD_SETSIZE?FD_SETSIZE:outfds->fd_count; i>0; i--)
	{
		s = outfds->fd_array[i];
		if (bValidateRealSocket(&s))
			VSL_FD_SET(s, infds);
	}
}

