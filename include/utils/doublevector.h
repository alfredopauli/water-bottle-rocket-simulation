#pragma once
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    double *data;
    size_t size;
    size_t capacity;
} DoubleVector;

void CreateDoubleVector(DoubleVector *source, size_t capacity);
void CopyDoubleVector(DoubleVector *dest, DoubleVector *source);
void AppendDoubleVector(DoubleVector *source, double value);
void ClearDoubleVector(DoubleVector *source);
double GetDoubleVector(DoubleVector *source, size_t index);
double GetLastDoubleVector(DoubleVector *source);
size_t SizeDoubleVector(DoubleVector *source);
void FreeDoubleVector(DoubleVector *source);

