CC       = mpic++
#mpic++
OUTPUT   = cluster-rl
INCLUDES = /usr/include/mpi
INCLUDES+= include
FFT_FLAGS= -DUSE_FFT -lfftw3_omp -lfftw3 -lm -fopenmp -lrt
CPPFLAGS = -std=gnu++0x -g -fopenmp -Wall -pedantic -Wshadow
CPPFLAGS+= $(addprefix -I,$(INCLUDES))
CXXFLAGS = -Llib -fopenmp
SRCS     = $(wildcard *.cpp)
OBJS     = $(addprefix build/,$(SRCS:.cpp=.o))

all: $(OUTPUT)

run:
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
