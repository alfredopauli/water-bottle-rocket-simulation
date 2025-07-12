#include "simulation/solution.h"


void CreateSolution(Solution *source)
{
    CreateDoubleVector(&source->time, 100);
    CreateDoubleVector(&source->y_position, 100);
    CreateDoubleVector(&source->y_velocity, 100);
    source->temperature = 0;
    source->pressure = 0;
}

void CopySolution(Solution *dest, Solution *source)
{
    CopyDoubleVector(&dest->time, &source->time);
    CopyDoubleVector(&dest->y_position, &source->y_position);
    CopyDoubleVector(&dest->y_velocity, &source->y_velocity);
    dest->temperature = source->temperature;
    dest->pressure = source->pressure;
}

void ClearSolution(Solution *source)
{
    ClearDoubleVector(&source->time);
    ClearDoubleVector(&source->y_position);
    ClearDoubleVector(&source->y_velocity);
}

void JoinSolution(Solution *first, Solution *second)
{
    double start_time = GetLastDoubleVector(&first->time);
    for (size_t i=0; i < second->time.size; i++)
    {
       AppendDoubleVector(&first->time, start_time + GetDoubleVector(&second->time, i));
       AppendDoubleVector(&first->y_position, GetDoubleVector(&second->y_position, i));
       AppendDoubleVector(&first->y_velocity, GetDoubleVector(&second->y_velocity, i));
    }
    first->temperature = second->temperature;
    first->pressure = second->pressure;
}

void SaveSolution(Solution *source, const char *position_file, const char *velocity_file)
{
    FILE *file;

    file = fopen(position_file, "w");
    for (size_t i=0; i < source->time.size; i++)
        fprintf(file, "%lf,%lf\n", GetDoubleVector(&source->time, i), GetDoubleVector(&source->y_position, i));
    fclose(file);
    
    file = fopen(velocity_file, "w");
    for (size_t i=0; i < source->time.size; i++)
        fprintf(file, "%lf,%lf\n", GetDoubleVector(&source->time, i), GetDoubleVector(&source->y_velocity, i));
    fclose(file);
}

void FreeSolution(Solution *source)
{
    FreeDoubleVector(&source->time);
    FreeDoubleVector(&source->y_position);
    FreeDoubleVector(&source->y_velocity);
}

