#include "zString.h"

zString::zString(const char *text)
{
	XCALL(0x004010C0);
}

zString::zString(int str)
{
	XCALL(0x00435870);
}

/*
zString& zString::operator= (const zString &zstr)
{
	XCALL(0x0059CEB0);
}
*/

zString::~zString()
{
	XCALL(0x00401160);
}
