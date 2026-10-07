
ifeq ($(DJGPP),)
$(error ERROR: DJGPP not defined! ***)
endif

RM=rm -f
CP=cp -f
MD=mkdir -p
ZIP=zip -r

CC = $(CROSS_COMPILER)gcc
CFLAGS = -O2 -std=gnu99 -Wall -finline 

all: pll

pll: *.c
	-$(CC) $(CFLAGS) -c *.c

clean:
	-$(RM) *.o

