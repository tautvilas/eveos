#include <system.h>

/* assembler fuction */

extern void
idt_load();

/* the structure of IDT entry */

typedef struct
{
    word_t base_lo;     /* IDT address low byte */
    word_t sel;
    byte_t always0;     /* always set this to 0 */
    byte_t flags;       /* 1 - present?1:0; 2 - ring; 01110 in the end */
    word_t base_hi;     /* IDT address high byte */
} __attribute__ ((packed)) idt_entry_t;

/* special pointer to IDT, mentioning its size */

typedef struct
{
    word_t limit;
    dword_t base;
} __attribute__ ((packed)) idt_ptr_t;


idt_ptr_t gIdtp;    /* non static for asm */
static idt_entry_t gIdt[256];

static void
idt_set_gate(byte_t num, dword_t base, byte_t sel, byte_t flags)
{
    gIdt[num].base_lo = base & 0xFFFF;
    gIdt[num].base_hi = (base >> 16) & 0xFFFF;
    gIdt[num].sel = sel;
    gIdt[num].flags = flags;
    gIdt[num].always0 = 0x00;
    return;
}

void KERNEL_CALL
idt_install()
{
    gIdtp.limit = sizeof(idt_entry_t) * 256 - 1;
    gIdtp.base = (dword_t) &gIdt;

    memset((byte_t *) &gIdt, 0x00, sizeof(idt_entry_t) * 256);

    idt_set_gate(0, 0, 0, 0);

    idt_load();
    return;
}
