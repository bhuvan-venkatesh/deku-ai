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
ASAN_OBJS = $(addsuffix -asan,$(OBJS))

# HPP recursive directories

HPP = $(dir $(wildcard $(INCLUDE_DIR)/*) $(wildcard $(INCLUDE_DIR)/**/*))
H_INCLUDES = $(addprefix -I,$(sort $(HPP)) $(INCLUDE_DIR))
H_FILES = $(wildcard $(INCLUDE_DIR)/**/*.hpp) $(wildcard $(INCLUDE_DIR)/*.cpp)

# Main hardcoded paths

EXE_PATH = $(BIN_DIR)/$(EXE)
ASAN_PATH = $(BIN_DIR)/$(EXE)-asan

# Test hardcoded paths

TEST_OBJS_DIR = $(TEST_DIR)/objs
TEST_BIN_DIR = $(TEST_DIR)/bin

#Compiler Options

CXX = clang++
LD = clang++
CXXFLAGS = -std=c++11 $(H_INCLUDES) -O0 -g -pedantic -Wno-deprecated-declarations \
-Wno-c++98-compat -fcolor-diagnostics -ftemplate-backtrace-limit=0
LDFLAGS = -std=c++11 $(H_INCLUDES) -L/usr/lib -Weverything -pedantic -Wl,--start-group -lcairo \
-lX11 -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui \
-lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lxdo -lopencv_calib3d\
-Wl,--end-group

# All only makes the executableadd_to_

all: $(EXE_PATH)

# Fake targets for conveniencerun-asan

.PHONY: all check clean run run-asan

# Small program -- All modules are linked

$(EXE_PATH): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

$(ASAN_PATH): $(ASAN_OBJS)
	$(LD) $^ $(LDFLAGS) -fsanitize=address -o $@



# Function to find the matching path of the cpp file from .o file

define get_cpp
    $(filter %$(1).cpp,$(CPP))
endef

define get_hpp
    $(filter %$(1).hpp,$(H_FILES))
endef

# Needs a second hand expansion to correctly match pattern

.SECONDEXPANSION:
$(OBJS) : % : $$(call get_cpp,$$(subst .o,,$$(notdir %))) | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.SECONDEXPANSION:
$(ASAN_OBJS) : % : $$(call get_cpp,$$(subst .o-asan,,$$(notdir %))) | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer -c $< -o $@


# Test patterns

$(TEST_OBJS_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TEST_BIN_DIR)/%: $(subst $(OBJS_DIR)/$(EXE).o,,$(OBJS)) $(TEST_OBJS_DIR)/%.o | $(TEST_BIN_DIR)
	$(LD) $^ $(LDFLAGS) -o $@

# Precautionary directory makedirs in event directory does not exist

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

$(TEST_DIR):
	mkdir $(TEST_DIR) || mkdir $(TEST_OBJS_DIR)

$(TEST_BIN_DIR):
	mkdir $(TEST_BIN_DIR)

# Phony targets for convenience

# Check runs all of the boost tests

check: $(TEST_EXE)
	(cd $(TEST_BIN_DIR) ; rm *txt ; for f in * ; do ./$$f ; done)
	echo Finished

# Removes Object Files

clean:
	(cd $(OBJS_DIR); rm -rf *)

# Runs the executable

run:
	./$(EXE_PATH)

run-asan:
	ASAN_SYMBOLIZER_PATH=$$(which llvm-symbolizer-3.4) ./$(ASAN_PATH)


# Special print command to print the value of the variable
# Useful for makefile debugging, uncomment and set phony as needed

# print-%: ; @echo $* = $($*)
