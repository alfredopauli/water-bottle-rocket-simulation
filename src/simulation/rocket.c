#include "simulation/rocket.h"


void CreateRocketVariables(RocketVariables *rocketvar, size_t capacity)
{
    rocketvar->names = (char **)malloc(sizeof(char *) * capacity);
    rocketvar->values = (double *)malloc(sizeof(double) * capacity);
    rocketvar->min_values = (double *)malloc(sizeof(double) * capacity);
    rocketvar->max_values = (double *)malloc(sizeof(double) * capacity);
    rocketvar->size = 0;
    rocketvar->capacity = capacity;
}

void SetRocketVariable(RocketVariables *rocketvar, const char *name, double value, double min, double max)
{
    assert(rocketvar->size < rocketvar->capacity && "ERROR: Rocket variables maximum capacity reached!");
    rocketvar->names[rocketvar->size] = (char *)malloc(strlen(name) + 1);
    strcpy(rocketvar->names[rocketvar->size], name);
    rocketvar->values[rocketvar->size] = value;
    rocketvar->min_values[rocketvar->size] = min;
    rocketvar->max_values[rocketvar->size] = max;
    rocketvar->size++;
}

double GetRocketVariable(RocketVariables *rocketvar, const char *name)
{
    for (size_t i=0; i < rocketvar->size; i++)
        if (strcmp(rocketvar->names[i], name) == 0)
            return rocketvar->values[i];
        assert(0 && "[ERROR]: Could not find key");
}

void DefaultRocketVariables(RocketVariables *rocketvar, PhysicsConstants *constants)
{
    double drag_coeficient = 0.5;
    double initial_pressure = 7e5;
    double launch_tube_volume = 0.0005;
    double bottle_volume = 2.1 / 1000;
    double initial_water_volume = 0.5 / 1000;
    double dry_mass = 0.10;
    double launch_tube_size = 0.15;
    double launch_tube_section_1 = pow(0.0035, 2) * M_PI;
    double launch_tube_section_2 = pow(0.0045, 2) * M_PI;
    double bottle_radius_1 = 0.0022;
    double bottle_radius_2 = 0.0525;
    double bottle_height = 0.27;
    double nozzle_size_1 = 0.015;
    double nozzle_size_2 = 0.075;
    double initial_temperature = 273 + 20;

    CreateRocketVariables(rocketvar, 18);
    SetRocketVariable(rocketvar, "drag_coeficient", drag_coeficient, 0, 2.0 * drag_coeficient);
    SetRocketVariable(rocketvar, "initial_pressure", initial_pressure, 0, 2.0 * initial_pressure);
    SetRocketVariable(rocketvar, "launch_tube_volume", launch_tube_volume, 0, 2.0 * launch_tube_volume);
    SetRocketVariable(rocketvar, "bottle_volume", bottle_volume, 0, 2.0 * bottle_volume);
    SetRocketVariable(rocketvar, "initial_water_volume", initial_water_volume, 0, 2.0 * initial_water_volume);
    SetRocketVariable(rocketvar, "dry_mass", dry_mass, 0, 2.0 * dry_mass);
    SetRocketVariable(rocketvar, "launch_tube_size", launch_tube_size, 0, 2.0 * launch_tube_size);
    SetRocketVariable(rocketvar, "launch_tube_section_1", launch_tube_section_1, 0, 2.0 * launch_tube_section_1);
    SetRocketVariable(rocketvar, "launch_tube_section_2", launch_tube_section_2, 0, 2.0 * launch_tube_section_2);
    SetRocketVariable(rocketvar, "bottle_radius_1", bottle_radius_1, 0, 2.0 * bottle_radius_1);
    SetRocketVariable(rocketvar, "bottle_radius_2", bottle_radius_2, 0, 2.0 * bottle_radius_2);
    SetRocketVariable(rocketvar, "bottle_height", bottle_height, 0, 2.0 * bottle_height);
    SetRocketVariable(rocketvar, "nozzle_size_1", nozzle_size_1, 0, 2.0 * nozzle_size_1);
    SetRocketVariable(rocketvar, "nozzle_size_2", nozzle_size_2, 0, 2.0 * nozzle_size_2);
    SetRocketVariable(rocketvar, "initial_temperature", initial_temperature, 0.1, 2.0 * initial_temperature);
}

void FreeRocketVariables(RocketVariables *rocketvar)
{
    for (size_t i=0; i < rocketvar->size; i++)
        free(rocketvar->names[i]);
    free(rocketvar->names);
    free(rocketvar->values);
}


double GetInitialMass(RocketVariables *rocketvar, PhysicsConstants *physconst)
{
    return GetRocketVariable(rocketvar, "dry_mass") + 
        GetRocketVariable(rocketvar, "initial_water_volume") * 
        physconst->water_density;
}

double GetInitialGasVolume(RocketVariables *rocketvar)
{
    return GetRocketVariable(rocketvar, "launch_tube_volume")
        + GetRocketVariable(rocketvar, "bottle_volume")
        - GetRocketVariable(rocketvar, "initial_water_volume")
        - GetRocketVariable(rocketvar, "launch_tube_volume")
        * (GetRocketVariable(rocketvar, "launch_tube_section_2") 
            - GetRocketVariable(rocketvar, "launch_tube_section_1"));
}

double GetBottleSectionalArea(RocketVariables *rocketvar)
{
    return pow(GetRocketVariable(rocketvar, "bottle_radius_2"), 2) * M_PI;
}

double GetRocketRadius(double n1, double n2, double r1, double r2, double bh, double z)
{
    if (z >= 0 && z < n1)
    {
        return r1;
    } else if (z >= n1 && z < n2)
    {
        return (r2 - r1) * (z - n1) / (n2 - n1) + r1;
    } else if (z >= n2 && z <= bh)
    {
        return r2;
    } else
    {
        assert(0 && "out of bounds");
    }
}

double GetRocketSection(double n1, double n2, double r1, double r2, double bh, double z)
{
    return M_PI * pow(GetRocketRadius(n1, n2, r1, r2, bh, z), 2);
}

double GetWaterVolume(double n1, double n2, double r1, double r2, double bh, double z_max, size_t points)
{
    double z = 0; 
    double water_volume = 0;
    double dz = z_max / points;
    for (z=0; z < z_max; z+=dz)
        water_volume += GetRocketSection(n1, n2, r1, r2, bh, z) * dz;
    return water_volume;
}

double GetWaterLevel(double wv, double n1, double n2, double r1, double r2, double bh, double dy)
{
    double y = 0;
    while (GetWaterVolume(n1, n2, r1, r2, bh, y, 100) < wv)
        y += dy;
    
    return y;
}

