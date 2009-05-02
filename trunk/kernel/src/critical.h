#ifndef _CRITICAL_H_
#define _CRITICAL_H_

#include <global.h>


#define CRITICAL_SECTION    _critical_ ## __LINE__ ## _
#define CRITICAL            for (bool CRITICAL_SECTION = Critical::begin(); \
                                    CRITICAL_SECTION; \
                                    CRITICAL_SECTION = Critical::end())

class Critical
{
public:

    static bool KERNEL_CALL
    begin();
    
    static bool KERNEL_CALL
    end();
    
    static Size KERNEL_CALL
    depth();
    
    
private:
    static Size     mDepth;    
    
    Critical();
};


#endif
