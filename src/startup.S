    .section ".startup", "ax"
    .global _start
_start:

    # Switch to supervisor mode with all interrupts disabled. Remember to
    # prefetch flush after this.
    mov r0, #0
    msr cpsr_c, #0b11010011
    mcr p15, 0, r0, c7, c5, 4

    # Setup the stack
    mov fp, #0
    mov sp, #0x100000

    # Zero out the .bss section. We can use the frame pointer since we zeroed it
    # out just now.
    ldr r0, =__bss_start
    ldr r1, =__bss_end
.Lbss_zero_loop:
    cmp r0, r1
    strne fp, [r0], #4
    bne   .Lbss_zero_loop

    # Call main
    bl main

    # If main ever returns, loop forever
    .global _end
_end:
    wfi
    b _end
