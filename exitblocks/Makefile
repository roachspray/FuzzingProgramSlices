LLBIN=/usr/lib/llvm-4.0/bin
LLVM_CONFIG=$(LLBIN)/llvm-config
#QUIET:=@
QUIET:=

SRC_DIR?=$(PWD)/src
#3.8 flags
LDFLAGS38=-lpthread -ldl -lz -lcurses
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags) $(LDFLAGS38) 

COMMON_FLAGS=-Wall -Wextra -g

# For PSKIN_ROOT
PR=.

CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags)
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -std=c++11 -I$(SRC_DIR) 

LOADABLE_MODULE_OPTIONS=-shared
#LOADABLE_MODULE_OPTIONS=-shared -Wl,-O1

LDIS=$(LLBIN)/llvm-dis
CPP=$(LLBIN)/clang++
CC=$(LLBIN)/clang
LLC=$(LLBIN)/llc
OPTB=$(LLBIN)/opt

BOD=build/obj

EBPASS=ExitBlock.so
EBPASS_OBJECTS=Transform/ExitBlocksPass.o  \
 CommonUtils.o  \
 TraceUtils.o 


LIBS=$(shell $(LLVM_CONFIG) --libs) 

TDIR=build/tests

default: prep $(EBPASS)


#passmgr

prep:
	@echo "Prep phase"
	$(QUIET)mkdir -p build/obj
	$(QUIET)mkdir -p build/obj/Analysis
	$(QUIET)mkdir -p build/obj/Transform
	$(QUIET)mkdir -p build/bin
	$(QUIET)mkdir -p build/lib

define builditdood
$(QUIET)$(CPP) -o $(BOD)/$(1)/$(@F) -c $(CPPFLAGS) $(CXXFLAGS) $<
endef

Transform/%.o: $(SRC_DIR)/Transform/%.cpp
	@echo "Compiling $*.cpp"
	$(call builditdood,Transform)

Analysis/%.o: $(SRC_DIR)/Analysis/%.cpp
	@echo "Compiling $*.cpp"
	$(call builditdood,Analysis)

%.o : $(SRC_DIR)/%.cpp
	@echo "Compiling $*.cpp"
	$(call builditdood,.)

$(EBPASS) : $(EBPASS_OBJECTS)
	@echo "Linking $@"
	$(QUIET)$(CPP) -o build/lib/$@ $(LOADABLE_MODULE_OPTIONS) $(CXXFLAGS) $(LDFLAGS) ${addprefix $(BOD)/,$^}

clean:
	$(QUIET)rm -rf build Pruneskin

libs:
	cd lib/libtrays && make -f Makefile	

cleanlibs:
	cd lib/libtrays && make -f Makefile	clean

cleanall: clean cleanlibs
