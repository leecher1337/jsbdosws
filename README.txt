JSB Virtual Socket Library for DOS 
Virtual Device Driver for DOS Virtual machines (=emulators)

(c) leecher@dose.0wnz.at 2020

===============================================================================
What is it?
===============================================================================
JSB software back in the days wrote a socket library that enabled DOS 
applications to use TCP sockets:

"The Virtual Socket Library is a single socket style API which provides 
transport and platform independent (TPI) development. By writting to the 
single VSL API, the developer is provided with complete connectivity over 
TCP/IP (all legacy and winsock stacks), DECnet, and IPX/SPX; this is offered 
on multiple platforms including Windows, Windows NT, DOS."

There are some DOS applications out there in the field (for instance 
Pervasive SQL server/BTRIEVE for DOS) which make use of this socket library.
This can be seen by the use of the JSBDOSWS.EXE executable which loads the
TSR.
If you are searching for a way to integrate BTRIEVE in DOSEMU, I suggest that
you use my https://github.com/leecher1337/btrieve-vdd project instead.

But nevertheless, this library may become handy for some legacy applications
that depend on this library.

This is a virtual device driver to connect the library service functions 
to any of the following DOS Virtual machines:

 * NTVDM  (Not needed, you get the driver from JSB anyway)
 * DOSEMU

If you want ports for different emulators and see a need for is, feel free to
do so yourself or contact me.

===============================================================================
File structure
===============================================================================
Common modules
	JSBDOSWS.H	- Header file for the Dispatch-function module
	JSBDOSWS.C	- The main dispatch-function module
	Platform dependent implementation of common modules (select ONE!)
		VSLFUNC_WIN32.C	- Windows implementation of platform functions
		VSLFUNC_WIN32.H
		VSLFUNC_LINUX.H	- Linux   implementation of platform functions
Emulator specific modules (entry points for emulator, select ONE!)
	MOD_NTVDM.C	- VDD for NTVDM
	MOD_DOSEMU.C	- VDD for DOSEMU

DEV/	DOS device drivers required for specific emulator
	DOSEMU
		JSBDOSWS.EXE	- Patched JSBDOSWS.EXE for use with DOSEMU
	NTVDM
		Just use JSBDOSWS.EXE shipped with your product

CONFIG/	Configuration files for DOSEMU plugin 

===============================================================================
Compiling
===============================================================================
DOSEMU:
-------
In Dosemu, this works as a plugin:
 * Create directory src/plugin/jsbdosws and copy the contents of this
   directory to it
 * ./default-configure
 * make
 * Copy DEV/dosemu/jsbdosws.exe to your Dosemu DOS directory
   ~/.dosemu/drives/c/ or whereever you have it to execute

All other emulators:
--------------------
In your makefile:
 * Add JSBDOSWS.C
 * Depending on platform (WIN32, LINUX, ...), add platform dependent 
   BTRFNC_*.c file, if exists
 * If on Windows, ensure that WIN32 define is set, so that correct
   .h gets loaded by JSBDOSWS.C
 * Depending on used DOS virtual machine, add the appropriate MOD_*.*

===============================================================================
Limitations
===============================================================================
I must admit that I haven't tested it so far. Hopefully, it works?

===============================================================================
Updates
===============================================================================
https://github.com/leecher1337/jsbdosws

===============================================================================
Author
===============================================================================
mail: leecher@dose.0wnz.at
https://github.com/leecher1337/

===============================================================================
License
===============================================================================
This Application and Sourcecode may be distributed freely.
The Sourcecode is Licensed in Terms of the GNU GPL v2.
See: http://www.gnu.org/licenses/gpl.html

Have a nice DOS...
