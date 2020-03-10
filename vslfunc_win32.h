/*
 * JSB Virtual socket library virtual device driver
 * Platform dependent functions
 *
 * leecher@dose.0wnz.at   02/2020
 *
 * https://github.com/leecher1337/jsbdosws
 */
BOOL bValidateVirtualSocket(SOCKET *pSocket);
BOOL bValidateRealSocket(SOCKET *pSocket);
BOOL setSelectInputFds(int nfds, PTVSLFDSET infds, fd_set *outfds);
void setSelectOutputFds(int nfds, char *infds, fd_set *outfds);

extern SOCKET  SocketArray[VSL_FD_SETSIZE];

#define VALIDATE_SOCKET \
	if (!bValidateVirtualSocket(&s)) \
	{ \
		SetGeneralReturnAndErrorValues(&p->Header, VSL_INVALID_SOCKET, VSL_ERR_NOTSOCK, -1, TRUE); \
		return; \
	}
