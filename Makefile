CXX = g++
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -lGL -lGLU -lGLEW -lstdc++
CFLAGS = -O1 -std=gnu++11 -MMD -g

#thanks to bobah on SO
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all: aiTest

aiTest: $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LIBS)

obj/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm obj/* aiTest
