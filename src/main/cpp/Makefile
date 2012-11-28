OS:=$(shell uname -s)
BITS:=$(shell getconf LONG_BIT)
MACHINE:=$(shell uname -i)
ARCH:=$(shell echo "$(OS)-$(MACHINE)" | tr A-Z a-z)
V8_TARGET_32:=ia32.release
V8_TARGET_64:=x64.release
BASEDIR:=../../..
LIBOUTDIR:=$(BASEDIR)/target/classes/de/matrixweb/ne/$(ARCH)
LINKDIR:=$(BASEDIR)/target/ne

NPROCS:=1
ifeq ($(OS),Linux)
  NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
endif
ifeq ($(OS),Darwin)
  NPROCS:=$(shell system_profiler | awk '/Number Of CPUs/{print $4}{next;}')
endif

.PHONY: all prepare v8 ne

all: v8 ne

prepare:
	mkdir -p $(LIBOUTDIR)
	mkdir -p $(LINKDIR)

v8: prepare 
	$(MAKE) -C $(BASEDIR)/target/v8/ dependencies
	CXX="g++ -fPIC" $(MAKE) -j$(NPROCS) -C $(BASEDIR)/target/v8/ $(V8_TARGET_$(BITS))

ne: prepare
	g++ -Wall -pedantic -fpic -I$(BASEDIR)/target/v8/include/ -o $(BASEDIR)/target/ne/native-engine.o -c native-engine.cpp
	g++ -Wall -shared -Wl,--no-undefined -o $(LINKDIR)/libnative-engine.so \
		-pthread \
		-Wl,--start-group \
			$(BASEDIR)/target/ne/native-engine.o \
			$(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS))/obj.target/tools/gyp/libv8_snapshot.a \
			$(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS))/obj.target/tools/gyp/libv8_base.a \
		-Wl,--end-group
	cp $(LINKDIR)/libnative-engine.so $(LIBOUTDIR)
