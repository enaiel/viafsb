
ifeq ($(DJGPP),)
$(error ERROR: DJGPP not defined! ***)
endif

RM=rm -f
CP=cp -f
MD=mkdir -p
ZIP=zip -r

CC = $(CROSS_COMPILER)gcc
VERFILE=../VERSION
VER=$(strip $(shell cat $(VERFILE)))
CFLAGS = -DVERSION=\"$(VER)\" -O2 -std=gnu99 -Wall -finline 
LDFLAGS = -lm
OBJS=viafsb.o pci.o smb.o log.o
PLLOBJS=pll/*.o
PROG=viafsb
TARGET=../$(PROG).exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(MAKE) -C pll all
	-$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(PLLOBJS) $(LDFLAGS)

%.o: %.c
	-$(CC) $(CFLAGS) -c $^

clean:
	-$(RM) $(TARGET)
	-$(RM) *.o
	-$(MAKE) -C pll clean

