C_FILES = $(wildcard kernel/*.c)
OBJ_FILES = $(C_FILES:.c=.o)

LLVM_PATH = /opt/homebrew/opt/llvm/bin/
CLANG_FLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a76+nosimd


# Default target
all: clean kernel8.img

# Compile the boot.S file into an object file
boot.o: boot.S
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# Compile all the C files into object files
$(OBJ_FILES): $(C_FILES)
	$(LLVM_PATH)clang --target=aarch64-elf $(CLANG_FLAGS) -c $< -o $@

# 1. Link all the object files into a single ELF file
# 2. Convert the ELF file into a binary file
kernel8.img: boot.o $(OBJ_FILES)
	$(LLVM_PATH)ld.lld -m aarch64elf -nostdlib boot.o $(OBJ_FILES) -T linker.ld -o kernel8.elf
	$(LLVM_PATH)llvm-objcopy -O binary kernel8.elf kernel8.img

# clean previous build and residual files
clean:
	/bin/rm -f *.o *.elf *.img kernel/*.o > /dev/null 2> /dev/null || true
