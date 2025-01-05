CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC_DIR = src
BUILD_DIR = build

PKG_CONFIG = pkg-config
SFML_CFLAGS = $(shell $(PKG_CONFIG) --cflags sfml-window sfml-graphics sfml-system sfml-audio sfml-network)
SFML_LIBS = $(shell $(PKG_CONFIG) --libs sfml-window sfml-graphics sfml-system sfml-audio sfml-network)

SOURCES = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(BUILD_DIR)/%.o, $(SOURCES))

EXECUTABLE = $(BUILD_DIR)/perlin-noise

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@ $(SFML_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SFML_CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
