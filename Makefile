CXX := g++-14
CXXFLAGS := -std=c++20 -fmodules-ts -Wall -g
CURSES_LIB := ncurses
SDL_LIBS := -lSDL2 -lSDL2_mixer
SRC_DIR := src

AGE_OBJECTS := $(SRC_DIR)/core/Position.o \
                $(SRC_DIR)/core/Hitbox.o \
                $(SRC_DIR)/controller/InputEvent.o \
                $(SRC_DIR)/core/Clock.o \
                $(SRC_DIR)/view/Shape.o \
                $(SRC_DIR)/view/Drawable.o \
                $(SRC_DIR)/model/Animation.o \
                $(SRC_DIR)/model/Entity.o \
                $(SRC_DIR)/events/Event.o \
                $(SRC_DIR)/events/EventManager.o \
                $(SRC_DIR)/model/ResourceManager.o \
                $(SRC_DIR)/audio/SoundSystem.o \
                $(SRC_DIR)/controller/Controller.o \
                $(SRC_DIR)/model/World.o \
                $(SRC_DIR)/view/View.o \
                $(SRC_DIR)/model/Model.o \
                $(SRC_DIR)/model/Engine.o

MAIN_OBJECTS := $(AGE_OBJECTS) $(SRC_DIR)/main.o games/FlappyBird.o games/SpaceInvaders.o

HEADERS := iostream sstream memory vector clocale string_view stdexcept algorithm optional utility cstddef variant functional unordered_map random string

.PHONY: all clean

all: age

# System header compilation
gcm.cache/usr:
	$(CXX) $(CXXFLAGS) -c -x c++-system-header iostream
	$(CXX) $(CXXFLAGS) -c -x c++-system-header sstream
	$(CXX) $(CXXFLAGS) -c -x c++-system-header memory
	$(CXX) $(CXXFLAGS) -c -x c++-system-header vector
	$(CXX) $(CXXFLAGS) -c -x c++-system-header clocale
	$(CXX) $(CXXFLAGS) -c -x c++-system-header string_view
	$(CXX) $(CXXFLAGS) -c -x c++-system-header stdexcept
	$(CXX) $(CXXFLAGS) -c -x c++-system-header algorithm
	$(CXX) $(CXXFLAGS) -c -x c++-system-header optional
	$(CXX) $(CXXFLAGS) -c -x c++-system-header utility
	$(CXX) $(CXXFLAGS) -c -x c++-system-header cstddef
	$(CXX) $(CXXFLAGS) -c -x c++-system-header variant
	$(CXX) $(CXXFLAGS) -c -x c++-system-header functional
	$(CXX) $(CXXFLAGS) -c -x c++-system-header unordered_map
	$(CXX) $(CXXFLAGS) -c -x c++-system-header random
	$(CXX) $(CXXFLAGS) -c -x c++-system-header string

# Compile source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cc gcm.cache/usr
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link age executable
age: $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJECTS) -l$(CURSES_LIB) $(SDL_LIBS) -o $@

# Module dependency ordering
$(SRC_DIR)/core/Hitbox.o: $(SRC_DIR)/core/Position.o
$(SRC_DIR)/controller/InputEvent.o: $(SRC_DIR)/core/Position.o
$(SRC_DIR)/view/Drawable.o: $(SRC_DIR)/view/Shape.o

$(SRC_DIR)/events/EventManager.o: $(SRC_DIR)/events/Event.o
$(SRC_DIR)/model/ResourceManager.o: $(SRC_DIR)/view/Shape.o

# World depends on entity and events
$(SRC_DIR)/model/Entity.o: $(SRC_DIR)/core/Hitbox.o $(SRC_DIR)/controller/InputEvent.o $(SRC_DIR)/core/Position.o $(SRC_DIR)/model/Animation.o $(SRC_DIR)/view/Drawable.o $(SRC_DIR)/view/Shape.o
$(SRC_DIR)/model/World.o: $(SRC_DIR)/core/Hitbox.o $(SRC_DIR)/controller/InputEvent.o $(SRC_DIR)/core/Position.o $(SRC_DIR)/model/Entity.o $(SRC_DIR)/events/Event.o $(SRC_DIR)/events/EventManager.o $(SRC_DIR)/view/Drawable.o $(SRC_DIR)/view/Shape.o

$(SRC_DIR)/controller/Controller.o: $(SRC_DIR)/controller/InputEvent.o $(SRC_DIR)/core/Position.o
$(SRC_DIR)/view/View.o: $(SRC_DIR)/view/Drawable.o $(SRC_DIR)/view/Shape.o
$(SRC_DIR)/model/Model.o: $(SRC_DIR)/controller/Controller.o $(SRC_DIR)/view/Drawable.o $(SRC_DIR)/view/View.o

# Engine depends on Model and all subsystems
$(SRC_DIR)/model/Engine.o: $(SRC_DIR)/model/Model.o $(SRC_DIR)/controller/Controller.o $(SRC_DIR)/core/Clock.o $(SRC_DIR)/controller/InputEvent.o $(SRC_DIR)/events/Event.o $(SRC_DIR)/events/EventManager.o $(SRC_DIR)/model/ResourceManager.o $(SRC_DIR)/audio/SoundSystem.o $(SRC_DIR)/view/Drawable.o $(SRC_DIR)/view/View.o $(SRC_DIR)/model/World.o

$(SRC_DIR)/main.o: $(SRC_DIR)/model/Engine.o

clean:
	rm -f $(MAIN_OBJECTS) age
	rm -rf gcm.cache
