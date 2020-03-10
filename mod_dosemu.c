/*
 * JSB Virtual socket library virtual device driver
 * DOSEMU main dispatcher
 *
 * leecher@dose.0wnz.at   02/2020
 *
 * https://github.com/leecher1337/jsbdosws
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "emu.h"
#include "cpu.h"
#include "memory.h"
#include "doshelpers.h"

#include "jsbdosws.h"

extern u_short wSharedMemSize;
extern u_long X86Address;
extern u_char *pLinearPointer;

int jsbdoswsDispatch(void);

int jsbdoswsDispatch(void)
{
    u_long addr = SEGOFF2LINEAR(_DX, _BX);

    if (!X86Address || addr != X86Address || _CX != wSharedMemSize)
    {
        wSharedMemSize = _CX;
        X86Address = addr;
        pLinearPointer = (u_char*)addr;
    }
    jsbdosws_Dispatch((u_short)_ES);
    return 1;
}

