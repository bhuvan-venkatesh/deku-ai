EXE = main
TEST_EXE = $(addprefix $(TEST_BIN_DIR)/,$(subst .cpp,,$(subst $(TEST_DIR)/,,$(wildcard $(TEST_DIR)/*cpp))))
INCLUDE_DIR = include
SOURCE_DIR = src
TEST_DIR = tests
OBJS_DIR = objs
BIN_DIR = bin

TEST_OBJS_DIR = $(TEST_DIR)/objs
TEST_BIN_DIR = $(TEST_DIR)/bin

CXX = clang++
LD = clang++
CXXFLAGS = -std=c++11 -I$(INCLUDE_DIR)/ -O2
LDFLAGS = -std=c++11 -I$(INCLUDE_DIR)/ -L/usr/lib -Wl,--start-group -lcairo \
-lX11 -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui \
-lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lxdo\
-Wl,--end-group

CPP = $(wildcard $(SOURCE_DIR)/**/*.cpp) $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(addprefix $(OBJS_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(CPP))))
TEST_OBJS = $(addprefix $(TEST_OBJS_DIR)/,gene_test.o)

MAIN_O = $(OBJS_DIR)/main.o
# HEADERS_INC = $(wildcard $(INCLUDE_DIR)/*.hpp)
# HEADERS = $(subst $(INCLUDE_DIR)/,,$(HEADERS_INC))
EXE_PATH = $(BIN_DIR)/$(EXE)

print-%: ; @echo $* = $($*)
all: $(EXE_PATH)
# Pattern rules for object src files

check: $(TEST_EXE)
	(cd $(TEST_BIN_DIR) ; for f in * ; do ./$$f ; done)

define get_cpp
    $(filter %$(1).cpp,$(CPP))
endef

.SECONDEXPANSION:
$(OBJS) : % : $$(call get_cpp,$$(subst .o,,$$(notdir %)))
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJS_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TEST_BIN_DIR)/%: $(OBJS) $(TEST_OBJS_DIR)/%.o | $(TEST_BIN_DIR)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_PATH): $(OBJS) $(MAIN_O)
	$(LD) $^ $(LDFLAGS) -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

$(TEST_DIR):
	mkdir $(TEST_DIR)
	mkdir $(TEST_OBJS_DIR)

$(TEST_BIN_DIR):
	$(TEST_BIN_DIR)

clean:
	(cd $(OBJS_DIR); rm -rf *)

.PHONY: all clean

run:
	./$(EXE_PATH)
