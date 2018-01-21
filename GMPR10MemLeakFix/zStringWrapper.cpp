#include "zStringWrapper.h"
#include <windows.h>
#include <iostream>
#include "zString.h"
#include "garbageCollector.h"

zString* zStringWrapper::zStringWrapperCCC(const char *text)
{
	zString *calledForAddr = ((zString*)(this)); //Get address, which this constr. is called for
	new(calledForAddr) zString(text); //Call of zString constructor with that address
	GarbageCollector::addObject(calledForAddr); //Add to garbage collector.
	return calledForAddr;
}

zString* zStringWrapper::zStringWrapperIC(int str)
{		
	zString *calledForAddr = ((zString*)(this)); //Get address, which this constr. is called for
	new(calledForAddr) zString(str); //Call of zString constructor with that address
	GarbageCollector::addObject(calledForAddr); //Add to garbage collector.
	return calledForAddr;
}

/*
zString& zStringWrapper::operator= (const zString &zstr)
{
	zString *calledForAddr = ((zString*)(this)); //Get address, which this constr. is called for
	*calledForAddr = zstr; //Call of assignment operator
	GarbageCollector::addObject(calledForAddr); //Add to garbage collector.
	return *calledForAddr;
}
*/
