$(CC)    = g++
OUTPUT   = cluster-rl
CXXFLAGS = -std=gnu++0x -g
SRCS     = $(wildcard *.cpp)
OBJS     = $(addprefix build/,$(SRCS:.cpp=.o))

all: build 

build: $(OBJS)
	g++ -o $(OUTPUT) $(OBJS)

build/%.o: %.cpp
	@mkdir -p build
	g++ $(CXXFLAGS) -o $@ -c $<

rebuild: clean build

clean:
	rm -rf build/*

.PHONY: clean rebuild all
