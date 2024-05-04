    .section ".text.run_test_asm", "ax"
    .global run_test_asm
run_test_asm:

    # Stack setup
    push {r4-r6}

    # Put the address of the timer peripheral into a register so we don't have
    # to keep loading it.
    mov r6, #0x20000000
    orr r6, #0x3000

    # Have r4 keep the index into the array
    mov r4, #0

    # Remember the initial value of the timer
    ldr r5, [r6, #4]

.Ltest_loop:
    # Decrement the counter. If we hit zero, we're done
    subs r3, #1
    beq  .Ltest_loop_end
    # Load using r4 as the offset, then increment by the stride under the mask
    ldr ip, [r0, r4]
    add r4, r2
    and r4, r1
    b .Ltest_loop

.Ltest_loop_end:
    # Get the new timer, and calculate how long we took
    ldr r0, [r6, #4]
    sub r0, r5

    # Stack teardown
    pop {r4-r6}
    bx lr
