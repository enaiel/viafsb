
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
endif
ifeq ($(shell uname -s),Linux)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
endif
ifeq ($(shell uname -s),Darwin)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
endif
ifeq ($(findstring MINGW,$(shell uname -s)),MINGW)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
endif
ifeq ($(findstring MSYS,$(shell uname -s)),MSYS)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
endif
ifeq ($(findstring CYGWIN,$(shell uname -s)),CYGWIN)
	RM=rm -f
	CP=cp -f
	MD=mkdir -p
	ZIP=zip -r
	FIX_PATH = $(subst \,/,$1)
endif

PROG=viafsb
VERFILE=VERSION
VER=$(strip $(shell cat $(VERFILE)))
DPMI=cwsdpmi.exe
TARGET=$(PROG).exe
TARGETTXT=$(PROG).txt
TARGETZIP=$(PROG)-$(VER).zip
TARGETDISTZIP=$(PROG)_dist-$(VER).zip

all:
	-$(MAKE) -C src all

clean:
	-$(MAKE) -C src clean

dist: 
	-$(MD) dist
	-$(CP) $(call FIX_PATH,src/$(TARGET)) .
	-$(RM) $(call FIX_PATH,src/$(TARGET))
	-$(RM) $(call FIX_PATH,dist/$(TARGETZIP))
	-$(ZIP) $(call FIX_PATH,dist/$(TARGETZIP)) $(TARGET) $(TARGETTXT) $(DPMI)
	-$(RM) $(call FIX_PATH,dist/$(TARGETDISTZIP))
	-$(ZIP) $(call FIX_PATH,dist/$(TARGETDISTZIP)) * -x bak/* dist/* *.o bak/ dist/ src/*.o

.PHONY:	dist

