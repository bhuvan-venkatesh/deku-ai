EXE = main


CXX = clang++
LD = clang++
CXXFLAGS = -std=c++11 -Iinclude/
LDFLAGS = -std=c++11 -Iinclude/ -Wl,--start-group -lcairo \
-lX11 -lopencv_calib3d -lopencv_contrib -lopencv_core \
-lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui \
-lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree \
-lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching \
-lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab \
-Wl,--end-group
SOURCE_DIR = src
OBJS_DIR = objs
BIN_DIR = bin

OBJS = $(OBJS_DIR)/brain.o $(OBJS_DIR)/eye.o $(OBJS_DIR)/gene.o \
$(OBJS_DIR)/genome.o $(OBJS_DIR)/neuron.o $(OBJS_DIR)/pool.o \
$(OBJS_DIR)/species.o $(OBJS_DIR)/main.o

HEADERS = brain.h eye.h gene.h genome.h key_press.h neuron.h \
pool.h species.h


all: $(EXE)

# Pattern rules for object files
$(OBJS_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $(BIN_DIR)/$@

clean:
	(cd $(OBJS_DIR); rm -rf *)

.PHONY: all clean

check:
	lol
