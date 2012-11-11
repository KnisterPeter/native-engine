OS:=$(shell uname -s)
BITS:=$(shell getconf LONG_BIT)
MACHINE:=$(shell uname -i)
ARCH:=$(shell echo "$(OS)-$(MACHINE)" | tr A-Z a-z)
V8_TARGET_32:=ia32
V8_TARGET_64:=x64
BASEDIR:=../../..
LIBOUTDIR:=$(BASEDIR)/target/classes/de/matrixweb/jv8/$(ARCH)
LINKDIR:=$(BASEDIR)/target/jv8

NPROCS:=1
ifeq ($(OS),Linux)
  NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
endif
ifeq ($(OS),Darwin)
  NPROCS:=$(shell system_profiler | awk '/Number Of CPUs/{print $4}{next;}')
endif

.PHONY: all prepare v8 jv8

all: v8 jv8

prepare:
	mkdir -p $(LIBOUTDIR)
	mkdir -p $(LINKDIR)

v8: prepare 
	$(MAKE) -C $(BASEDIR)/target/v8/ dependencies
	$(MAKE) -j$(NPROCS) -C $(BASEDIR)/target/v8/ $(V8_TARGET_$(BITS)) library=shared
	cp $(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS)).release/lib.target/libv8.so $(LINKDIR)
	cp $(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS)).release/lib.target/libv8.so $(LIBOUTDIR)

jv8: prepare
	g++ -Wall -pedantic -fpic -I$(BASEDIR)/target/v8/include/ -o $(BASEDIR)/target/jv8/jv8.o -c jv8.cpp
	g++ -Wall -shared -Wl,--no-undefined -L$(LINKDIR) -o $(LINKDIR)/libjv8.so $(BASEDIR)/target/jv8/jv8.o -lv8
	cp $(LINKDIR)/libjv8.so $(LIBOUTDIR)
