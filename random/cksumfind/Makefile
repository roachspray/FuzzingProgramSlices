LLBIN=/usr/lib/llvm-4.0/bin
LLVM_CONFIG=$(LLBIN)/llvm-config
#QUIET:=@
QUIET:=

SRC_DIR?=$(PWD)
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

PASS=CksumFind.so
PASS_OBJECTS=CksumFind.o

LIBS=$(shell $(LLVM_CONFIG) --libs) 


default: prep $(PASS)
prep:
	@echo "Prep phase"
	$(QUIET)mkdir -p build/obj
	$(QUIET)mkdir -p build/lib

define builditdood
$(QUIET)$(CPP) -o $(BOD)/$(1)/$(@F) -c $(CPPFLAGS) $(CXXFLAGS) $<
endef

%.o : $(SRC_DIR)/%.cpp
	@echo "Compiling $*.cpp"
	$(call builditdood,.)

$(PASS) : $(PASS_OBJECTS)
	@echo "Linking $@"
	$(QUIET)$(CPP) -o build/lib/$@ $(LOADABLE_MODULE_OPTIONS) $(CXXFLAGS) $(LDFLAGS) ${addprefix $(BOD)/,$^}


clean:
	$(QUIET)rm -rf build 

