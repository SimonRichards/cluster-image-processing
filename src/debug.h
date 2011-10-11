// debug.h is a simple debugging tool for any project, calculate
// run times, print variables and trace program execution. Or do
// none of the above with a simple change to DEBUG_LEVEL
#pragma once
#include <ctime>
#include <iostream>
#include <cstdio>
#include <time.h>
#include <omp.h>
#define DEBUG_LEVEL 2

#define LOC std::cout << "debug:" << __FILE__ << ":" << __LINE__ << " ";

// Always allow errors (unless DEBUG_LEVEL < 0)
#define FERROR(fmt,...) {LOC fprintf(stderr,fmt"\n",__VA_ARGS__);}
#define ERROR(txt) {LOC std::cerr << txt << std::endl;}


#if DEBUG_LEVEL > 0 // allow warnings
#define FWARNING(fmt,...) {LOC printf(fmt"\n",__VA_ARGS__);}
#define WARNING(txt) {LOC std::cout << txt << std::endl;}
#else
#define FWARNING(fmt,...)
#define WARNING(txt)
#endif

#if DEBUG_LEVEL > 1 // allow info
#define FPRINT(fmt,...) {LOC printf(fmt"\n",__VA_ARGS__);}
#define PRINT(txt) {LOC std::cout << txt << std::endl;}
#define VAL(txt) {LOC std::cout << text << std::endl;}
#define VAR(text) {LOC std::cout << (#text) << "=" << text << std::endl;}


#else
#define FPRINT(fmt"\n",...)
#define PRINT(txt"\n")
#define VAL(txt)
#define VAR(text)

#endif

//Performance timing (only prints if DEBUG_LEVEL > 1
#define START_TIMER() (clock() / (double) CLOCKS_PER_SEC)
#define PRINT_TIMER(task, x) \
    FPRINT(task" took %g seconds", ((clock() / (double) CLOCKS_PER_SEC) - x) / omp_get_num_procs());


class PerfTimer {
    struct timespec start, finish;

    public:
        void begin() {
            clock_gettime(CLOCK_MONOTONIC, &start);
        }

        double getElapsed() {
            clock_gettime(CLOCK_MONOTONIC, &finish);
            double elapsed = finish.tv_sec - start.tv_sec;
            VAR(elapsed);
            return elapsed + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        }
};
