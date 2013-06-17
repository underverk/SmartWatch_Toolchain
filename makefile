ADDITIONALDEFINES = 

BUILDPREFIX= arm-none-eabi
CPUCLASS=STM32
MEMORY_TARGET ?= flash

MCU_FAMILY    := STM32F2
MCU           := STM32F205RGY6
HSE_FREQUENCY := 26000000
MCU_OPTIONS   := -mcpu=cortex-m3 -mthumb

LDFLAGS = -Tflash.ld -Xlinker --gc-sections -lc -nostartfiles

DEFINECPU = $(MCU_OPTIONS) -DMCU_$(MCU) -D$(MCU_FAMILY) -fno-exceptions 
EXTRACPPFLAGS = -fno-rtti
RUNTIMELIB = 
EXTRAINCDIRS = $(SRCDIR) $(SRCDIR)/libstm32f2

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Object files directory
#     To put object files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
OBJDIRBASE = bin
OBJDIR = $(OBJDIRBASE)

# Target file name (without extension).
TARGET = $(OBJDIR)/firmware

# List C source files here. (C dependencies are automatically generated.)
SRC = $(SRCDIR)/driver_display.c \
      $(SRCDIR)/driver_i2c.c \
      $(SRCDIR)/driver_power.c \
      $(SRCDIR)/driver_adc.c \
      $(SRCDIR)/driver_rtc.c \
      $(SRCDIR)/pins.c \
      $(SRCDIR)/libstm32f2/stm32f2xx_adc.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_can.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_crc.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_cryp.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_cryp_aes.c  \
      $(SRCDIR)/libstm32f2/stm32f2xx_cryp_des.c  \
      $(SRCDIR)/libstm32f2/stm32f2xx_cryp_tdes.c \
      $(SRCDIR)/libstm32f2/stm32f2xx_dac.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_dbgmcu.c    \
      $(SRCDIR)/libstm32f2/stm32f2xx_dcmi.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_dma.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_exti.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_flash.c     \
      $(SRCDIR)/libstm32f2/stm32f2xx_fsmc.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_gpio.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_hash.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_hash_md5.c  \
      $(SRCDIR)/libstm32f2/stm32f2xx_hash_sha1.c \
      $(SRCDIR)/libstm32f2/stm32f2xx_i2c.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_iwdg.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_misc.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_pwr.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_rcc.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_rng.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_rtc.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_sdio.c      \
      $(SRCDIR)/libstm32f2/stm32f2xx_spi.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_syscfg.c    \
      $(SRCDIR)/libstm32f2/stm32f2xx_tim.c       \
      $(SRCDIR)/libstm32f2/stm32f2xx_usart.c     \
      $(SRCDIR)/libstm32f2/stm32f2xx_wwdg.c      \
      $(SRCDIR)/libstm32f2/system_stm32f2xx.c    \
      $(SRCDIR)/libstm32f2/syscalls.c

LIBDIR     = libraries
SRCDIR = src

# List C++ source files here. (C dependencies are automatically generated.)
CPPSRC = $(SRCDIR)/main.cpp

# List Assembler source files here.
#     Make them always end in a capital .S.  Files ending in a lowercase .s
#     will not be considered source files but generated files (assembler
#     output from the compiler), and will be deleted upon "make clean"!
#     Even though the DOS/Win* filesystem matches both .s and .S the same,
#     it will preserve the spelling of the filenames, and gcc itself does
#     care about how the name is spelled on its command-line.
ASRC =  $(SRCDIR)/libstm32f2/startup_stm32f2xx.S


# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
OPT = s


# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
DEBUG = dwarf-2



# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99


# Place -D or -U options here for C sources
CDEFS = -DHSE_VALUE=$(HSE_FREQUENCY)UL -DUSE_STDPERIPH_DRIVER
CDEFS += $(ADDITIONALDEFINES)

# Place -D or -U options here for ASM sources
ADEFS = -DHSE_VALUE=$(HSE_FREQUENCY)


