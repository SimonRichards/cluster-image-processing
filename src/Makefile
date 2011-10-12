CC       = mpic++
OUTPUT   = cluster-rl
INCLUDES = /usr/include/mpi
INCLUDES+= include

# Add these to use the FFT version
FFT_FLAGS= -DUSE_FFT -lfftw3_omp -lfftw3 -lm -lrt

CPPFLAGS = -std=gnu++0x -fopenmp -Wall -pedantic -Wshadow -O3
CPPFLAGS+= $(addprefix -I,$(INCLUDES))
CXXFLAGS = -Llib -fopenmp
SRCS     = $(wildcard *.cpp)
OBJS     = $(addprefix build/,$(SRCS:.cpp=.o))

all: $(OUTPUT)

run: all
	build/$(OUTPUT)

cluster-rl: $(OBJS)
	$(CC) -o build/$(OUTPUT) $(OBJS) lib/* $(CXXFLAGS)

build/%.o: %.cpp *.h
	@mkdir -p build
	$(CC) $(CPPFLAGS) -o $@ -c $<

rebuild: clean cluster-rl

clean:
	rm -rf build/*

.PHONY: clean rebuild all
