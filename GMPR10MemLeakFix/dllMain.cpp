#include <windows.h>
#include "memLeakFix.h"

#pragma pack(1)

HINSTANCE hLThis = 0;
HINSTANCE hL = 0;
FARPROC p[7] = { 0 };

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH: //DLL attach to program
	{	
		MemLeakFix::initialize(); //Init module (Start worker thread)

		//Setup proxy dll:
		hLThis = hInstance;
		//Thanks to ataulin :)
		//Technique of getting system path taken from GD3D11-Renderer Project.
		char infoBuf[MAX_PATH];
		GetSystemDirectoryA(infoBuf, MAX_PATH);
		strcat_s(infoBuf, MAX_PATH, "\\dinput.dll");

		hL = LoadLibrary(infoBuf);
		if (!hL) return false;
		p[0] = GetProcAddress(hL, "DirectInputCreateA");
		p[1] = GetProcAddress(hL, "DirectInputCreateEx");
		p[2] = GetProcAddress(hL, "DirectInputCreateW");
		p[3] = GetProcAddress(hL, "DllCanUnloadNow");
		p[4] = GetProcAddress(hL, "DllGetClassObject");
		p[5] = GetProcAddress(hL, "DllRegisterServer");
		p[6] = GetProcAddress(hL, "DllUnregisterServer");
		
		break;
	}
	case DLL_PROCESS_DETACH: //Program closing - Stop
	{
		if (MemLeakFix::workerThread != NULL) //If there is a worker running
		{
			MemLeakFix::quitThread = true; //Indicate that thread shall stop now.
		}
		FreeLibrary(hL);
		break;
	}
	}
	return 1;
}

// DirectInputCreateA
extern "C" __declspec(naked) void __stdcall __E__0__()
{
	__asm
	{
		jmp p[0 * 4];
	}
}

// DirectInputCreateEx
extern "C" __declspec(naked) void __stdcall __E__1__()
{
	__asm
	{
		jmp p[1 * 4];
	}
}

// DirectInputCreateW
extern "C" __declspec(naked) void __stdcall __E__2__()
{
	__asm
	{
		jmp p[2 * 4];
	}
}

// DllCanUnloadNow
extern "C" __declspec(naked) void __stdcall __E__3__()
{
	__asm
	{
		jmp p[3 * 4];
	}
}

// DllGetClassObject
extern "C" __declspec(naked) void __stdcall __E__4__()
{
	__asm
	{
		jmp p[4 * 4];
	}
}

// DllRegisterServer
extern "C" __declspec(naked) void __stdcall __E__5__()
{
	__asm
	{
		jmp p[5 * 4];
	}
}

// DllUnregisterServer
extern "C" __declspec(naked) void __stdcall __E__6__()
{
	__asm
	{
		jmp p[6 * 4];
	}
}
