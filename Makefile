CXX = g++
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lm -lGL -lGLU -lGLEW -lstdc++ -ltinyxml
CFLAGS = -std=gnu++11 -MMD -MP -g
INCLUDES = -I include

#thanks to bobah on SO
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.d)))

all: testExe

testExe: $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)


obj/%.o: src/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c -o $@ $<

-include obj/*.d

clean:
	rm obj/* testExe
