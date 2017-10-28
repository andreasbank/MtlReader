STRIP         ?= $(shell which strip)
UPX           ?= $(shell which upx)
UPX_LEVEL     ?= -9
DOXYGEN       ?= $(shell which doxygen)
VALGRIND      ?= $(shell which valgrind)
VALGRIND_OPTS ?= --tool=memcheck --leak-check=yes
VALGRIND_PROG ?= $(BASE_DIR)/$(PROG) -Ux -f mac=0
DEBUG_FILE     = $(BASE_DIR)/.debug
NOCOMPR_FILE   = $(BASE_DIR)/.nocompr

PROG           = mtlreader
PROG_SO        = libmtlreader.so

prefix         = /usr/bin
BINDIR         = /usr/local/bin
INSTALL        = $(prefix)/install
BASE_DIR       = $(shell pwd)
SRCS_DIR       = $(BASE_DIR)/src
OBJS_DIR       = $(BASE_DIR)/obj
INCL_DIR       = $(BASE_DIR)/include
DOXYGEN_DIRS   = $(BASE_DIR)/html $(BASE_DIR)/latex

INCLUDES       = -I$(INCL_DIR)

CFLAGS        += --std=c++11 -O3 -Wall -g $(INCLUDES)
LDFLAGS       +=
LIBS          +=

SRCS           = $(wildcard $(SRCS_DIR)/*.cpp)
OBJS           = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))
OBJS_FPIC      = $(filter-out main.o,$(patsubst $(OBJS_DIR)/%.o,$(OBJS_DIR)/%_fpic.o,$(OBJS)))
DEPS           = $(wildcard $(INCL_DIR)/*.hpp)

STRIP_ERROR   := '\e[1;33m*** ERROR: strip command not found,'\
                 ' no stripping has been performed ***\e[0m'

UPX_ERROR     := '\e[1;33m*** ERROR: upx command not found,'\
                 ' no compressing has been performed ***\e[0m'

VALGRIND_ERROR:= '\e[1;33m*** ERROR: valgrind does not seem to be' \
                 ' installed on this system ***\e[0m'

DOXYGEN_ERROR := '\e[1;33m*** ERROR: doxygen does not seem to be' \
                 ' installed on this system ***\e[0m'

DEBUG_NOTE    := '\e[1;33m*** NOTE: This is a DEBUG build,'\
                 ' no stripping or compressing has been done ***\e[0m'

.PHONY: makedirs docs debug nodebug checkmem

all: makedirs $(PROG) $(PROG_SO)

makedirs:
	mkdir -p $(OBJS_DIR)

$(PROG): $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@
ifeq ("$(wildcard $(DEBUG_FILE))","")
ifneq ("$(STRIP)","")
	-$(STRIP) $@
else
	@echo -e $(STRIP_ERROR)
endif
ifeq ("$(wildcard $(NOCOMPR_FILE))","")
ifneq ("$(UPX)","")
	$(UPX) $(UPX_LEVEL) $@; true
else
	@echo -e $(UPX_ERROR)
endif
endif
else
	@echo -e $(DEBUG_NOTE)
endif

$(PROG_SO): $(OBJS_FPIC)
	$(CXX) $(CFLAGS) -shared $(LDFLAGS) $^ $(LIBS) -o $@
ifeq ("$(wildcard $(DEBUG_FILE))","")
ifneq ("$(STRIP)","")
	-$(STRIP) $@
else
	@echo -e $(STRIP_ERROR)
endif
ifeq ("$(wildcard $(NOCOMPR_FILE))","")
ifneq ("$(UPX)","")
	$(UPX) $(UPX_LEVEL) $@; true
else
	@echo -e $(UPX_ERROR)
endif
endif
else
	@echo -e $(DEBUG_NOTE)
endif

$(OBJS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.cpp $(DEPS)
	$(CXX) -c $(CFLAGS) $(LDFLAGS) $< -o $@

$(OBJS_FPIC): $(OBJS_DIR)/%_fpic.o : $(SRCS_DIR)/%.cpp $(DEPS)
	$(CXX) -c $(CFLAGS) -fPIC $(LDFLAGS) $< -o $@

install: all
	$(INSTALL) -d $(BINDIR)
	$(INSTALL) -m 0755 $(PROG) $(BINDIR)

clean:
	$(RM) $(PROG) $(PROG_SO) $(OBJS_DIR)/*.o *~ doxyfile.inc doxygen_sqlite3.db
	$(RM) -rf $(DOXYGEN_DIRS)

debug: clean
	touch $(DEBUG_FILE)

nodebug: clean
	$(RM) $(DEBUG_FILE)

nocompress: clean
	touch $(NOCOMPR_FILE)

compress: clean
	$(RM) $(NOCOMPR_FILE)

checkmem: $(PROG)
ifneq ("$(VALGRIND)","")
	$(VALGRIND) $(VALGRIND_OPTS) $(VALGRIND_PROG)
else
	@echo -e $(VALGRIND_ERROR)
endif

doxyfile.inc: Makefile
	echo INPUT = $(INCL_DIR) $(SRCS_DIR) > doxyfile.inc
	echo FILE_PATTERNS = $(DEPS) $(SRCS) >> doxyfile.inc

docs: doxyfile.inc $(SRCS)
ifneq ("$(DOXYGEN)","")
	$(DOXYGEN) doxyfile.mk
else
	@echo -e $(DOXYGEN_ERROR)
endif
