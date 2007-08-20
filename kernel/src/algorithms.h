#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "global.h"

namespace Generic {

////// Declarations //////

template <typename TInputIterator, typename TOutputIterator>
TOutputIterator KERNEL_CALL
Copy(TInputIterator aFirst, TInputIterator aLast, TOutputIterator aResult);


template <typename TOutputIterator, typename TValue>
void KERNEL_CALL
Fill(TOutputIterator aFirst, TOutputIterator aLast, const TValue& aValue);


template <typename TInputIterator, typename TValue>
TInputIterator KERNEL_CALL
Find(TInputIterator aFirst, TInputIterator aLast, const TValue& aValue);


template <typename TInteger>
TInteger KERNEL_CALL
DivUp(const TInteger& aValue, const TInteger& aDivisor);


int KERNEL_CALL
Abs(int aValue);



////// Templates implementation //////

template <typename TInputIterator, typename TOutputIterator>
TOutputIterator KERNEL_CALL
Copy(TInputIterator aFirst, TInputIterator aLast, TOutputIterator aResult)
{
    for (; aFirst != aLast; ++aFirst, ++aResult)
        *aResult = *aFirst;
    return aResult;
}


template <typename TOutputIterator, typename TValue>
void KERNEL_CALL
Fill(TOutputIterator aFirst, TOutputIterator aLast, const TValue& aValue)
{
    for (; aFirst != aLast; ++aFirst)
        *aFirst = aValue;
}


template <typename TInputIterator, typename TValue>
TInputIterator KERNEL_CALL
Find(TInputIterator aFirst, TInputIterator aLast, const TValue& aValue)
{
    for (; aFirst != aLast; ++aFirst)
        if (aValue == *aFirst)
            break;

    return aFirst;
}


template <typename TInteger>
TInteger KERNEL_CALL
DivUp(const TInteger& aValue, const TInteger& aDivisor)
{
    TInteger result = aValue / aDivisor;
    if (aValue % aDivisor > 0)
        ++result;
    return result;
}


inline int KERNEL_CALL
Abs(int aValue)
{
    if (aValue < 0)
        return ~aValue + 1;
    else
        return aValue;
}


}; // namespace Generic

#endif // _ALGORITHMS_H_

