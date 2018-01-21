#include "garbageCollector.h"
#include "zString.h"

std::map<DWORD, std::list<watchObject>*> GarbageCollector::threadObjectLists;
HANDLE GarbageCollector::mutex = CreateMutex(NULL, FALSE, NULL);

void GarbageCollector::addObject(zString* object)
{
	WaitForSingleObject(mutex, INFINITE); //Get lock

	void* currentStackPointer;
	__asm { mov currentStackPointer, ESP }
	DWORD currentThreadID = GetCurrentThreadId();
	std::list<watchObject> *threadObjList = getListOfThread(currentThreadID);

	watchObject newObject; //Create new watch Object
	newObject.stackPointer = (DWORD)currentStackPointer;
	newObject.vtable = object->vtable;		//Copy all zString attributes to delete
	newObject.allocator = object->allocator;//It later with the
	newObject.length = object->length;		//official zString destructor.
	newObject.ptr = object->ptr;
	newObject.ressource = object->resource;
	
	//Delete out of scope objects
	while (!threadObjList->empty() && threadObjList->front().stackPointer < (DWORD)currentStackPointer)
	{
		DWORD zStringAddr = (DWORD)&((threadObjList->front())); //Take out of scope object.
		zStringAddr += 4; //Skip stack pointer, which is no part of zString object.
		((zString*)(zStringAddr))->~zString(); //Call the official zString Destructor for the zString.
		threadObjList->pop_front(); //Remove the object from garbage collector's watchlist.
	}
	threadObjList->push_front(newObject); //Add object to garbage collection

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
