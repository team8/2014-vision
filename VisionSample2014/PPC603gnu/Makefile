# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC603gnu
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/WindRiver/workspace/vision
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/VisionSample2014



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC603
TOOL_FAMILY = gnu
TOOL = gnu
TOOL_PATH = 
CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 

IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 

IDE_DEFINES = 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif


#Project Targets
PROJECT_TARGETS = Vision2014/$(MODE_DIR)/Vision2014.out \
	 Vision2014_partialImage/$(MODE_DIR)/Vision2014_partialImage.o


#Rules

# Vision2014
ifeq ($(DEBUG_MODE),1)
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
Vision2014/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
Vision2014/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 
Vision2014/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
Vision2014/$(MODE_DIR)/% : IDE_DEFINES = 
Vision2014/$(MODE_DIR)/% : PROJECT_TYPE = DKM
Vision2014/$(MODE_DIR)/% : DEFINES = 
Vision2014/$(MODE_DIR)/% : EXPAND_DBG = 0
Vision2014/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
Vision2014/$(MODE_DIR)/% : CPU = PPC603
Vision2014/$(MODE_DIR)/% : TOOL_FAMILY = gnu
Vision2014/$(MODE_DIR)/% : TOOL = gnu
Vision2014/$(MODE_DIR)/% : TOOL_PATH = 
Vision2014/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
Vision2014/$(MODE_DIR)/% : LIBPATH = 
Vision2014/$(MODE_DIR)/% : LIBS = 
Vision2014/$(MODE_DIR)/% : OBJ_DIR := Vision2014/$(MODE_DIR)

OBJECTS_Vision2014 = Vision2014_partialImage/$(MODE_DIR)/Vision2014_partialImage.o

Vision2014/$(MODE_DIR)/Vision2014.out : $(OBJECTS_Vision2014)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmppc $(OBJECTS_Vision2014) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_Vision2014) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

Vision2014/$(MODE_DIR)/Vision2014_compile_file : $(FILE) ;

_clean :: Vision2014/$(MODE_DIR)/Vision2014_clean

Vision2014/$(MODE_DIR)/Vision2014_clean : 
	$(TRACE_FLAG)if [ -d "Vision2014" ]; then cd "Vision2014"; rm -rf $(MODE_DIR); fi


# Vision2014_partialImage
ifeq ($(DEBUG_MODE),1)
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
Vision2014_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
Vision2014_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip 
Vision2014_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
Vision2014_partialImage/$(MODE_DIR)/% : IDE_DEFINES = 
Vision2014_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
Vision2014_partialImage/$(MODE_DIR)/% : DEFINES = 
Vision2014_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
Vision2014_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
Vision2014_partialImage/$(MODE_DIR)/% : CPU = PPC603
Vision2014_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
Vision2014_partialImage/$(MODE_DIR)/% : TOOL = gnu
Vision2014_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
Vision2014_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
Vision2014_partialImage/$(MODE_DIR)/% : LIBPATH = 
Vision2014_partialImage/$(MODE_DIR)/% : LIBS = 
Vision2014_partialImage/$(MODE_DIR)/% : OBJ_DIR := Vision2014_partialImage/$(MODE_DIR)

Vision2014_partialImage/$(MODE_DIR)/Objects/VisionSample2014/VisionSample2014.o : $(PRJ_ROOT_DIR)/VisionSample2014.cpp $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


OBJECTS_Vision2014_partialImage = Vision2014_partialImage/$(MODE_DIR)/Objects/VisionSample2014/VisionSample2014.o

Vision2014_partialImage/$(MODE_DIR)/Vision2014_partialImage.o : $(OBJECTS_Vision2014_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o "$@" $(OBJECTS_Vision2014_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

Vision2014_partialImage/$(MODE_DIR)/Vision2014_partialImage_compile_file : $(FILE) ;

_clean :: Vision2014_partialImage/$(MODE_DIR)/Vision2014_partialImage_clean

Vision2014_partialImage/$(MODE_DIR)/Vision2014_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "Vision2014_partialImage" ]; then cd "Vision2014_partialImage"; rm -rf $(MODE_DIR); fi

force : 

DEP_FILES := Vision2014_partialImage/$(MODE_DIR)/Objects/VisionSample2014/VisionSample2014.d
-include $(DEP_FILES)



-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
