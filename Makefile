CC = sdcc
OBJCOPY = sdobjcopy
PACK_HEX = packihx
WCHISP = wchisptool
OBJDIR = ./build

TARGET = hidman

OBJECTS = \
$(OBJDIR)/ch559.rel \
$(OBJDIR)/gpio.rel \
$(OBJDIR)/keyboardled.rel \
$(OBJDIR)/main.rel \
$(OBJDIR)/parsedescriptor.rel \
$(OBJDIR)/system.rel \
$(OBJDIR)/task.rel \
$(OBJDIR)/timer2.rel \
$(OBJDIR)/usbhost.rel \
$(OBJDIR)/recvbuffer.rel \
$(OBJDIR)/data.rel \
$(OBJDIR)/menu.rel \
$(OBJDIR)/ps2protocol.rel \
$(OBJDIR)/ps2.rel \
$(OBJDIR)/util.rel \
$(OBJDIR)/andyalloc.rel \
$(OBJDIR)/uart.rel \
$(OBJDIR)/uart1.rel \
$(OBJDIR)/mouse.rel 



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

CFLAGS := -V -mmcs51 --debug --model-large --stack-auto \
	--xram-size $(XRAM_SIZE) --xram-loc $(XRAM_LOC) \
	--code-size $(CODE_SIZE) \
	-I/ -DFREQ_SYS=$(FREQ_SYS) \
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

flash: $(OBJDIR)/$(TARGET).bin pre-flash
	$(WCHISP) -f $(OBJDIR)/$(TARGET).bin -g

.DEFAULT_GOAL := all
all: $(OBJDIR)/$(TARGET).bin $(OBJDIR)/$(TARGET).hex

clean:
	rm -f $(OBJDIR)/*

test: $(OBJECTS) $(OBJDIR)/test.rel
	$(CC) build/test.rel build/parsedescriptor.rel build/andyalloc.rel build/data.rel build/uart.rel $(LFLAGS) -o $(OBJDIR)/test.ihx
	s51 build/test -s -