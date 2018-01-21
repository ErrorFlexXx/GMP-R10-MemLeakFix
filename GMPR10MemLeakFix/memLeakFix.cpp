#include "memLeakFix.h"
#include <tlhelp32.h>
#include "zString.h"
#include "zStringWrapper.h"
#include <iostream>

HANDLE	MemLeakFix::workerThread = NULL;//!< Handle of worker thread
DWORD	MemLeakFix::threadID = 0;		//!< ID of worker thread
BOOL	MemLeakFix::quitThread = false;	//!< Flag set from dllMain to quit this thread.
HMODULE	MemLeakFix::gmpHandle = NULL;	//!< Handle to loaded gmp (to get the base address)

/** starts a worker thread that actually fixes something.
 */
VOID MemLeakFix::initialize()
{
	if (workerThread == NULL) //No worker running ?
	{	//Start one:
		workerThread = CreateThread(
			NULL,			//Default security attributes
			0,				//Default stack size
			workerFunction,	//Thread function address
			NULL,			//No arguments
			0,				//Default thread creation flags
			&threadID);		//Store thread identifier
	}
}

/** replaces a function with another. (Signature and compiler type has to match)
 */
VOID MemLeakFix::Hook(DWORD hookSourceOffset, DWORD hookTargetAddress)
{
	DWORD oldProt, dummy; // Variables for usage of VirtualProtect.
	DWORD sourceAddress = (DWORD)gmpHandle + hookSourceOffset; //Calculate absolute address in current memory
	VirtualProtect((LPVOID)(sourceAddress), 5, PAGE_EXECUTE_READWRITE, &oldProt); //Get write permission
	*((unsigned char*)(sourceAddress)) = 0xE9; //JMP
	*((DWORD*)(sourceAddress + 1)) = (DWORD)hookTargetAddress - sourceAddress - 5; //Calc and write relative jmp offset
	VirtualProtect((LPVOID)(sourceAddress), 5, oldProt, &dummy); //Restore original permission
}

DWORD WINAPI MemLeakFix::workerFunction(LPVOID lpThreadParameter)
{
	BOOL fixed = FALSE;
	
	/* Keep the worker thread running to prevent unwanted dll unloading (Don't know if that can happen) */
	while (quitThread == false) //While thread shall not quit
	{
		Sleep(100); //Don't waste the CPU
		
		if (gmpHandle == NULL) //GMP handle not found... Searching.
		{
			gmpHandle = GetModuleHandleA(TARGET_PROGRAM_NAME); //Returns NULL if not loaded
		}
		else if (fixed == FALSE) //Fix not applied... Fixing.
		{
			//Hook zStringConstChar Constructor of gmp.dll
			zString* (__thiscall zStringWrapper::*ConstructorChar)(const char*) = &zStringWrapper::zStringWrapperCCC;
			Hook(FuncOffsets::zStringCreateConstCharTrampoline, (DWORD)((void*&) ConstructorChar)); //Hook const char constr.
			
			//Hook zStringInt Constructor of gmp.dll
			zString* (__thiscall zStringWrapper::*ConstructorInt)(int) = &zStringWrapper::zStringWrapperIC;
			Hook(FuncOffsets::zStringCreateIntTrampoline, (DWORD)((void*&)ConstructorInt)); //Hook const char constr.
			
			fixed = true; //Hooks placed - Do not hook again.
		}
	}

	//Cleanup
	workerThread = NULL;	//Clear worker thread handle.
	threadID = 0;			//Clear worher thread ID.
	return 0; //No error - normal exit code.
}
