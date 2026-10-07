
ifeq ($(DJGPP),)
$(error ERROR: DJGPP not defined! ***)
endif

RM=rm -f
CP=cp -f
MD=mkdir -p
ZIP=zip -r

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
	-$(RM) dist/$(TARGETZIP)
	-$(ZIP) dist/$(TARGETZIP) $(TARGET) $(TARGETTXT) $(DPMI)
	-$(RM) dist/$(TARGETDISTZIP)
	-$(ZIP) dist/$(TARGETDISTZIP) * -x bak/* dist/* *.o bak/ dist/ src/*.o sec/pll/*.o

.PHONY:	dist

