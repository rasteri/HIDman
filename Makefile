BOARD_TYPE = BOARD_AXD
#BOARD_TYPE = BOARD_MINI
#BOARD_TYPE = BOARD_MICRO

#BOARD_OPTIONS = SWAP_KBD_MSC

CC = sdcc
OBJCOPY = sdobjcopy
PACK_HEX = packihx
WCHISP = wchisptool
OBJDIR = ./build

TARGET = hidman

OBJECTS = \
$(OBJDIR)/CH559.rel \
$(OBJDIR)/GPIO.rel \
$(OBJDIR)/KeyboardLed.rel \
$(OBJDIR)/Main.rel \
$(OBJDIR)/ParseDescriptor.rel \
$(OBJDIR)/System.rel \
$(OBJDIR)/Task.rel \
$(OBJDIR)/Timer2.rel \
$(OBJDIR)/UsbHost.rel \
$(OBJDIR)/recvbuffer.rel \
$(OBJDIR)/data.rel \
$(OBJDIR)/menu.rel \
$(OBJDIR)/ps2protocol.rel \
$(OBJDIR)/ps2.rel \
$(OBJDIR)/util.rel \
$(OBJDIR)/andyalloc.rel \
$(OBJDIR)/mouse.rel

ifneq ($(BOARD_TYPE), BOARD_MICRO)
  OBJECTS += $(OBJDIR)/uart.rel
	OBJECTS += $(OBJDIR)/uart1.rel
endif

ifndef FREQ_SYS
FREQ_SYS = 48000000
endif

ifndef XRAM_SIZE
XRAM_SIZE = 0x1600
endif

ifndef XRAM_LOC
XRAM_LOC = 0x0200
endif

ifndef CODE_SIZE
CODE_SIZE = 0xEFFF
endif

CFLAGS := -V -mmcs51 --model-large --stack-auto \
	--xram-size $(XRAM_SIZE) --xram-loc $(XRAM_LOC) \
	--code-size $(CODE_SIZE) \
	-I/ -DFREQ_SYS=$(FREQ_SYS) -D$(BOARD_TYPE) -D$(BOARD_OPTIONS) \
	$(EXTRA_FLAGS)

LFLAGS := $(CFLAGS)

print-%  : ; @echo $* = $($*)

$(OBJDIR)/%.rel : %.c
	$(CC) -c $(CFLAGS) -o $@ $<


$(OBJDIR)/$(TARGET).ihx: $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $(OBJDIR)/$(TARGET).ihx

$(OBJDIR)/$(TARGET).hex: $(OBJDIR)/$(TARGET).ihx
	$(PACK_HEX) $(OBJDIR)/$(TARGET).ihx > $(OBJDIR)/$(TARGET).hex

$(OBJDIR)/$(TARGET).bin: $(OBJDIR)/$(TARGET).ihx
	$(OBJCOPY) -I ihex -O binary $(OBJDIR)/$(TARGET).ihx $(OBJDIR)/$(TARGET).bin

size: $(OBJDIR)/$(TARGET).ihx
	@echo '---------- Segments ----------'
	@egrep '(ABS,CON)|(REL,CON)' $(OBJDIR)/$(TARGET).map | gawk --non-decimal-data '{dec = sprintf("%d","0x" $$2); print dec " " $$0}' | /usr/bin/sort -n -k1 | cut -f2- -d' ' | uniq
	@echo '---------- Memory ----------'
	@egrep 'available|EXTERNAL|FLASH' $(OBJDIR)/$(TARGET).mem

flash: $(OBJDIR)/$(TARGET).bin
	$(WCHISP) -f $(OBJDIR)/$(TARGET).bin -g

.DEFAULT_GOAL := all
all: $(OBJDIR)/$(TARGET).bin $(OBJDIR)/$(TARGET).hex

clean:
	rm -f $(OBJDIR)/*


