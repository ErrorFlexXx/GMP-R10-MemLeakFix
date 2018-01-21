#pragma once

class zString; //Forward declaration

/** is a class that defines hook targets. This methods replace the gmp routines.
 */
class zStringWrapper
{
public:
	//Hook targets:
	zString* zStringWrapperCCC(const char*);	//!< zStringWrapperConstCharConstructor
	zString* zStringWrapperIC(int str);			//!< zStringWrapperIntConstructor
	zString& operator= (const zString &zstr);	//!< zStringWrapperAssignOperator
};

