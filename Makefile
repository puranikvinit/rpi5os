C_FILES = $(wildcard kernel/src/*.c)
C_OBJ_FILES = $(C_FILES:.c=.o)

ASM_FILES = $(wildcard kernel/src/*.S)
ASM_OBJ_FILES = $(ASM_FILES:.S=.o)

LLVM_PATH = /opt/homebrew/opt/llvm/bin/
CLANG_FLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a76+nosimd -Ikernel/include


# Default target
all: clean kernel8.img

# Compile the boot.S file into an object file
boot.o: boot.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the C files into object files
$(C_OBJ_FILES): %.o: %.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the ASSEMBLY files into object files
$(ASM_OBJ_FILES): %.o: %.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile the kernel entry-point function file into object file
kernel.o: kernel/kernel.c
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# 1. Link all the object files into a single ELF file
# 2. Convert the ELF file into a binary file
kernel8.img: boot.o $(ASM_OBJ_FILES) $(C_OBJ_FILES) kernel.o
	$(LLVM_PATH)ld.lld -m aarch64elf -nostdlib boot.o $(ASM_OBJ_FILES) $(C_OBJ_FILES) kernel.o -T linker.ld -o kernel8.elf
	$(LLVM_PATH)llvm-objcopy -O binary kernel8.elf kernel8.img

# clean previous build and residual files
clean:
	/bin/rm -f *.o *.elf *.img kernel/src/*.o > /dev/null 2> /dev/null || true
