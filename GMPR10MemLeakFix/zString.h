#pragma once

#include "windows.h"

#define XCALL(address)	\
	__asm { mov esp, ebp		} \
	__asm { pop ebp				} \
	__asm { mov eax, address	} \
	__asm { jmp eax				}


/** is the zString image
 */
class zString
{
public:
	//Attributes of a zString object
	DWORD vtable;	/**< holds the vtable of this object. */
	DWORD allocator;/**< is currently unknown. */
	DWORD ptr;		/**< is currently unknown. */
	int length;		/**< represents the length of the string */
	char* resource;	/**< is the actual text base of the zString. */
	
	//! constructor to create a zString from a const char* variable.
	zString(const char *text);
	
	//! constructor to create a zString from an integer variable.
	zString(int str);

	//! assign operator to copy contents from one zString into another.
	//zString& operator= (const zString &zstr);
	
	//! the (mostly forgotten D:) destructor of a zString.
	~zString();
};

