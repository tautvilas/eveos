#ifndef _FIXED_STACK_
#define _FIXED_STACK_

#include <types.h>


namespace Generic {


template <typename T, typename BidirectionalIterator = T* >
class FixedStack
{
public:

    typedef BidirectionalIterator Iterator;

    /**
     *  Constructor.
     *
     *  Stack grows from end down to first in range [first; end).
     */
    
    FixedStack(BidirectionalIterator first, BidirectionalIterator end);
    
    
    FixedStack(BidirectionalIterator first, Size size);
    
    
    FixedStack();

    void 
    push(const T& value);

    T 
    pop();

    T& 
    top();

    const T& 
    top() const;

    BidirectionalIterator 
    begin();

    const BidirectionalIterator 
    begin() const;

    BidirectionalIterator 
    end();

    const BidirectionalIterator 
    end() const;

    Size 
    size() const;
    
    Bool 
    empty() const;
    

private:

    BidirectionalIterator   mFirst;
    BidirectionalIterator   mEnd;
    BidirectionalIterator   mCurr;

};

#ifdef _OUT_STREAM_H_

template <typename T, typename I>
inline OutStream& 
operator << (OutStream& stream, const FixedStack<T, I>& stack)
{
    I last = stack.end();
    --last;
    stream << '{';
    for (I i = stack.begin(); i != stack.end();  ++i)
    {
        stream << *i;
        if (i != last)
            stream << ", ";
    }
    stream << '}';

    return stream;
}

#endif  // TOutStream



template <typename T, typename I>
inline 
FixedStack<T, I>::FixedStack(I first, I end)
        : mFirst(first), mEnd(end), mCurr(end)
{
    ASSERT(mFirst <= mEnd);
}


template <typename T, typename I>
inline 
FixedStack<T, I>::FixedStack(I first, Size size)
        : mFirst(first), mEnd(first + size), mCurr(first + size)
{
    ASSERT(mFirst <= mEnd);
}


template <typename T, typename I>
inline 
FixedStack<T, I>::FixedStack()
        : mFirst(0), mEnd(0), mCurr(0)
{}


template <typename T, typename I>
inline void 
FixedStack<T, I>::push(const T& value)
{
    ASSERT(mCurr > mFirst && mCurr <= mEnd);

    *--mCurr = value;
}


template <typename T, typename I>
inline T 
FixedStack<T, I>::pop()
{
    ASSERT(mCurr >= mFirst && mCurr < mEnd);

    return *mCurr++;
}


template <typename T, typename I>
inline T& 
FixedStack<T, I>::top()
{
    ASSERT(mCurr >= mFirst && mCurr < mEnd);

    return *mCurr;
}


template <typename T, typename I>
inline const T& 
FixedStack<T, I>::top() const
{
    ASSERT(mCurr <= mFirst && mCurr > mEnd);

    return *mCurr;
}


template <typename T, typename I>
inline I 
FixedStack<T, I>::begin()
{
    return mCurr;
}


template <typename T, typename I>
inline const I 
FixedStack<T, I>::begin() const
{
    return mCurr;
}


template <typename T, typename I>
inline I 
FixedStack<T, I>::end()
{
    return mEnd;
}


template <typename T, typename I>
inline const I 
FixedStack<T, I>::end() const
{
    return mEnd;
}


template <typename T, typename I>
inline Size 
FixedStack<T, I>::size() const
{
    return end() - begin();
}


template <typename T, typename I>
inline Bool 
FixedStack<T, I>::empty() const
{
    return end() == begin();
}


};  // namespace Generic

#endif  // _FIXED_STACK_
