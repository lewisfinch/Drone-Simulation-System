CXX=g++
ROOT_DIR = ../..
DEP_DIR = $(ROOT_DIR)/dependencies
-include $(DEP_DIR)/env
CXXFLAGS = -std=c++17 -g -Wl,-rpath,$(DEP_DIR)/lib

APP_NAME = transit_service

BUILD_DIR = $(ROOT_DIR)/build/apps/$(APP_NAME)
EXEFILE = $(ROOT_DIR)/build/bin/$(APP_NAME)
INCLUDES = -I.. -I$(DEP_DIR)/include -Isrc -I. -I$(DEP_DIR)/include -Iinclude -I. -I$(ROOT_DIR)/libs/transit/include -I$(ROOT_DIR)/libs/routing/include
LIBDIRS = -L$(DEP_DIR)/lib -L$(ROOT_DIR)/build/lib
LIBS = -ltransit -lrouting -lCppWebServer -lwebsockets -lssl -lcrypto -lz -lpthread
SOURCES = $(shell find src -name '*.cc')
OBJFILES = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cc=.o))

all: $(EXEFILE)

# Applicaiton Targets:
$(EXEFILE): $(ROOT_DIR)/build/lib/libtransit.a $(OBJFILES)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LIBDIRS) $(OBJFILES) $(LIBS) -o $@

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
	rm -rf $(EXEFILE)
