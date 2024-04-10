.global _dtb_ptr
.section .data
_dtb_ptr: .dc.a 0x0

.section ".text.boot"

.global _start
#define CORE0_TIMER_IRQ_CTRL 0x40000040

# https://developer.arm.com/documentation/ddi0500/j/System-Control/AArch64-register-descriptions/Multiprocessor-Affinity-Register
_start:
    ldr x1, =_dtb_ptr
    str x0, [x1]

    mrs x2, mpidr_el1
    and x2, x2, #3
    cbz x2, lab3

halt:
    wfe
    b halt

lab3:
    bl from_el2_to_el1
    adr x0, exception_vector_table
    msr vbar_el1, x0

set_sp:
    ldr x0, = _start
    mov sp, x0
    ldr x0, = __bss_start
    ldr x1, = __bss_size

initialize_bss:
    cbz x1, kernel_main
    str xzr, [x0], #8
    sub x1, x1, #1
    b initialize_bss

kernel_main:
    bl core_timer_enable
    bl main
    b halt

from_el2_to_el1:
    mov x0, (1 << 31)
    msr hcr_el2, x0
    mov x0, 0x3c5 //01111000101 = 965(dec)
    msr spsr_el2, x0 // Saved Program Status Register. M[4:0](el1h), bit 8(System Error disable), bit 7(IRQ disable), bit6(Fast Interrupt Request disable)
    msr elr_el2, lr // exception link register
    eret //use exception return so it will return with the EL1 (specified by the spsr_el2)

core_timer_enable:
    mov x0, 1
    msr cntp_ctl_el0, x0
    mrs x0, cntfrq_el0
    mov x0, 2
    ldr x1, =CORE0_TIMER_IRQ_CTRL
    str x0, [x1]
    ret

.align 11
.global exception_vector_table
exception_vector_table:
    // ---------- el1t ----------
    // Exception from the current EL while using SP_EL0
    .align 7                // Synchronous
    b invalid_exception
    .align 7                // IRQ / vIRQ
    b invalid_exception
    .align 7                // FIQ / vFIQ
    b invalid_exception
    .align 7                // SError
    b invalid_exception

    // ---------- el1h ---------- (EL1 -> EL1)
    // Exception from the current EL while using SP_ELn
    .align 7                // Synchronous
    b sync_el1h
    .align 7                // IRQ / vIRQ
    b irq_el1h
    .align 7                // FIQ / vFIQ
    b invalid_exception
    .align 7                // SError
    b invalid_exception

    // ---------- 64 ---------- (EL0 -> EL1)
    // Exception from a lower EL and at least one lower EL is AArch64 ()
    .align 7                // Synchronous
    b sync_64
    .align 7                // IRQ / vIRQ
    b irq_64
    .align 7                // FIQ / vFIQ
    b invalid_exception
    .align 7                // SError
    b invalid_exception

    // ---------- 32 ----------
    // Exception from a lower EL and all lower ELs are AArch32
    .align 7                // Synchronous
    b invalid_exception
    .align 7                // IRQ / vIRQ
    b invalid_exception
    .align 7                // FIQ / vFIQ
    b invalid_exception
    .align 7                // SError
    b invalid_exception

// save general registers to stack
.macro save_all
    sub sp, sp, 32 * 8
    stp x0, x1, [sp ,16 * 0]
    stp x2, x3, [sp ,16 * 1]
    stp x4, x5, [sp ,16 * 2]
    stp x6, x7, [sp ,16 * 3]
    stp x8, x9, [sp ,16 * 4]
    stp x10, x11, [sp ,16 * 5]
    stp x12, x13, [sp ,16 * 6]
    stp x14, x15, [sp ,16 * 7]
    stp x16, x17, [sp ,16 * 8]
    stp x18, x19, [sp ,16 * 9]
    stp x20, x21, [sp ,16 * 10]
    stp x22, x23, [sp ,16 * 11]
    stp x24, x25, [sp ,16 * 12]
    stp x26, x27, [sp ,16 * 13]
    stp x28, x29, [sp ,16 * 14]
    str x30, [sp, 16 * 15]
.endm

// load general registers from stack
.macro load_all
    ldp x0, x1, [sp ,16 * 0]
    ldp x2, x3, [sp ,16 * 1]
    ldp x4, x5, [sp ,16 * 2]
    ldp x6, x7, [sp ,16 * 3]
    ldp x8, x9, [sp ,16 * 4]
    ldp x10, x11, [sp ,16 * 5]
    ldp x12, x13, [sp ,16 * 6]
    ldp x14, x15, [sp ,16 * 7]
    ldp x16, x17, [sp ,16 * 8]
    ldp x18, x19, [sp ,16 * 9]
    ldp x20, x21, [sp ,16 * 10]
    ldp x22, x23, [sp ,16 * 11]
    ldp x24, x25, [sp ,16 * 12]
    ldp x26, x27, [sp ,16 * 13]
    ldp x28, x29, [sp ,16 * 14]
    ldr x30, [sp, 16 * 15]
    add sp, sp, 32 * 8
.endm

invalid_exception:
    save_all
    mov x10, 0x11
    bl invalid_exception_handler
    load_all
    eret

sync_el1h:
    save_all
    mov x10, 0x12
    bl sync_el1h_handler
    load_all
    eret

irq_el1h:
    save_all
    mov x10, 0x13
    bl irq_el1h_handler
    load_all
    eret

sync_64:
    save_all
    mov x10, 0x14
    bl sync_64_handler
    load_all
    eret

irq_64:
    save_all
    mov x10, 0x15
    bl irq_64_handler
    load_all
    eret