## Introduction ##

Interrupts and exceptions are closely related in Intel architecture and their handling is very similar.

The difference between interrupts and exceptions is that **interrupts are used to handle asynchronous events** external to the processor, but **exceptions handle conditions detected by the processor itself** in the course of executing instructions.

There are two sources of **interrupts**:
  1. **Maskable** interrupts, which are signalled via the INTR pin.
  1. **Nonmaskable** interrupts, which are signalled via the NMI (Non-Maskable Interrupt) pin.

There are two sources of **exceptions**:
  1. Processor **detected** (They can also be classified into faults, traps and aborts)
  1. **Programmed** (The exceptions can be trigerred by INT 0, INT 2, INT n commands)

There are three types of exceptions:
  1. **Faults** are exceptions that are reported **"before" the instruction** causing the exception. **Permits restart** of the instruction.
  1. **Traps** are exceptions that are reported immediately **after the instruction** in which the exception was detected.
  1. **Aborts** are exceptions that permits **neither precise location nor restart** of the program that caused the exception. Aborts are used to report severe errors, such as hardware errors and inconsistent or illegal values in system tables.


## Terminology ##

ISR - Interrupt Service Routine: The code that is executed when interrupt happens.

IRQ - Interrupt request: The hardware interrupt, controlled by PIC.

PIC - Programmable interrupt controller: It is a chip that manages IRQs. Also it can be programmed to assign IRQs the Interrupt numbers the user wants.

NMI - Non maskable interrupt

IDT - Interrupt descriptor table

_Note: For me(zv) there is some confusion about all this terminology. For example: is the code executed when exception happens an ISR or not? The exception is not classified as interrupt (see introduction), but also can be executed by INT command and the execution code is reference in IDT (interrupt descriptor table)._

## Interrupt descriptor table ##

Assembler command LIDT loads address of IDT descriptor to IDT register. This descriptor is pointing to IDT. IDT has several gates. Each gate has a descriptor which points to particular ISR.

```

                                              INTERRUPT DESCRIPTOR TABLE
                                              +------+-----+-----+------+
                                        +---->|      |     |     |      |
                                        |     |- GATE FOR INTERRUPT #N -|
                                        |     |      |     |     |      |
                                        |     +------+-----+-----+------+
                                        |     *                         *
                                        |     *                         *
                                        |     *                         *
                                        |     +------+-----+-----+------+
                                        |     |      |     |     |      |
                                        |     |- GATE FOR INTERRUPT #2 -|
                                        |     |      |     |     |      |
                                        |     |------+-----+-----+------|
            IDT REGISTER                |     |      |     |     |      |
                                        |     |- GATE FOR INTERRUPT #1 -|
                    15            0     |     |      |     |     |      |
                   +---------------+    |     |------+-----+-----+------|
                   |   IDT LIMIT   |----+     |      |     |     |      |
  +----------------+---------------|          |- GATE FOR INTERRUPT #0 -|
  |            IDT BASE            |--------->|      |     |     |      |
  +--------------------------------+          +------+-----+-----+------+
   31                             0
```

IDT can contain:

  * Trap gates
  * Task gates
  * Interrupt gates

Only interrupt gates will be covered in this document.

The structure of an interrupt gate descriptor:
```
                              80386 INTERRUPT GATE
   31                23                15                7                0
  +-----------------+-----------------+---+---+---------+-----+-----------+
  |           OFFSET 31..16           | P |DPL|0 1 1 1 0|0 0 0|(NOT USED) |4
  |-----------------------------------+---+---+---------+-----+-----------|
  |             SELECTOR              |           OFFSET 15..0            |0
  +-----------------+-----------------+-----------------+-----------------+
```

P - is the entry present?

DPL - execution ring

References:
  1. [Intel 80386 Programmer's Reference Manual: 9 Interrupts and exceptions](http://www.logix.cz/michal/doc/i386/chp09-00.htm)