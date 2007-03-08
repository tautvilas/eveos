#include "mem.h"
#include "idt.h"
#include "stdio.h"

#define NUM_ISRS 32
#define KERNEL_CODE_SEGMENT 0x08
#define IDT_FLAGS 0x8E /* entry is present, ring 0 */

extern void idt_load(); /* assembler fuction with 'lidt' instruction */

/* ISR entries, see kentry.asm for more info */

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* the structure of IDT entry */

typedef struct
{
    word_t base_lo;     /* ISR address low word */
    word_t sel;         /* kernel segment */
    byte_t always0;     /* always set this to 0 */
    byte_t flags;       /* 1 - present?1:0; 2 - ring; 01110 in the end */
    word_t base_hi;     /* ISR address high word */
} __attribute__ ((packed)) idt_entry_t;

/* special pointer to IDT, mentioning its size */

typedef struct
{
    word_t limit;
    dword_t base;
} __attribute__ ((packed)) idt_ptr_t;

/* This defines what the stack looks like after an ISR was running */
typedef struct
{
    dword_t gs, es, fs, ds;                          /* pushed the segs last */
    dword_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    dword_t int_no, err_code;                        /* push byteX and push ecode */
    dword_t eip, cs, eflags, useresp, ss;            /* pushed by the processor automatically */
} regs_t;

idt_ptr_t gIdtp;    /* non static for asm to reach */
static idt_entry_t gIdt[256];
/* ISR debug messages */
static  char* gpExceptionMessages[] =
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

/**
 *  Sets one IDT gate
 *
 *  @param  aNum   gate number
 *  @param  aBase  ISR base address
 *  @param  aSel   kernel segment
 *  @param  aFlags IDT gate flags
 *  @return
 */

static void KERNEL_CALL
idt_set_gate(byte_t aNum, dword_t aBase, byte_t aSel, byte_t aFlags)
{
    gIdt[aNum].base_lo = aBase & 0xFFFF;
    gIdt[aNum].base_hi = (aBase >> 16) & 0xFFFF;
    gIdt[aNum].sel = aSel;
    gIdt[aNum].flags = aFlags;
    gIdt[aNum].always0 = 0x00;
    return;
}

void KERNEL_CALL
idt_install()
{
    gIdtp.limit = sizeof(idt_entry_t) * 256 - 1;
    gIdtp.base = (dword_t) &gIdt;

    memset((byte_t *) &gIdt, 0x00, sizeof(idt_entry_t) * 256);

    idt_set_gate(0, (dword_t)isr0, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(1, (dword_t)isr1, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(2, (dword_t)isr2, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(3, (dword_t)isr3, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(4, (dword_t)isr4, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(5, (dword_t)isr5, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(6, (dword_t)isr6, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(7, (dword_t)isr7, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(8, (dword_t)isr8, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(9, (dword_t)isr9, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(10, (dword_t)isr10, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(11, (dword_t)isr11, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(12, (dword_t)isr12, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(13, (dword_t)isr13, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(14, (dword_t)isr14, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(15, (dword_t)isr15, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(16, (dword_t)isr16, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(17, (dword_t)isr17, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(18, (dword_t)isr18, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(19, (dword_t)isr19, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(20, (dword_t)isr20, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(21, (dword_t)isr21, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(22, (dword_t)isr22, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(23, (dword_t)isr23, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(24, (dword_t)isr24, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(25, (dword_t)isr25, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(26, (dword_t)isr26, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(27, (dword_t)isr27, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(28, (dword_t)isr28, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(29, (dword_t)isr29, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(30, (dword_t)isr30, KERNEL_CODE_SEGMENT, IDT_FLAGS);
    idt_set_gate(31, (dword_t)isr31, KERNEL_CODE_SEGMENT, IDT_FLAGS);

    idt_load();
    return;
}

/**
 *  Manages the exception after ISRs have been called
 *
 *  @param  apRegs pointer to a struct tontaining all register info
 */
void KERNEL_CALL
exception_handler(regs_t * apRegs)
{
    if(apRegs->int_no < 32)
    {
        //int sp;
        /*asm ("movl %%esp, %0\n" :"=r"(sp));
        print_int_dec(sp);
        printf("\n");*/
        printf("pr%dntf %ds l%dt\n", 1, 1, 33, 45, 23, 54, 12, 123, 32);
        //printf("hex value of x = %x\n", 255);
        /*asm ("movl %%esp, %0\n" :"=r"(sp));
        print_int_dec(sp);
        printf("\n"); */
        printf(gpExceptionMessages[apRegs->int_no]);
        printf(" Exception caught\n");
        printf("Kernel panic!");
        for(;;);
    }
}
