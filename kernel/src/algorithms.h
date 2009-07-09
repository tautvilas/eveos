#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "global.h"

/**
 *  Iterator concepts: http://www.sgi.com/tech/stl/Iterators.html
 */


namespace Generic {

////// Declarations //////

template <typename InputIterator, typename OutputIterator>
OutputIterator 
copy(InputIterator first, InputIterator last, OutputIterator result);


template <typename OutputIterator, typename Value>
void 
fill(OutputIterator first, OutputIterator last, const Value& value);


template <typename InputIterator, typename Value>
InputIterator 
find(InputIterator first, InputIterator last, const Value& value);


//template <typename TInteger>
//TInteger 
//DivUp(const TInteger& aValue, const TInteger& aDivisor);

UInt 
divUp(const UInt& value, const UInt& divisor);

template <typename T, typename M>
inline T 
mask(const T& value, const M& mask);

int 
abs(int value);

template <typename T>
const T& 
max(const T& value1, const T& value2);

template <typename T>
const T& 
min(const T& value1, const T& value2);



////// Templates implementation //////

template <typename InputIterator, typename OutputIterator>
OutputIterator 
copy(InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}


template <typename OutputIterator, typename Value>
void 
fill(OutputIterator first, OutputIterator last, const Value& value)
{
    for (; first != last; ++first)
        *first = value;
}


template <typename InputIterator, typename Value>
InputIterator 
find(InputIterator first, InputIterator last, const Value& value)
{
    for (; first != last; ++first)
        if (value == *first)
            break;

    return first;
}


//template <typename TInteger>
//TInteger 
//DivUp(const TInteger& aValue, const TInteger& aDivisor)
//{
//    TInteger result = aValue / aDivisor;
//    if (aValue % aDivisor > 0)
//        ++result;
//    return result;
//}


inline UInt 
divUp(const UInt& value, const UInt& divisor)
{
    UInt result = value / divisor;
    if (value % divisor > 0)
        ++result;
    return result;
}


inline int 
abs(int value)
{
    if (value < 0)
        return ~value + 1;
    else
        return value;
}


template <typename T>
inline const T& 
max(const T& value1, const T& value2)
{
    return (value2 < value1) ? value1 : value2;
}


template <typename T>
inline const T& 
min(const T& value1, const T& value2)
{
    return (value2 < value1) ? value2 : value1;
}


template <typename T, typename M>
inline T 
maskBits(const T& value, const M& mask)
{
    return (T)((M)value & mask);
}


template <typename T, typename M>
inline T 
setBits(const T& value, const M& mask)
{
    return (T)((M)value | mask);
}


template <typename T, typename M>
inline T 
shiftLeft(const T& value, const M& mask)
{
    return (T)((M)value << mask);
}


template <typename T, typename M>
inline T 
shiftRight(const T& value, const M& mask)
{
    return (T)((M)value >> mask);
}





}; // namespace Generic

#endif // _ALGORITHMS_H_
