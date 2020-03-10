/*
 * JSB Virtual socket library virtual device driver
 * NTVDM main dispatcher and entry point
 *
 * leecher@dose.0wnz.at   02/2020
 *
 * https://github.com/leecher1337/jsbdosws
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vddsvc.h>
#include "vsl.h"
#include "vslfunc_win32.h"
#include "jsbdosws.h"

#pragma comment (lib, "ntvdm.lib")

extern WORD wSharedMemSize;
extern ULONG X86Address;
extern PBYTE pLinearPointer;

BOOL bWinsockInitialized = FALSE;

__declspec(dllexport) BOOL __cdecl
jsbdoswsInitialize(
    HANDLE   hVdd,
    DWORD    dwReason,
    LPVOID   lpReserved)
{
	if (dwReason == DLL_PROCESS_DETACH)
	{
		if (bWinsockInitialized)
		{
			int i;

			for (i=0; i<VSL_FD_SETSIZE; i++)
			{
				if (SocketArray[i] != INVALID_SOCKET)
				{
					closesocket(SocketArray[i]);
					SocketArray[i] = INVALID_SOCKET;
				}
			}
			bWinsockInitialized = FALSE;
			WSACleanup();
		}
		if (X86Address && pLinearPointer)
		{
			wSharedMemSize = 0;
			X86Address = 0;
			pLinearPointer = NULL;
		}
	}
    return TRUE;
}

__declspec(dllexport) VOID __cdecl
jsbdoswsInit(
    VOID
    )
{
	if (!bWinsockInitialized)
	{
		WSADATA wsaData;

		bWinsockInitialized = WSAStartup(VSLSOCKVERSION, &wsaData) == 0;
	}
    setCF(!bWinsockInitialized);
}

__declspec(dllexport) VOID __cdecl
jsbdoswsDispatch()
{
	ULONG addr = getBX() | (getDX() << 16);

	if (!X86Address || addr != X86Address || getCX() != wSharedMemSize)
	{
		wSharedMemSize = getCX();
		X86Address = addr;
		pLinearPointer = (PBYTE)MGetVdmPointer(X86Address, (USHORT)getCX(), 0);
	}
	jsbdosws_Dispatch((USHORT)getES());
}

