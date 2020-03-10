/*
 * JSB Virtual socket library virtual device driver
 * Platform dependent functions
 *
 * leecher@dose.0wnz.at   02/2020
 *
 * https://github.com/leecher1337/jsbdosws
 */
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

#define SO_DONTLINGER (~SO_LINGER)
#define ioctlsocket ioctl
#define closesocket close
#define SOCKET int
#define WSAGetLastError() errno
#define WSABASEERR 0
#define INVALID_SOCKET (-1)
#define BOOL int
#define TRUE 1
#define FALSE 0

#define bValidateVirtualSocket(x) TRUE
#define bValidateRealSocket(x) TRUE
#define VALIDATE_SOCKET
