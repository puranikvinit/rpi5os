INTERRUPTS_C_FILES = $(wildcard kernel/src/interrupts/*.c)
INTERRUPTS_C_OBJ_FILES = $(INTERRUPTS_C_FILES:.c=.o)

HANDLER_C_FILES = $(wildcard kernel/src/interrupts/handlers/*.c)
HANDLER_C_OBJ_FILES = $(HANDLER_C_FILES:.c=.o)

HANDLER_ASM_FILES = $(wildcard kernel/src/interrupts/handlers/*.S)
HANDLER_ASM_OBJ_FILES = $(HANDLER_ASM_FILES:.S=.o)

INTERRUPTS_ASM_FILES = $(wildcard kernel/src/interrupts/*.S)
INTERRUPTS_ASM_OBJ_FILES = $(INTERRUPTS_ASM_FILES:.S=.o)


PERIPHERALS_C_FILES = $(wildcard kernel/src/peripherals/*.c)
PERIPHERALS_C_OBJ_FILES = $(PERIPHERALS_C_FILES:.c=.o)

PERIPHERALS_ASM_FILES = $(wildcard kernel/src/peripherals/*.S)
PERIPHERALS_ASM_OBJ_FILES = $(PERIPHERALS_ASM_FILES:.S=.o)


UTIL_C_FILES = $(wildcard kernel/src/util/*.c)
UTIL_C_OBJ_FILES = $(UTIL_C_FILES:.c=.o)

UTIL_ASM_FILES = $(wildcard kernel/src/util/*.S)
UTIL_ASM_OBJ_FILES = $(UTIL_ASM_FILES:.S=.o)


SCHEDULER_C_FILES = $(wildcard kernel/src/scheduler/*.c)
SCHEDULER_C_OBJ_FILES = $(SCHEDULER_C_FILES:.c=.o)

SCHEDULER_ASM_FILES = $(wildcard kernel/src/scheduler/*.S)
SCHEDULER_ASM_OBJ_FILES = $(SCHEDULER_ASM_FILES:.S=.o)


SYSCALLS_C_FILES = $(wildcard kernel/src/sys_calls/*.c)
SYSCALLS_C_OBJ_FILES = $(SYSCALLS_C_FILES:.c=.o)

SYSCALLS_ASM_FILES = $(wildcard kernel/src/sys_calls/*.S)
SYSCALLS_ASM_OBJ_FILES = $(SYSCALLS_ASM_FILES:.S=.o)


TEST_C_FILES = $(wildcard test/src/*.c)
TEST_C_OBJ_FILES = $(TEST_C_FILES:.c=.o)

TEST_ASM_FILES = $(wildcard test/src/*.S)
TEST_ASM_OBJ_FILES = $(TEST_ASM_FILES:.S=.o)


LLVM_PATH = /opt/homebrew/opt/llvm/bin/
CLANG_FLAGS = -Wall -g -O2 -ffreestanding -nostdinc -nostdlib -mstack-alignment=4 -mcpu=cortex-a76+nosimd -Ikernel/include -Itest/include -I /opt/homebrew/Cellar/llvm/18.1.6/lib/clang/18/include/


# Default target
all: clean kernel8.img

# Compile the boot.S file into an object file
boot.o: boot.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the Interrupts C files into object files
$(INTERRUPTS_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the Interrupts ASSEMBLY files into object files
$(INTERRUPTS_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the Interrupts Handler C files into object files
$(HANDLER_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the Interrupts Handler ASSEMBLY files into object files
$(HANDLER_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the IO C files into object files
$(PERIPHERALS_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the IO ASSEMBLY files into object files
$(PERIPHERALS_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the UTIL C files into object files
$(UTIL_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the UTIL ASSEMBLY files into object files
$(UTIL_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the SCHEDULER C files into object files
$(SCHEDULER_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the SCHEDULER ASSEMBLY files into object files
$(SCHEDULER_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the SYSCALLS C files into object files
$(SYSCALLS_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the SYSCALLS ASSEMBLY files into object files
$(SYSCALLS_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the TEST C files into object files
$(TEST_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the TEST ASSEMBLY files into object files
$(TEST_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@


# Compile the mmio.S file into an object file
mmio.o: kernel/src/mmio.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile the kernel entry-point function file into object file
kernel.o: kernel/kernel.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# 1. Link all the object files into a single ELF file
# 2. Convert the ELF file into a binary file
kernel8.img: boot.o mmio.o $(PERIPHERALS_C_OBJ_FILES) $(PERIPHERALS_ASM_OBJ_FILES) $(UTIL_C_OBJ_FILES) $(UTIL_ASM_OBJ_FILES) $(HANDLER_C_OBJ_FILES) $(HANDLER_ASM_OBJ_FILES) $(INTERRUPTS_C_OBJ_FILES) $(INTERRUPTS_ASM_OBJ_FILES) $(SCHEDULER_C_OBJ_FILES) $(SCHEDULER_ASM_OBJ_FILES) $(SYSCALLS_C_OBJ_FILES) $(SYSCALLS_ASM_OBJ_FILES) $(TEST_C_OBJ_FILES) $(TEST_ASM_OBJ_FILES) kernel.o
	$(LLVM_PATH)ld.lld -m aarch64elf -nostdlib boot.o mmio.o $(PERIPHERALS_C_OBJ_FILES) $(PERIPHERALS_ASM_OBJ_FILES) $(UTIL_C_OBJ_FILES) $(UTIL_ASM_OBJ_FILES) $(HANDLER_C_OBJ_FILES) $(HANDLER_ASM_OBJ_FILES) $(INTERRUPTS_C_OBJ_FILES) $(INTERRUPTS_ASM_OBJ_FILES) $(SCHEDULER_C_OBJ_FILES) $(SCHEDULER_ASM_OBJ_FILES) $(SYSCALLS_C_OBJ_FILES) $(SYSCALLS_ASM_OBJ_FILES) $(TEST_C_OBJ_FILES) $(TEST_ASM_OBJ_FILES) kernel.o -T linker.ld -o kernel8.elf
	$(LLVM_PATH)llvm-objcopy -O binary kernel8.elf kernel8.img

# clean previous build and residual files
clean:
	/bin/rm -f *.o *.elf *.img kernel/src/*.o kernel/src/interrupts/*.o kernel/src/interrupts/handlers/*.o kernel/src/peripherals/*.o kernel/src/util/*o kernel/src/scheduler/*.o kernel/src/sys_calls/*.o > /dev/null 2> /dev/null || true
