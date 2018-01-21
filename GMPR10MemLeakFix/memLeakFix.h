#pragma once

#include <windows.h>
#include <list>

#define TARGET_PROGRAM_NAME	"gmp.dll"

class zString; //Forward declaration of zString class

/** Offsets to functions of the gmp.dll
 */
class FuncOffsets
{
public:
	static const DWORD zStringCreateConstCharTrampoline	= 0x0001560;
	static const DWORD zStringCreateIntTrampoline		= 0x0034e70;
};

/** Class to apply this fix and handle the loading of this module.
 */
class MemLeakFix
{
public:
	static HANDLE				workerThread;
	static DWORD				threadID;
	static BOOL					quitThread;
	static HMODULE 				gmpHandle;
	static VOID					initialize();
	static DWORD WINAPI			workerFunction(LPVOID lpThreadParameter);
	static VOID					Hook(DWORD hookSourceOffset, DWORD hookTargetAddress);
	
private:
	MemLeakFix(); //Abstract class
};
