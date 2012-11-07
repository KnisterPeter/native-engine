ARCH:=$(shell getconf LONG_BIT)
V8_TARGET_32:=ia32
V8_TARGET_64:=x64
BASEDIR:=../../..

NPROCS:=1
OS:=$(shell uname -s)
ifeq ($(OS),Linux)
  NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
endif
ifeq ($(OS),Darwin)
  NPROCS:=$(shell system_profiler | awk '/Number Of CPUs/{print $4}{next;}')
endif

.PHONY: v8

all: v8 jv8

v8: 
	$(MAKE) -C $(BASEDIR)/target/v8/ dependencies
	$(MAKE) -j$(NPROCS) -C $(BASEDIR)/target/v8/ $(V8_TARGET_$(ARCH)) library=shared
	cp $(BASEDIR)/target/v8/out/$(V8_TARGET_$(ARCH)).release/lib.target/libv8.so $(BASEDIR)/target/classes/lib

jv8:
	mkdir -p $(BASEDIR)/target/jv8
	gcc -Wall -fPIC -I$(BASEDIR)/target/v8/include -o $(BASEDIR)/target/jv8/jv8.o -c jv8.cpp
	mkdir -p $(BASEDIR)/target/classes/lib
	gcc -Wall -shared -o $(BASEDIR)/target/classes/lib/libjv8.so $(BASEDIR)/target/jv8/jv8.o

