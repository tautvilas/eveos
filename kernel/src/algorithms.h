#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "global.h"

/**
 *  Iterator concepts: http://www.sgi.com/tech/stl/Iterators.html
 */


namespace Generic {

////// Declarations //////

template <typename InputIterator, typename OutputIterator>
OutputIterator KERNEL_CALL
copy(InputIterator first, InputIterator last, OutputIterator result);


template <typename OutputIterator, typename Value>
void KERNEL_CALL
fill(OutputIterator first, OutputIterator last, const Value& value);


template <typename InputIterator, typename Value>
InputIterator KERNEL_CALL
find(InputIterator first, InputIterator last, const Value& value);


//template <typename TInteger>
//TInteger KERNEL_CALL
//DivUp(const TInteger& aValue, const TInteger& aDivisor);

UInt KERNEL_CALL
divUp(const UInt& value, const UInt& divisor);

template <typename T, typename M>
inline T KERNEL_CALL
mask(const T& value, const M& mask);

int KERNEL_CALL
abs(int value);

template <typename T>
const T& KERNEL_CALL
max(const T& value1, const T& value2);

template <typename T>
const T& KERNEL_CALL
min(const T& value1, const T& value2);



////// Templates implementation //////

template <typename InputIterator, typename OutputIterator>
OutputIterator KERNEL_CALL
copy(InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}


template <typename OutputIterator, typename Value>
void KERNEL_CALL
fill(OutputIterator first, OutputIterator last, const Value& value)
{
    for (; first != last; ++first)
        *first = value;
}


template <typename InputIterator, typename Value>
InputIterator KERNEL_CALL
find(InputIterator first, InputIterator last, const Value& value)
{
    for (; first != last; ++first)
        if (value == *first)
            break;

    return first;
}


//template <typename TInteger>
//TInteger KERNEL_CALL
//DivUp(const TInteger& aValue, const TInteger& aDivisor)
//{
//    TInteger result = aValue / aDivisor;
//    if (aValue % aDivisor > 0)
//        ++result;
//    return result;
//}


inline UInt KERNEL_CALL
divUp(const UInt& value, const UInt& divisor)
{
    UInt result = value / divisor;
    if (value % divisor > 0)
        ++result;
    return result;
}


inline int KERNEL_CALL
abs(int value)
{
    if (value < 0)
        return ~value + 1;
    else
        return value;
}


template <typename T>
inline const T& KERNEL_CALL
max(const T& value1, const T& value2)
{
    return (value2 < value1) ? value1 : value2;
}


template <typename T>
inline const T& KERNEL_CALL
min(const T& value1, const T& value2)
{
    return (value2 < value1) ? value2 : value1;
}


template <typename T, typename M>
inline T KERNEL_CALL
maskBits(const T& value, const M& mask)
{
    return (T)((M)value & mask);
}


template <typename T, typename M>
inline T KERNEL_CALL
setBits(const T& value, const M& mask)
{
    return (T)((M)value | mask);
}


template <typename T, typename M>
inline T KERNEL_CALL
shiftLeft(const T& value, const M& mask)
{
    return (T)((M)value << mask);
}


template <typename T, typename M>
inline T KERNEL_CALL
shiftRight(const T& value, const M& mask)
{
    return (T)((M)value >> mask);
}





}; // namespace Generic

#endif // _ALGORITHMS_H_
