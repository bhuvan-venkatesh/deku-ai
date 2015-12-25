EXE = 

OBJS_DIR = objs

CXX = g++
LD = g++
CXXFLAGS = -std=c++11
LDFLAGS = -std=c++11
HEADERS = src/NEAT/
SOURCE = src/NEAT/
TEST = tests/
TEST_SRC = src/
TEST_BIN = bin/

all: $(EXE)

# Pattern rules for object files
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Create directories
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(EXE):
	$(LD) $^ $(LDFLAGS) -o $@

# Include automatically generated dependencies
-include $(OBJS_DIR)/*.d

clean:
	(cd $(OBJS_DIR); rm -rf *)

.PHONY: all clean


check:
	$(CXX) $(CXXFLAGS) -I$(HEADERS) $(TEST)$(TEST_SRC)gene_test.cpp \
	$(SOURCE)gene.cpp $(HEADERS)gene.h -o $(TEST)$(TEST_BIN)gene_test && \
	./$(TEST)$(TEST_BIN)gene_test;