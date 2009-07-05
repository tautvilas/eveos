#include <global.h>
#include <io_port.h>
#include <debug.h>
#include <out.h>
#include <isr.h>

extern "C" void enable_int();
extern "C" Byte kernel_cs_sel;
extern "C" void exc0(), exc1(), exc2(), exc3(), exc4(), exc5(), exc6(), exc7(), exc8(),
                exc9(), exc10(), exc11(), exc12(), exc13(), exc14(), exc15(), exc16(),
                exc17(), exc18(), exc19(), exc20(), exc21(), exc22(), exc23(), exc24(),
                exc25(), exc26(), exc27(), exc28(), exc29(), exc30(), exc31(),

                irq0(), irq1(), irq2(), irq3(), irq4(), irq5(), irq6(), irq7(), irq8(),
                irq9(), irq10(), irq11(), irq12(), irq13(), irq14(), irq15(),

                sys69();

namespace Isr {


namespace {

    typedef struct Regs Regs;
    typedef struct IdtPtr IdtPtr;

    extern "C" void KERNEL_CALL isrCommonHandler(Regs*);
    extern "C" void load_idt(IdtPtr*);

    typedef struct Regs
    {
        DWord gs, fs, es, ds;
        DWord edi, esi, ebp, esp, ebx, edx, ecx, eax;
        DWord intNo, errCode;
        DWord eip, cs, eflags, useresp, ss;
    };

    typedef struct IdtPtr
    {
        Word limit;
        DWord base;
    } __attribute__ ((packed));

    typedef struct
    {
        // ISR address low word
        Word baseLow;
        // kernel segment
        Word sel;
        // always set this to 0
        Byte always0;
        // 1 - present?1:0; 2 - ring; 01110 in the end
        Byte flags;
        // ISR address high word
        Word baseHigh;
    } __attribute__ ((packed)) IdtGate;

    enum
    {
        IRQ_FIRST_INDEX = 32,
        IRQ_LAST_INDEX = 46,
        // entry is present, ring 3
        ISR_FLAGS = 0xEE,
        PIC1 = 0x20,
        PIC2 = 0xA0,
        // PIC command ports
        PIC1_COMMAND = PIC1,
        PIC2_COMMAND = PIC2,
        // PIC data ports
        PIC1_DATA = (PIC1 + 1),
        PIC2_DATA = (PIC2 + 1),
        // End of interrupt signal
        PIC_EOI = 0x20,
    };

    const char* mExceptionNames[] =
    {
        "Division By Zero",
        "Debug",
        "Non Maskable interrupt",
        "Breakpoint",
        "Into  Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No coprocessor",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",
        "Coprocessor Fault",
        "Aligment Check",
        "Machine Check",
        "Reserved 19",
        "Reserved 20",
        "Reserved 21",
        "Reserved 22",
        "Reserved 23",
        "Reserved 24",
        "Reserved 25",
        "Reserved 26",
        "Reserved 27",
        "Reserved 28",
        "Reserved 29",
        "Reserved 30",
        "Reserved 31"
    };

    void(*isrs[])() = {
        exc0, exc1, exc2, exc3, exc4, exc5, exc6, exc7, exc8,
        exc9, exc10, exc11, exc12, exc13, exc14, exc15, exc16,
        exc17, exc18, exc19, exc20, exc21, exc22, exc23, exc24,
        exc25, exc26, exc27, exc28, exc29, exc30, exc31,

        irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8,
        irq9, irq10, irq11, irq12, irq13, irq14, irq15,
    };

    IoPort mPic1Command(PIC1_COMMAND);
    IoPort mPic2Command(PIC2_COMMAND);
    IoPort mPic1Data(PIC1_DATA);
    IoPort mPic2Data(PIC2_DATA);
    IdtGate mIdt[256];
    IdtPtr gIdtPtr;
    // void* mIsrHandlers[70];

    void KERNEL_CALL
    setIdtGate(Byte num, void (*routine)(), Word sel, Byte flags)
    {
        DWord base = reinterpret_cast<Addr>(routine);
        mIdt[num].baseLow = base & 0xFFFF;
        mIdt[num].baseHigh = (base >> 16) & 0xFFFF;
        mIdt[num].sel = sel;
        mIdt[num].flags = flags;
        mIdt[num].always0 = 0x00;
        return;
    }

    void KERNEL_CALL
    isrCommonHandler(Regs* regs)
    {
        // void (*handler)(Regs *regs);

        UInt intNo = regs->intNo;
        // if (intNo != 32) DBG(intNo);
        // handler = mIsrHandlers[intNo];
        if (intNo < IRQ_FIRST_INDEX)
        {
            Out::err() << mExceptionNames[intNo];
            Out::err() << " Exception caught\n";
            // kernel_panic();
        }
        // IRQ
        else if (intNo >= IRQ_FIRST_INDEX && intNo <= IRQ_LAST_INDEX)
        {
            // If the IDT entry that was invoked was greater than 40
            // (meaning IRQ8 - 15), then we need to send an EOI to
            // the slave controller
            if (intNo >= 40)
            {
                mPic2Command.writeByte(PIC_EOI);
            }

            // In either case, we need to send an EOI to the master
            // interrupt controller too
            mPic1Command.writeByte(PIC_EOI);
        }
        else
        {
            Out::warn() << "Unhandled interrupt caught - " << intNo;
        }
        return;
    }

    /*
    static void KERNEL_CALL
    installisrHandler(int exceptionId, void (*handler)(Regs *r))
    {
        mIsrHandlers[exceptionId] = reinterpret_cast<void*>(handler);
    }
    */

    /*
    static void KERNEL_CALL
    irq_install_handler(int aIrq, void (*handler)(regs_t *apR))
    {
        gpIsrRoutines[aIrq + 32] = handler;
    }*/

    // IRQs 0-7 are by default mapped to entries 8-15
    // This conflicts with ISRs so we should remap all IRQs to 32-47
    void KERNEL_CALL
    irqRemap(void)
    {
        // start sequence
        mPic1Command.writeByte(0x11);
        mPic2Command.writeByte(0x11);
        // vectors
        mPic1Data.writeByte(0x20);
        mPic2Data.writeByte(0x28);
        // continue initialization
        mPic1Data.writeByte(0x04);
        mPic2Data.writeByte(0x02);
        // processing mode
        mPic1Data.writeByte(0x01);
        mPic2Data.writeByte(0x01);
        // end
        mPic1Data.writeByte(0x0);
        mPic2Data.writeByte(0x0);
    }

}

void KERNEL_CALL
init()
{
    gIdtPtr.limit = (Word) sizeof(IdtGate) * 256 - 1;
    gIdtPtr.base = (DWord) &mIdt;

    irqRemap();

    unsigned int i;
    for (i = 0; i < sizeof(isrs) / sizeof(void*); i++) {
        setIdtGate(i, isrs[i], kernel_cs_sel, ISR_FLAGS);
    }

    setIdtGate(69, sys69, kernel_cs_sel, ISR_FLAGS);

    load_idt(&gIdtPtr);
    enable_int();
    return;
}


}

