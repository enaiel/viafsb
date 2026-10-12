
ifeq ($(DJGPP),)
$(error ERROR: DJGPP not defined! ***)
endif

RM=rm -f
CP=cp -f
MD=mkdir -p
ZIP=zip -r

CC = $(CROSS_COMPILER)gcc
CFLAGS = -O2 -std=gnu99 -Wall -finline 
SRCS = alg1.c cy28316.c ics9148-37.c ics9248-127.c ics94211.c ics94215.c \
       ics94241.c ics950405.c ics950908.c pll205-03.c pllname.c w124.c \
       w156c.c w196.c w211bh.c w230-03h.c w230h.c w83194br-39b.c \
       w83195r-08.c

OBJS = $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
	-$(CC) $(CFLAGS) -c $^

clean:
	-$(RM) $(OBJS)

