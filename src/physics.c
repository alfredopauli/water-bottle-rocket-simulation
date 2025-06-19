#include "physics.h"


void LoadPhysicsConstants(PhysicsConstants *constants)
{
    constants->atmospheric_pressure = 1.01e5;
    constants->specific_heat_ratio = 7.0 / 5.0;
    constants->gravity_acceleration = 9.8;
    constants->water_density = 997;
    constants->air_density = 1.225;
    constants->gas_constant = 8.314/0.028;
}
