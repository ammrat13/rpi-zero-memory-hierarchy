    .section ".text.mmu_init", "ax"
    .global mmu_init
mmu_init:

    # Start by invalidating all of the caches since they might have bad data
    # from the previous boot.
.Lmmu_invalidate_caches:
    # Invalidate the TLB
    mcr p15, 0, r0, c8, c7, 0
    # Invalidate the data cache
    mcr p15, 0, r0, c7, c6, 0
    # Invalidate the instruction cache. This also flushes the branch target
    # buffer. There appears to be an errata for this device, so we have to go
    # through this long sequence
    .rept 4
        mcr p15, 0, r0, c7, c5, 0
    .endr
    .rept 11
        nop
    .endr
    # Perform a DSB and a prefetch flush so that the changes become visible.
    # We'll do the prefetch flush later anyway.
    mcr p15, 0, r0, c7, c10, 4

.Lmmu_set_xp:
    # Set the XP bit in the CR1. This way, we can use the ARMv6 translation
    # table entries. Also set round-robin replacement instead of random
    # replacement. As with all CP15 registers, we have to prefetch flush.
    mrc p15, 0, r0, c1, c0, 0
    orr r0, #(1 << 23)
    orr r0, #(1 << 14)
    mcr p15, 0, r0, c1, c0, 0
    mcr p15, 0, r0, c7, c5, 4

    bx lr

    .section ".text.mmu_set_attributes", "ax"
    .global mmu_set_attributes
mmu_set_attributes:
    # Set the TTBR0 and DAC registers, then prefetch flush since we wrote to a
    # CP15 register. Also DSB so that the page table walker can actually see the
    # data.
    mcr p15, 0, r0, c2, c0, 0
    mcr p15, 0, r1, c3, c0, 0
    mcr p15, 0, r0, c7, c10, 4
    mcr p15, 0, r0, c7, c5, 4
    bx lr

    .section ".text.mmu_enable", "ax"
    .global mmu_enable
mmu_enable:

    # RMW the control register. This enables the caches, the branch predictor,
    # and the MMU.
    mrc p15, 0, r0, c1, c0, 0
    orr r0, #(0b11 << 11)
    orr r0, #(0b101 << 0)
    mcr p15, 0, r0, c1, c0, 0

    # DSB so that future instructions see the translation
    mcr p15, 0, r0, c7, c10, 4

    # Flush the branch predictor since we just enabled the MMU, then prefetch
    # flush so that it becomes visible.
    mcr p15, 0, r0, c7, c5, 6
    mcr p15, 0, r0, c7, c5, 4

    bx lr
