#pragma once

typedef struct
{
    double atmospheric_pressure;
    double specific_heat_ratio;
    double gravity_acceleration;
    double water_density;
    double air_density;
    double gas_constant;
} PhysicsConstants;

void LoadPhysicsConstants(PhysicsConstants *constants);

