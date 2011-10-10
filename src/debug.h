#pragma once
#include <iostream>
#include <cstdio>
#include <time.h>
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
#define START_TIMER() time(NULL)
#define PRINT_TIMER(task, x) \
    FPRINT(task" took %i seconds", uint16_t(time(NULL) - x));
