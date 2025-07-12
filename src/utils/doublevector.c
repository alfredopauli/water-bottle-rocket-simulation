#include "utils/doublevector.h"


void CreateDoubleVector(DoubleVector *source, size_t capacity)
{
    source->data = (double *)malloc(sizeof(double) * capacity);
    source->size = 0;
    source->capacity = capacity;
}

void CopyDoubleVector(DoubleVector *dest, DoubleVector *source)
{
    dest->data = (double *)malloc(sizeof(double) * source->capacity);
    dest->size = source->size;
    dest->capacity = source->capacity;
    memcpy(dest->data, source->data, sizeof(double) * source->size);
}

void AppendDoubleVector(DoubleVector *source, double value)
{
    if (source->size >= source->capacity)
    {
        source->capacity *= 2;
        source->data = (double *)realloc(source->data, sizeof(double) * source->capacity);
    }

    source->data[source->size] = value;
    source->size++;
}

void ClearDoubleVector(DoubleVector *source)
{
    source->size = 0;
}

double GetDoubleVector(DoubleVector *source, size_t index)
{
    return source->data[index];
}

double GetLastDoubleVector(DoubleVector *source)
{
    return source->data[source->size-1];
}

size_t SizeDoubleVector(DoubleVector *source)
{
    return source->size;
}

void FreeDoubleVector(DoubleVector *source)
{
    free(source->data);
}

