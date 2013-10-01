OS:=$(shell uname -s)
BITS:=$(shell getconf LONG_BIT)
MACHINE:=$(shell uname -i)
ARCH:=$(shell echo "$(OS)-$(MACHINE)" | tr A-Z a-z)
V8_TARGET_32:=ia32.release
V8_TARGET_64:=x64.release
BASEDIR:=../../..
LINKDIR:=$(BASEDIR)/target/ne
LIBOUTDIR:=$(BASEDIR)/target/classes/de/matrixweb/ne/$(ARCH)

ifdef DEBUG
	CFLAGS+=-DDEBUG
endif

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
	$(MAKE) -j$(NPROCS) -C $(BASEDIR)/target/v8/ $(V8_TARGET_$(BITS)) library=shared
	if [ "$(OS)" = "Linux" ] ; then
		cp $(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS))/obj.target/tools/gyp/libv8.so $(LINKDIR)
		cp $(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS))/obj.target/tools/gyp/libv8.so $(LIBOUTDIR)
	elif [ "$(OS)" = "Darwin" ] ; then
		cp $(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS))/libv8.dylib $(LINKDIR)
		cp $(BASEDIR)/target/v8/out/$(V8_TARGET_$(BITS))/libv8.dylib $(LIBOUTDIR)
	fi

ne: prepare
	g++ -Wall -pedantic -fPIC $(CFLAGS) -I$(BASEDIR)/target/v8/include/ -o $(BASEDIR)/target/ne/native-engine.o -c native-engine.cpp
	g++ -Wall -shared -o $(LINKDIR)/libnative-engine.so \
		-pthread \
		-L$(LIBOUTDIR) \
		$(BASEDIR)/target/ne/native-engine.o \
		-lv8
	if [ "$(OS)" = "Linux" ] ; then
		cp $(LINKDIR)/libnative-engine.so $(LIBOUTDIR)
	elif [ "$(OS)" = "Darwin" ] ; then
		cp $(LINKDIR)/libnative-engine.dylib $(LIBOUTDIR)
	fi
