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

	g++ eye.cpp -lcairo -lX11 -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab
