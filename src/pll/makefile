
ifeq ($(DJGPP),)
$(error ERROR: DJGPP not defined! ***)
endif

RM=del
CP=copy /Y
MD=mkdir
ZIP=zip -r
FIX_PATH = $1

ifeq ($(OS),WINDOWS_NT)
	RM=del /Q /F
	CP=copy /Y
	MD=mkdir
	ZIP=zip -r
	FIX_PATH = $1
	CROSS_COMPILER=i586-pc-msdosdjgpp-
endif
ifeq ($(shell uname -s),Linux)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
	CROSS_COMPILER=i586-pc-msdosdjgpp-
endif
ifeq ($(shell uname -s),Darwin)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
	CROSS_COMPILER=i586-pc-msdosdjgpp-
endif
ifeq ($(findstring MINGW,$(shell uname -s)),MINGW)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
	CROSS_COMPILER=i586-pc-msdosdjgpp-
endif
ifeq ($(findstring MSYS,$(shell uname -s)),MSYS)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
	CROSS_COMPILER=i586-pc-msdosdjgpp-
endif
ifeq ($(findstring CYGWIN,$(shell uname -s)),CYGWIN)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
	CROSS_COMPILER=i586-pc-msdosdjgpp-
endif

CC = $(CROSS_COMPILER)gcc
CFLAGS = -O2 -std=gnu99 -Wall -finline 

all: pll

pll: *.c
	-$(CC) $(CFLAGS) -c *.c

clean:
	-$(RM) *.o

