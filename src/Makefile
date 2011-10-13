CC       = g++
#mpic++
OUTPUT   = cluster-rl
INCLUDES = /usr/include/mpi
INCLUDES+= include /usr/include/openmpi-x86_64/

# Add these to use the FFT version
FFT_FLAGS= -DUSE_FFT -lfftw3_omp -lfftw3 -lm -lrt

CPPFLAGS = -fopenmp -Wall -pedantic -Wshadow -O3
CPPFLAGS+= $(addprefix -I,$(INCLUDES))
CXXFLAGS = -Llib64 -fopenmp -lcfitsio -lrt -lstdc++
SRCS     = $(wildcard *.cpp)
OBJS     = $(addprefix build/,$(SRCS:.cpp=.o))

all: $(OUTPUT)

run: all
	build/$(OUTPUT)

cluster-rl: $(OBJS)
	$(CC) -o build/$(OUTPUT) $(OBJS) $(CXXFLAGS)

build/%.o: %.cpp *.h
	@mkdir -p build
	$(CC) $(CPPFLAGS) -o $@ -c $<

rebuild: clean cluster-rl

clean:
	rm -rf build/*

.PHONY: clean rebuild all
