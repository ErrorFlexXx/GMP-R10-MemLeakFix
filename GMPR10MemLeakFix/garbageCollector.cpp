#include "garbageCollector.h"
#include "zString.h"

std::map<DWORD, std::list<watchObject>*> GarbageCollector::threadObjectLists;
HANDLE GarbageCollector::mutex = CreateMutex(NULL, FALSE, NULL);
DWORD stackMax = 0;

void GarbageCollector::addObject(zString* object)
{
	WaitForSingleObject(mutex, INFINITE); //Get lock

	void* currentStackPointer;
	__asm { mov currentStackPointer, ESP }
	DWORD currentThreadID = GetCurrentThreadId();
	std::list<watchObject> *threadObjList = getListOfThread(currentThreadID);
	DWORD currentTicks = GetTickCount();

	watchObject newObject; //Create new watch Object
	newObject.stackPointer = (DWORD)currentStackPointer;
	newObject.vtable = object->vtable;		//Copy all zString attributes to delete
	newObject.allocator = object->allocator;//It later with the
	newObject.length = object->length;		//official zString destructor.
	newObject.ptr = object->ptr;
	newObject.ressource = object->resource;
	newObject.timestamp = currentTicks;
	
	
	threadObjList->push_front(newObject);
	//Clean up old objects
	// (Based on Stack Pointer)
	while (!threadObjList->empty() && threadObjList->back().stackPointer < (DWORD)currentStackPointer)
	{
		DWORD zStringAddr = (DWORD)&((threadObjList->back())); //Take out of scope object.
		zStringAddr += 4; //Skip stack pointer, which is no part of zString object.
		((zString*)(zStringAddr))->~zString(); //Call the official zString Destructor for the zString.
		threadObjList->pop_back(); //Remove the object from garbage collector's watchlist.
	}
	//Clean up old objects
	// (Based on Max Lifetime)
	while (currentTicks - threadObjList->back().timestamp > 10000) //If this guy gets never deleted
	{
		DWORD zStringAddr = (DWORD)&((threadObjList->back())); //Take out of scope object.
		zStringAddr += 4; //Skip stack pointer, which is no part of zString object.
		((zString*)(zStringAddr))->~zString(); //Call the official zString Destructor for the zString.
		threadObjList->pop_back();
	}

	ReleaseMutex(mutex); //Release lock
}

std::list<watchObject>* GarbageCollector::getListOfThread(DWORD threadId)
{
	if (threadObjectLists.find(threadId) == threadObjectLists.end()) //No list for thread ?
	{
		std::list<watchObject> *newList = new std::list<watchObject>();	//Create new list
		threadObjectLists.insert(std::pair<DWORD, std::list<watchObject>*>(threadId, newList));
		return newList; //Return this new list.
	}
	return threadObjectLists.at(threadId); //Return corresponding list
}
