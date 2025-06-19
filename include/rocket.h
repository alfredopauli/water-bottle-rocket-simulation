#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "physics.h"

typedef struct
{
    char **names;
    double *values;
    double *min_values;
    double *max_values;
    size_t size;
    size_t capacity;
} RocketVariables;

void CreateRocketVariables(RocketVariables *rocketvar, size_t capacity);
void SetRocketVariable(RocketVariables *rocketvar, const char *name, double value, double min, double max);
double GetRocketVariable(RocketVariables *rocketvar, const char *name);
void DefaultRocketVariables(RocketVariables *rocketvar, PhysicsConstants *constants);
void FreeRocketVariables(RocketVariables *rocketvar);

double GetInitialMass(RocketVariables *rocketvar, PhysicsConstants *physconst);
double GetInitialGasVolume(RocketVariables *rocketvar);
double GetBottleSectionalArea(RocketVariables *rocketvar);
double GetRocketRadius(double n1, double n2, double r1, double r2, double bh, double z);
double GetRocketSection(double n1, double n2, double r1, double r2, double bh, double z);
double GetWaterVolume(double n1, double n2, double r1, double r2, double bh, double z_max, size_t points);
double GetWaterLevel(double wv, double n1, double n2, double r1, double r2, double bh, double dy);

