#ifndef _NEW_H_
#define _NEW_H_

#include <global.h>

void*
operator new(Size size);


void* 
operator new[](Size size);


void 
operator delete(void* p);


void 
operator delete[](void* p);


#endif
