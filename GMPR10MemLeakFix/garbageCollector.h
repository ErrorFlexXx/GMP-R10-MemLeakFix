#pragma once

#include <windows.h>
#include <list>
#include <map>

class zString; //Forward declaration of the zString class

/** Watched objected with all necessary attributes
 */
struct watchObject
{
	DWORD stackPointer; /**< holds the stack pointer while creating the variable */
	DWORD vtable;		/**< holds the vtable of this object. */
	DWORD allocator;	/**< is currently unknown. */
	DWORD ptr;			/**< is currently unknown. */
	int length;			/**< represents the length of the string */
	char* ressource;	/**< Actual garbage at end of life */
	DWORD timestamp;	/**< Time at which the object was created */
};

/** an abstract class to handle cleanup of unhandled allocated resources
 */
class GarbageCollector
{
public:
	/************
	 * Methods: *
	 ************/

	/** Method to add a new object and the regarding thread id to the watch list. */
	static void addObject(zString *object);

	/** Returns a pointer to the list corresponding to the thread id or 
	 *  created a new list for the thread id and returns this pointer.
	 */
	static std::list<watchObject>* getListOfThread(DWORD threadId);

	/**************
	* Attributes: *
	***************/
	
	/** Map of lists to create a link between thread ID and list of objects. */
	static std::map<DWORD, std::list<watchObject>*> threadObjectLists;

	/** Mutex to prevent list accessing at the same time from multiple threads. */
	static HANDLE mutex;

private:
	GarbageCollector() {}; //Abstract class - Static access only
};

