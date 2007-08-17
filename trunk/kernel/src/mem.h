#ifndef _MEM_H_
#define _MEM_H_

class Mem
{
public:

    template <typename T>
    static T* KERNEL_CALL
    Copy(T* apDest, const T* apSrc, Size aSize);

    template <typename T>
    static T* KERNEL_CALL
    Set(T* apDest, Size aSize, T aVal);
};



template <typename T>
/*static*/ inline T* KERNEL_CALL
Mem::Copy(T* apDest, const T* apSrc, Size aSize)
{
    T* dest = apDest;
    for (; aSize != 0; --aSize, ++dest, ++apSrc)
        *dest = *apSrc;
    return apDest;
}


template <typename T>
/*static*/ T* KERNEL_CALL
Mem::Set(T* apDest, Size aSize, T aVal)
{
    T* dest = apDest;
    for (; aSize != 0; --aSize, ++dest)
        *dest = aVal;
    return apDest;
}


#endif // _MEM_H_

