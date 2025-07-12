#include "simulation/simulate.h"


static double B(double n1, double n2, double r1, double r2, double bh, double z_max, double dz)
{
    double z = 0; 
    double integral_value = 0;

    for (z=0; z < z_max; z += dz)
        integral_value += 
            (GetRocketSection(n1, n2, r1, r2, bh, 0) 
             / GetRocketSection(n1, n2, r1, r2, bh, z)) * dz;

    return integral_value;
}

void SimulateWaterPhase(Simulation *simulation)
{
    double dt = simulation->dt;
    double time = 0;
    double y_position = GetLastDoubleVector(&simulation->launch_tube_phase_solution.y_position);
    double y_velocity = GetLastDoubleVector(&simulation->launch_tube_phase_solution.y_position);
    double y_acceleration = 0;
    double initial_pressure = simulation->launch_tube_phase_solution.pressure;
    double initial_temperature = simulation->launch_tube_phase_solution.temperature;
    double uout = 0;
    double d_uout = 0;
    double d_h = 0;
    double thrust_force;
    double intern_force;
    double drag_force;
    
    double rho_water = simulation->physicsconst.water_density;
    double rho_air = simulation->physicsconst.air_density;
    double g = simulation->physicsconst.gravity_acceleration;
    double patm = simulation->physicsconst.atmospheric_pressure;
    double gamma = simulation->physicsconst.specific_heat_ratio;
    
    double mb = GetRocketVariable(&simulation->rocketvar, "dry_mass");
    double dc = GetRocketVariable(&simulation->rocketvar, "drag_coeficient");
    double ab = GetBottleSectionalArea(&simulation->rocketvar);
    double vb = GetRocketVariable(&simulation->rocketvar, "bottle_volume");
    double vw0 = GetRocketVariable(&simulation->rocketvar, "initial_water_volume");
    double n1 = GetRocketVariable(&simulation->rocketvar, "nozzle_size_1");
    double n2 = GetRocketVariable(&simulation->rocketvar, "nozzle_size_2");
    double r1 = GetRocketVariable(&simulation->rocketvar, "bottle_radius_1");
    double r2 = GetRocketVariable(&simulation->rocketvar, "bottle_radius_2");
    double bh = GetRocketVariable(&simulation->rocketvar, "bottle_height");
    double vw;
    double rs;
    
    double ab0 = GetRocketSection(n1, n2, r1, r2, bh, 0); 
    double h = GetWaterLevel(vw0, n1, n2, r1, r2, bh, 0.01);

    while (h > 0)
    {
        vw = GetWaterVolume(n1, n2, r1, r2, bh, h, 100);
        rs = GetRocketSection(n1, n2, r1, r2, bh, h);

        thrust_force = rho_water * ab0 * pow(uout, 2);
        intern_force = -rho_water * ab0 * 
            (h * d_uout + (ab0 / rs) * pow(uout, 2));
        drag_force = -1/2 * dc * rho_air * ab * y_velocity * fabs(y_velocity);
        
        y_acceleration = (thrust_force + intern_force + drag_force) 
            / (mb + rho_air * (vb - vw0) + rho_water * vw) 
            - g;
        y_velocity += y_acceleration * dt;
        y_position += y_velocity * dt;
        
        d_uout = 
            -(1.0/2.0 * (pow(ab0 / rs, 2) - 1)) * pow(uout, 2)
            -pow((vb - vw0) / (vb - vw), gamma) * initial_pressure / rho_water + 
            patm / rho_water - (y_acceleration + g) * h;
        d_uout /= B(n1, n2, r1, r2, bh, h, 0.01);
        uout += d_uout * dt;
        
        d_h = ab0 * uout / rs;
        h += d_h * dt;
        
        AppendDoubleVector(&simulation->water_phase_solution.time, time);
        AppendDoubleVector(&simulation->water_phase_solution.y_position, y_position);
        AppendDoubleVector(&simulation->water_phase_solution.y_velocity, y_velocity);
        
        time += dt;
    }
    
    simulation->water_phase_solution.pressure = initial_pressure * pow((vb - vw0) / vb, gamma); 
    simulation->water_phase_solution.temperature = initial_temperature * pow((vb - vw0) / vb, 1 - gamma);
}

