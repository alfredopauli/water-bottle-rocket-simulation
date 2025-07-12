#pragma once
#include <stdio.h>
#include "utils/doublevector.h"

typedef struct
{
    DoubleVector time;
    DoubleVector y_position;
    DoubleVector y_velocity;
    double temperature;
    double pressure;
} Solution;

void CreateSolution(Solution *source);
void CopySolution(Solution *dest, Solution *source);
void ClearSolution(Solution *source);
void JoinSolution(Solution *first, Solution *second);
void SaveSolution(Solution *source, const char *position_file, const char *velocity_file);
void FreeSolution(Solution *source);

