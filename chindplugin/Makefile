# makefile
#
# libraryname (for shared library?)
libname = auzoneobst.so
#
# AU Robot Server location (if this is placed outside aurobotservers folder
AUROBOTDIR = ../../aurobotservers/trunk
BINDIR = ../bin
#
# preprocessor flags like -I(include dir)
CPPFLAGS =  -I$(AUROBOTDIR)/include  -I/usr/include/opencv2
#
# linker flags like e.g. -lpthread and -L/usr/local/lib
LDFLAGS = -g0 -shared -Wl,-soname,$(libname)
#
# extra preprocessor defines (mainly for module testcode)
DEFINES = -D LIBRARY_OPEN_NEEDED
#
# C++ compiler flags
CXXFLAGS = -g3 -O0 -Wall -pedantic -fPIC $(DEFINES)
#CXXFLAGS += -Wno-unused-but-set-variable
include ../../aurobotservers/trunk/include/opencv.mk
include ../../aurobotservers/trunk/include/opencv_flags.mk
# ifeq ($(OPENCV),2.1)
#   export OPENCV=
# endif
# ifeq ($(OPENCV),)
# else
#   CXXFLAGS += `pkg-config opencv-$(OPENCV) --cflags`
# endif
#
# Object files to produce before link
objects = ufunczoneobst.o

# shared library file name (version 0)
shlib = $(libname).0

# compile all - all objects depend on all other (objects)
all: $(objects)
	c++ -o $(shlib) $(objects) $(LDFLAGS)
  
.PHONY : clean install
clean :
	rm -f $(shlib) $(objects)
	-@rm -fr *~ .deps


install:
	cp $(shlib) ../bin

######################################################################
#
# Automatic dependencies

DEPS_MAGIC := $(shell mkdir -p .deps)


%.o: .deps/%.d

.deps/%.d: src/%.c
	@cc $(CFLAGS) -MM $< | sed 's#^$(@F:%.d=%.o):#$@ $(@:.deps/%.d=%.o):#' 

.deps/%.d: %.cpp
	@g++ $(CFLAGS) -MM $< | sed 's#^$(@F:%.d=%.o):#$@ $(@:.deps/%.d=%.o):#' > $@



######################################################################
#
# Include automatic dependencies

-include $(patsubst %.o, .deps/%.d, $(objects))
