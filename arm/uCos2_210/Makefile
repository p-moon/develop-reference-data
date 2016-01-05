#======================================================================
# File Name  : Makefile
# Function   : Makefile for GNU make
# Author     : Kim Siyoung @ System LSI, Samsung Electronics.
#======================================================================
# $@ - the target name
# $* - the stem of the target name ("dir/foo.c" -> "dir/foo.e")
# $< - the name of the first dependency
# $(@F) - the target name with path = $(notdir $@)
# $(notdir $*.e) - ("dir/foo.c" -> "foo.e")
# $^ - the names of all the dependencies
#======================================================================

# Project name
PRJ = uCos2_210

# Head file directory
INC = Printf,\
	-IS5PV210 \
	-IS5PV210\include \
	-IS5PV210\source \
	-IuCOS_II\api \
	-IuCOS_II\arm \
	-IuCOS_II\source \
	-IuCOS_II\source\api 
	
LIBS = 
INIT_DRAM = S5PV210\source\210init.s

# Destination path Definition
OBJ=.\obj
ERR=.\err

# Source file
SRC = \
	Printf\Printf.c \
	S5PV210\source\main.c \
	S5PV210\source\6410slib.s \
	S5PV210\source\mmu.c \
	S5PV210\source\timer.c \
	S5PV210\source\uart.c \
	S5PV210\source\int.c \
	S5PV210\source\clock.c \
	S5PV210\source\led.c \
	S5PV210\source\key.c \
	S5PV210\source\font_libs.c \
	S5PV210\source\lcd.c \
	S5PV210\source\rtc.c \
	S5PV210\source\one_wire.c \
	S5PV210\source\target.c \
	uCOS_II\arm\Os_cpu_a.s \
	uCOS_II\arm\Os_cpu_c.c \
	uCOS_II\source\os_core.c \
	uCOS_II\source\os_dbg_r.c \
	uCOS_II\source\os_flag.c \
	uCOS_II\source\os_mbox.c \
	uCOS_II\source\os_mem.c \
	uCOS_II\source\os_mutex.c \
	uCOS_II\source\os_q.c \
	uCOS_II\source\os_sem.c \
	uCOS_II\source\os_task.c \
	uCOS_II\source\os_time.c \
	uCOS_II\source\os_tmr.c \
	uCOS_II\source\api\os_api.c 

# Objects
OBJS = \
	$(patsubst %.c, %.o, $(filter %.c, $(SRC))) \
	$(patsubst %.cpp, %.o, $(filter %.cpp, $(SRC))) \
	$(patsubst %.s, %.o, $(filter %.s, $(SRC)))

# Options
CPU = CORTEX-A8
AFLAGS = --cpu=$(CPU) --debug 
CFLAGS = --cpu=$(CPU) --debug -c -W -O0 -I$(INC)
LFLAGS_DRAM = --datacompressor=off --debug  --ro-base 0x20000000 --first 210init.o(Init) --entry 210init.o(Init)

# Tools
TOOLS =
ASM   = $(TOOLS)armasm
CC    = $(TOOLS)armcc
CPP   = $(TOOLS)armcpp
LINK  = $(TOOLS)armlink
ELF   = $(TOOLS)fromelf

# Targets
.SUFFIXES: .axf .map .bin .sym .lst

all: test
test: $(PRJ).axf

clean:
	del *.o *.bak *~ *.d *.bin *.axf *.sym *.lst *.map /s

$(PRJ).axf: $(INIT_DRAM:.s=.o) $(OBJS)
	$(LINK) $(LFLAGS_DRAM) $(LIBS) -o $@ --map --list $*.map $^ 
	$(ELF)  $@ --text -s -o $*.sym
	$(ELF)  $@ --text -c -o $*.lst
	$(ELF) --bin -o $*.bin $@

%.o: %.cpp
	$(CPP) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC)  $(CFLAGS) $< -o $@

%.o: %.s
	$(ASM) $(AFLAGS) $< -o $@


dep: $(PRJ).dep

ifeq ($(PRJ).dep, $(findstring $(PRJ).dep, $(wildcard *.dep)))
include $(PRJ).dep
else
$(PRJ).dep: $(OBJS:.o=.d)
	copy $(subst +$(firstword $^),$(firstword $^),$(patsubst %.d,+%.d,$^)) $(PRJ).dep

%.d: %.cpp
	$(CC) $(CFLAGS) -M $< -o $*.o > $@

%.d: %.c
	$(CC) $(CFLAGS) -M $< -o $*.o > $@

%.d: %.s
	$(ASM) $(AFLAGS) -m $< -o $*.o > $@
endif

