PROG = memory-heirarchy
OFILES = \
	src/startup.o src/main.o src/test.o src/test-asm.o \
	src/uart.o src/perfcount.o \
	src/mmu.o

CC = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy

ASFLAGS =
CFLAGS = \
	-O2 -Wall -Wextra -Werror \
	-Iinclude/ \
	-ffreestanding -nostdlib \
	-ffunction-sections -fdata-sections \
	-mcpu=arm1176jzf-s
LDFLAGS = -nostdlib -T aux/ldscript

.PHONY: all
all: $(PROG).bin

.PHONY: clean
clean:
	rm -f $(PROG).bin $(PROG).elf $(OFILES) $(DFILES)

$(PROG).bin: $(PROG).elf
	$(OC) -O binary $< $@

$(PROG).elf: $(OFILES)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

DFILES = $(OFILES:.o=.d)
-include $(DFILES)
