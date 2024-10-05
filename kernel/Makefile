# Define compilers for different architectures
x86_64_cxx = /usr/bin/x86_64-linux-gnu-g++-11
aarch64_cxx = /usr/bin/aarch64-linux-gnu-g++-11
loongarch64_cxx = /loongarch64/bin/loongarch64-unknown-linux-gnu-g++
riscv64_cxx = /usr/bin/riscv64-linux-gnu-g++-11

# Define assemblers for different architectures
x86_64_as = /usr/bin/x86_64-linux-gnu-as
aarch64_as = /usr/bin/aarch64-linux-gnu-as
loongarch64_as = /loongarch64/bin/loongarch64-unknown-linux-gnu-as
riscv64_as = /usr/bin/riscv64-linux-gnu-as

# Define linkers for different architectures
x86_64_ld = /usr/bin/x86_64-linux-gnu-ld
aarch64_ld = /usr/bin/aarch64-linux-gnu-ld
loongarch64_ld = /loongarch64/bin/loongarch64-unknown-linux-gnu-ld
riscv64_ld = /usr/bin/riscv64-linux-gnu-ld

# Define source folder and architecture folder
SRC_DIR = src
ARCH_DIR = src/arch

COMMON_CXX_FLAGS= -Wall -O2 -fno-stack-protector -Wextra -ffreestanding -fno-stack-check -fno-lto -fno-PIC -ffunction-sections -fdata-sections -I src/
COMMON_LD_FLAGS= -z max-page-size=0x1000 -nostdlib -static -gc-sections

# Set default architecture to x86_64
ARCH ?= x86_64

# Select compiler, assembler, linker, and flags based on ARCH
ifeq ($(ARCH), x86_64)
    CXX = $(x86_64_cxx)
    AS = $(x86_64_as)
    LD = $(x86_64_ld)
    ARCH_FOLDER = $(ARCH_DIR)/x86_64
    CXXFLAGS = $(COMMON_CXX_FLAGS) -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel # Example x86_64 specific flags
    ASFLAGS =
    LDFLAGS = -m elf_x86_64 $(COMMON_LD_FLAGS) -T x86_64-linker.ld
else ifeq ($(ARCH), aarch64)
    CXX = $(aarch64_cxx)
    AS = $(aarch64_as)
    LD = $(aarch64_ld)
    ARCH_FOLDER = $(ARCH_DIR)/aarch64
    CXXFLAGS = $(COMMON_CXX_FLAGS) -mgeneral-regs-only
    ASFLAGS =
    LDFLAGS = -m aarch64elf $(COMMON_LD_FLAGS) -T aarch64-linker.ld
else ifeq ($(ARCH), loongarch64)
    CXX = $(loongarch64_cxx)
    AS = $(loongarch64_as)
    LD = $(loongarch64_ld)
    ARCH_FOLDER = $(ARCH_DIR)/loongarch64
    CXXFLAGS = $(COMMON_CXX_FLAGS) -march=loongarch64 -mabi=lp64s
    ASFLAGS =
    LDFLAGS = -m elf64loongarch --no-relax $(COMMON_LD_FLAGS) -T loongarch64-linker.ld
else ifeq ($(ARCH), riscv64)
    CXX = $(riscv64_cxx)
    AS = $(riscv64_as)
    LD = $(riscv64_ld)
    ARCH_FOLDER = $(ARCH_DIR)/riscv64
    CXXFLAGS = $(COMMON_CXX_FLAGS) -mabi=lp64 -mno-relax
    ASFLAGS =
    LDFLAGS = -m elf64lriscv $(COMMON_LD_FLAGS) --no-relax -T riscv64-linker.ld
else
    $(error "Unsupported architecture: $(ARCH)")
endif

# Define object file output directory
OBJ_DIR = obj

# Get all .cpp files from src/ excluding src/arch/
SRC_FILES = $(filter-out $(ARCH_DIR)/%, $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp))

# Add all .cpp files from the selected architecture-specific folder
ARCH_FILES = $(wildcard $(ARCH_FOLDER)/**/*.cpp $(ARCH_FOLDER)/*.cpp)

# Find all .S files from the src directory excluding src/arch/
ASM_FILES = $(filter-out $(ARCH_DIR)/%, $(wildcard $(SRC_DIR)/**/*.S $(SRC_DIR)/*.S))

# Add all .S files from the selected architecture-specific folder
ARCH_ASM_FILES = $(wildcard $(ARCH_FOLDER)/**/*.S $(ARCH_FOLDER)/*.S)

# Combine both source, architecture-specific files, and assembly files
ALL_FILES = $(SRC_FILES) $(ARCH_FILES) $(ASM_FILES) $(ARCH_ASM_FILES)

# Object files generated from assembly and C++ files
OBJ_FILES = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES)) \
            $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(ARCH_FILES)) \
            $(patsubst %.S, $(OBJ_DIR)/%.o, $(ASM_FILES)) \
            $(patsubst %.S, $(OBJ_DIR)/%.o, $(ARCH_ASM_FILES))

# Define output binary
OUTPUT = kernel.elf

# Target to compile C++ files and assemble .S files
all: $(OUTPUT)

# Compile C++ files
$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(@D) # Create the required directory for the object file
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Assemble assembly files
$(OBJ_DIR)/%.o: %.S
	mkdir -p $(@D) # Create the required directory for the object file
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files into the final binary
$(OUTPUT): $(OBJ_FILES)
	$(LD) $(LDFLAGS) $^ -o $@

# Clean up build artifacts
clean:
	rm -f $(OUTPUT) $(OBJ_DIR)/*.o