# Place -D or -U options here for C++ sources
CPPDEFS = -DHSE_VALUE=$(HSE_FREQUENCY)UL
CPPDEFS += $(ADDITIONALDEFINES)
#CPPDEFS += -D__STDC_LIMIT_MACROS
#CPPDEFS += -D__STDC_CONSTANT_MACROS



#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fsingle-precision-constant
#CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wwrite-strings
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare

#CFLAGS += -mcall-prologues

CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)


#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual 
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS = -g$(DEBUG)
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -ffunction-sections
CPPFLAGS += -fdata-sections
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fsingle-precision-constant
CPPFLAGS += -fshort-enums
CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
CPPFLAGS += -Wundef
CPPFLAGS += -Wa,-adhlns=$(<:%.cpp=$(OBJDIR)/%.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))


#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -adhlns:   create listing
#  -gstabs:   have the assembler create line number information
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex 
#       dump that will be displayed for a given single line of source input.
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:%.S=$(OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100


#============================================================================

# Define programs and commands.
SHELL = sh
CC = $(BUILDPREFIX)-gcc
OBJCOPY = $(BUILDPREFIX)-objcopy
DFUUTIL = dfu-util
OBJDUMP = $(BUILDPREFIX)-objdump
SIZE = $(BUILDPREFIX)-size
AR = $(BUILDPREFIX)-ar rcs
NM = $(BUILDPREFIX)-nm
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
WINSHELL = cmd
TOUCH = touch


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:




# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o) 

# Define all listing files.
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst) 

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = $(DEFINECPU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = $(DEFINECPU) -I. -x c++ $(CPPFLAGS) $(EXTRACPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = $(DEFINECPU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: sizebefore touchmain build sizeafter

sketch: all

# Force recompile of src/main.cpp
touchmain:
	@$(REMOVE) $(OBJDIR)/src/main.o $@
	@$(REMOVE) $(OBJDIR)/*.* $@

build: elf bin hex lss sym

elf: $(TARGET).elf
bin: $(TARGET).bin
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym
lib: $(LIBNAME)

program:
	@echo $(MSG_FLASH) $@
	@$(DFUUTIL) -d 0FCE:F0FA -a 0 -i 0 -s 0x08040000 -D $(TARGET).bin $@
	@$(DFUUTIL) -d 0FCE:F0FA -a 0 -i 0 -s 0x080FFFFC -D magic.bin $@

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); 2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); 2>/dev/null; echo; fi


# Create final output files (.hex, .eep) from ELF output file.
%.bin: %.elf
	@echo $(MSG_FLASH) binary $@
	@$(OBJCOPY) -v -Obinary $< $@

%.hex: %.elf
	@echo $(MSG_FLASH) $@
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo $(MSG_EXTENDED_LISTING) $@
	@$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo $(MSG_SYMBOL_TABLE) $@
	@$(NM) -n $< > $@


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ) $(RUNTIMELIB)
	@echo $(MSG_LINKING) $@
	@$(CC) $(ALL_CFLAGS) $^ $(RUNTIMELIB) --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo $(MSG_COMPILING) $<
	@$(CC) -c $(ALL_CFLAGS) $(abspath $<) -o $@ $(VS_TRIM_ERRORS)

# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp
	@echo $(MSG_COMPILING_CPP) $<
	@$(CC) -c $(ALL_CPPFLAGS) $(abspath $<) -o $@ $(VS_TRIM_ERRORS)

# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : %.S
	@echo $(MSG_ASSEMBLING) $<
	@$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo $(MSG_CLEANING)
	-$(REMOVEDIR) $(OBJDIRBASE)

# Create object files directory
$(shell mkdir -p $(OBJDIR)/$(SRCDIR) 2>/dev/null)
$(shell mkdir -p $(OBJDIR)/$(SRCDIR)/libstm32f2 2>/dev/null)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter build elf hex eep lss sym clean clean_list program touchmain


