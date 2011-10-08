CC       = mpic++
OUTPUT   = cluster-rl
INCLUDES = /usr/include/mpi
INCLUDES+= include
CPPFLAGS = -std=gnu++0x -g -fopenmp
CPPFLAGS+= $(addprefix -I,$(INCLUDES))
CXXFLAGS = -Llib
SRCS     = $(wildcard *.cpp)
OBJS     = $(addprefix build/,$(SRCS:.cpp=.o))

all: $(OUTPUT)

cluster-rl: $(OBJS)
	$(CC) -o build/$(OUTPUT) $(OBJS) $(CXXFLAGS)

build/%.o: %.cpp
	@mkdir -p build
	$(CC) $(CPPFLAGS) -o $@ -c $<

rebuild: clean cluster-rl

clean:
	rm -rf build/*

.PHONY: clean rebuild all
