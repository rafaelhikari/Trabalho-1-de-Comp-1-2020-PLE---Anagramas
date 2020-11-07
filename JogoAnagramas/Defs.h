#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define TAM_MAX 7
#define DIC_MAX 46
#define LINHA_MAX 457

#ifdef _WIN32
    #define CLS "cls"
#elif WIN64
    #define CLS "cls"
#elif __linux__
    #define CLS "clear"
#else
    #define CLS "clear"
#endif

#endif
