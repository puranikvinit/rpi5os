INTERRUPTS_C_FILES = $(wildcard kernel/src/interrupts/*.c)
INTERRUPTS_C_OBJ_FILES = $(INTERRUPTS_C_FILES:.c=.o)

INTERRUPTS_ASM_FILES = $(wildcard kernel/src/interrupts/*.S)
INTERRUPTS_ASM_OBJ_FILES = $(INTERRUPTS_ASM_FILES:.S=.o)


IO_C_FILES = $(wildcard kernel/src/io/*.c)
IO_C_OBJ_FILES = $(IO_C_FILES:.c=.o)

IO_ASM_FILES = $(wildcard kernel/src/io/*.S)
IO_ASM_OBJ_FILES = $(IO_ASM_FILES:.S=.o)


UTIL_C_FILES = $(wildcard kernel/src/util/*.c)
UTIL_C_OBJ_FILES = $(UTIL_C_FILES:.c=.o)

UTIL_ASM_FILES = $(wildcard kernel/src/util/*.S)
UTIL_ASM_OBJ_FILES = $(UTIL_ASM_FILES:.S=.o)


LLVM_PATH = /opt/homebrew/opt/llvm/bin/
CLANG_FLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a76+nosimd -Ikernel/include


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

# Compile all the IO C files into object files
$(IO_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the IO ASSEMBLY files into object files
$(IO_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the UTIL C files into object files
$(UTIL_C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the UTIL ASSEMBLY files into object files
$(UTIL_ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile the kernel entry-point function file into object file
kernel.o: kernel/kernel.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# 1. Link all the object files into a single ELF file
# 2. Convert the ELF file into a binary file
kernel8.img: boot.o $(IO_C_OBJ_FILES) $(IO_ASM_OBJ_FILES) $(UTIL_C_OBJ_FILES) $(UTIL_ASM_OBJ_FILES) $(INTERRUPTS_C_OBJ_FILES) $(INTERRUPTS_ASM_OBJ_FILES) kernel.o
	$(LLVM_PATH)ld.lld -m aarch64elf -nostdlib boot.o $(IO_C_OBJ_FILES) $(IO_ASM_OBJ_FILES) $(UTIL_C_OBJ_FILES) $(UTIL_ASM_OBJ_FILES) $(INTERRUPTS_C_OBJ_FILES) $(INTERRUPTS_ASM_OBJ_FILES) kernel.o -T linker.ld -o kernel8.elf
	$(LLVM_PATH)llvm-objcopy -O binary kernel8.elf kernel8.img

# clean previous build and residual files
clean:
	/bin/rm -f *.o *.elf *.img kernel/src/*.o kernel/src/interrupts*.o kernel/src/io/*.o kernel/src/util/*o > /dev/null 2> /dev/null || true
