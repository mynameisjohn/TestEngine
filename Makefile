CXX = g++
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lm -lGL -lGLU -lGLEW -lstdc++# -ltinyxml
CFLAGS = -std=gnu++11 -MMD -MP -g -t
INCLUDES = -I include
TINYXML = obj/tinyxml

#thanks to bobah on SO
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all: testExe

testExe: $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS) $(TINYXML)/*.o


obj/%.o: src/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c -o $@ $<

-include obj/*.d

clean:
	rm obj/* testExe
