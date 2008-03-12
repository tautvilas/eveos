#ifndef _MEM_H_
#define _MEM_H_

#include "global.h"
#include "fixed_stack.h"
#include "algorithms.h"

class Mem
{
public:

    static void KERNEL_CALL
    init();


private:

    class TPage
    {
    public:

        enum TConstant
        {
            SIZE        = 4 * KILOBYTE,
        };

        inline KERNEL_CALL
        TPage();

        KERNEL_CALL
        TPage(Byte* apAddress);

        KERNEL_CALL
        TPage(Size aIndex);


        Byte* KERNEL_CALL
        Address();


        Byte& KERNEL_CALL
        operator [](Size aIndex);
        //{
        //    return reinterpret_cast<TByte*>(Address())[aIndex];
        //}

        KERNEL_CALL
        operator Byte*()
        {
            return mpAddress;
        }


        TPage& KERNEL_CALL
        Present();

        TPage& KERNEL_CALL
        NotPresent();

        TPage& KERNEL_CALL
        ReadWrite();

        TPage& KERNEL_CALL
        ReadOnly();

        TPage& KERNEL_CALL
        User();

        TPage& KERNEL_CALL
        System();

        /**
         *  Present().System().ReadWrite()
         */
        TPage& KERNEL_CALL
        SysRw();

        /**
         *  Present().User().ReadWrite()
         */
        TPage& KERNEL_CALL
        UserRw();


    protected:

        enum TProtectedConstant
        {
            SHIFT   = 12,
            MASK    = 0xfffff000,
        };

        enum TFlag
        {
            PRESENT         = 1,            // ...00001b
            READ_WRITE      = 2,            // ...00010b
            USER            = 4,            // ...00100b
            CLEAR           = 0xFFFFFFF8,   // ...11000b
        };

        Byte*   mpAddress;

    };


    class TPageDir
    {
    public:

        KERNEL_CALL
        TPageDir(TPage aPage);

        void KERNEL_CALL
        Map(TPage aPhysicalPage, TPage aVirtualPage);

    private:

        class TPageTbl
                : TPage
        {
        public:

            TPage& KERNEL_CALL
            operator [](Size aIndex);
            /*{
                return reinterpret_cast<TPage*>(Address())[aIndex];
            }*/

        private:

            enum TPrivateConstant
            {
                SIZE        = 1024,
            };

            TPage*  mpPages;
        };


        enum TPrivateConstant
        {
            SIZE        = 1024,
        };


        TPageTbl*   mpTables;
        /*
            mpTables = aPage.Address();
            mpTables[SIZE - 1] = TPageTbl(aPage.Address()).SysRw();
            mpTables[SIZE - 1][SIZE - 1]
        */
    };

    typedef Generic::FixedStack<TPage>  PageStack;

    static PageStack    msFreePages;

    KERNEL_CALL
    Mem();

};



////// Mem::TPage inlines //////


inline KERNEL_CALL
Mem::TPage::TPage()
        : mpAddress(NULL)
{}


inline KERNEL_CALL
Mem::TPage::TPage(Byte* apAddress)
        : mpAddress(Generic::maskBits(apAddress, UInt(MASK)))
{}


inline KERNEL_CALL
Mem::TPage::TPage(Size aIndex)
        : mpAddress(reinterpret_cast<Byte*>(aIndex << SHIFT))
{}


inline Byte* KERNEL_CALL
Mem::TPage::Address()
{
    return mpAddress;
}


#endif // _MEM_H_
