#ifndef _MAYBE_H_
#define _MAYBE_H_

#include <types.h>


struct None 
{};


template <typename Value>
class Maybe
{
public:    
    Maybe(Value val) : mVal(val), mEmpty(false) {}
    Maybe(None) : mEmpty(true) {}

    Bool KERNEL_CALL
    empty() const 
    { 
        return mEmpty;
    }

    Value KERNEL_CALL
    value(Value emptyVal = Value()) const
    {
        return empty() ? emptyVal : mVal;
    }
    
    
private:
    Value mVal;
    Bool mEmpty;
};


#endif

