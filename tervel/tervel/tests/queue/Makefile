#The MIT License (MIT)
#
#Copyright (c) 2015 University of Central Florida's Computer Software Engineering
#Scalable & Secure Systems (CSE - S3) Lab
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in
#all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#THE SOFTWARE.
#

OS := $(shell uname)

DEBUG = -DDEBUG=1 -g -g3 -Wall
# RELEASE = -O3 -DNDEBUG

delay ?= 5
limit ?= 5
version ?= NA

tervelINC = "-I../../../"
tervelSources= "$(shell find ../../util/ -name '*.cc')"
tervelFlags="-DUSE_TERVEL_METRICS -DTERVEL_PROG_ASSUR_DELAY=$(delay) -DTERVEL_PROG_ASSUR_LIMIT=$(limit)"

CXX      = g++
CXXFLAGS = $(RELEASE) $(DEBUG) $(cFlags) -DCONTAINER_FILE=$(input) -DINTEL -std=c++11  -march=native -m64 -pthread -fno-strict-aliasing  


# $(info CXXFLAGS is $(CXXFLAGS))

INC= $(cINC)
ifeq ($(OS), Darwin)
# Mac
LIB = $(cLIB) -g -ldl -lgflags -lpthread
else
# Linux
LIB = $(cLIB) -ldl -lgflags -lpthread -lrt
endif

# LIB += -lrt /usr/local/lib/libpapi.a
# cFlags += -DUSE_PAPI

SOURCES = $(cSources)

EXECUTABLE ?=../src/main.cc
OBJECTS = $(SOURCES:.cc=.o)

output ?= $(input).x
OUTPUT = executables/ver_$(version)_delay_$(delay)_lim_$(limit)/

.PHONY: all
.PHONY: all
all:  blank tervelQueueWF tervelQueueLF

.PHONY: all
verify: blank

.PHONY: blank
blank:
	$(MAKE) test input="api/blank.h" output="blank.x"

tervelQueueWF:
	$(MAKE) test input="tervel_api/wf_queue_api.h" output="queue_tervel_wf.x" cSources=$(tervelSources) cINC=$(tervelINC) cFlags=$(tervelFlags)

tervelQueueLF:
	$(MAKE) test input="tervel_api/lf_queue_api.h" output="queue_tervel_lf.x" cSources=$(tervelSources) cINC=$(tervelINC) cFlags=$(tervelFlags)

#feed executable 'queue_tervel_lf.x' parameters to switch between sequential and concurrent.
#proper syntax is ./queue_tervel_lf.x -numThreads <TotalThreads> <ThreadsInGroup> <dequeueThreads> <enqueueThreads> 0 0;
test-seq-lf:
	$(MAKE) test input="tervel_api/lf_myQueue_api.h" output="queue_tervel_lf.x" cSources=$(tervelSources) cINC=$(tervelINC) cFlags=$(tervelFlags)

test-seq-wf:
	$(MAKE) test input="tervel_api/wf_myQueue_api.h" output="queue_tervel_lf.x" cSources=$(tervelSources) cINC=$(tervelINC) cFlags=$(tervelFlags)

test-concur-lf:
	$(MAKE) test input="tervel_api/lf_myQueue_api.h" output="queue_tervel_lf.x" cSources=$(tervelSources) cINC=$(tervelINC) cFlags=$(tervelFlags)

test-concur-wf:
	$(MAKE) test input="tervel_api/wf_myQueue_api.h" output="queue_tervel_lf.x" cSources=$(tervelSources) cINC=$(tervelINC) cFlags=$(tervelFlags)


.PHONY: test
test: $(SOURCES) $(EXECUTABLE)

.PHONY: $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(OUTPUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC)  -o $(OUTPUT)$(output) $(EXECUTABLE) $^ $(LIB) $(TOBJS)

%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c -o $@ $<


.PHONY: clean
clean:
	$(RM) $(OUTPUT)*.x

.PHONY: clean-all
clean-all:
	$(RM) $(OUTPUT)*.x
	$(RM) $(shell find ../util/ -name '*.o')
	