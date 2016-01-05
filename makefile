# Makefile

# definitions of root directory nad EXES

EXE = main
TEST_EXE = $(addprefix $(TEST_BIN_DIR)/,$(subst .cpp,,$(subst $(TEST_DIR)/,,$(wildcard $(TEST_DIR)/*cpp))))
INCLUDE_DIR = include
SOURCE_DIR = src
TEST_DIR = tests
OBJS_DIR = objs
BIN_DIR = bin

# CPP recursive directories

CPP = $(wildcard $(SOURCE_DIR)/**/*.cpp) $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(addprefix $(OBJS_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(CPP))))
TEST_OBJS = $(addprefix $(TEST_OBJS_DIR)/,gene_test.o)

# Main hardcoded paths

EXE_PATH = $(BIN_DIR)/$(EXE)

# Test hardcoded paths

TEST_OBJS_DIR = $(TEST_DIR)/objs
TEST_BIN_DIR = $(TEST_DIR)/bin

#Compiler Options

CXX = clang++
LD = clang++
CXXFLAGS = -std=c++11 -I$(INCLUDE_DIR)/ -O2
LDFLAGS = -std=c++11 -I$(INCLUDE_DIR)/ -L/usr/lib -Wl,--start-group -lcairo \
-lX11 -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui \
-lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lxdo\
-Wl,--end-group

# All only makes the executable

all: $(EXE_PATH)

# Fake targets for convenience

.PHONY: all check clean run

# Small program -- All modules are linked

$(EXE_PATH): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

# Function to find the matching path of the cpp file from .o file

define get_cpp
    $(filter %$(1).cpp,$(CPP))
endef

# Needs a second hand expansion to correctly match pattern

.SECONDEXPANSION:
$(OBJS) : % : $$(call get_cpp,$$(subst .o,,$$(notdir %))) | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Test patterns

$(TEST_OBJS_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TEST_BIN_DIR)/%: $(OBJS) $(TEST_OBJS_DIR)/%.o | $(TEST_BIN_DIR)
	$(LD) $^ $(LDFLAGS) -o $@

# Precautionary directory makedirs

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

$(TEST_DIR):
	mkdir $(TEST_DIR)
	mkdir $(TEST_OBJS_DIR)

$(TEST_BIN_DIR):
	mkdir $(TEST_BIN_DIR)

# Phony targets for convenience

# Check runs all of the boost tests

check: $(TEST_EXE)
	(cd $(TEST_BIN_DIR) ; for f in * ; do ./$$f ; done)

clean:
	(cd $(OBJS_DIR); rm -rf *)

run:
	./$(EXE_PATH)
