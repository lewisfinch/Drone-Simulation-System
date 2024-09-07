CXX=g++
ROOT_DIR = ../..
DEP_DIR = $(ROOT_DIR)/dependencies
-include $(DEP_DIR)/env
CXXFLAGS = -std=c++17 -g -Wl,-rpath,$(DEP_DIR)/lib

LIB_NAME = libtransit.a

BUILD_DIR = $(ROOT_DIR)/build/libs/$(LIB_NAME)
LIBFILE = $(ROOT_DIR)/build/lib/$(LIB_NAME)
INCLUDES = -I.. -I$(DEP_DIR)/include -Isrc -I. -I$(DEP_DIR)/include -Iinclude -I. -I$(ROOT_DIR)/libs/routing/include
LIBDIRS = -L$(DEP_DIR)/lib
LIBS = -lCppWebServer -lwebsockets -lssl -lcrypto -lz -lpthread
SOURCES = $(shell find src -name '*.cc')
OBJFILES = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cc=.o))

all: $(LIBFILE)

# Applicaiton Targets:
$(LIBFILE): $(OBJFILES)
	mkdir -p $(dir $@)
	ar rcs $@ $(OBJFILES)

# Object File Targets:
$(BUILD_DIR)/%.o: %.cc
	mkdir -p $(dir $@)
	$(call make-depend-cxx,$<,$@,$(subst .o,.d,$@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Generate dependencies
make-depend-cxx=$(CXX) -MM -MF $3 -MP -MT $2 $(CXXFLAGS) $(INCLUDES) $1
-include $(OBJFILES:.o=.d)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(LIBFILE)