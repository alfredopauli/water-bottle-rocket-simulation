#include "simulation/simulate.h"


void SimulateLaunchTubePhase(Simulation *simulation)
{
    double dt = simulation->dt;
    double time = 0;
    double y_position = 0;
    double y_velocity = 0;
    double y_acceleration;
    double pressure;
    double rho;
    double thurst_force;
    double drag_force;
    
    double patm = simulation->physicsconst.atmospheric_pressure;
    double gamma = simulation->physicsconst.specific_heat_ratio;
    double rho_air = simulation->physicsconst.air_density;
    double g = simulation->physicsconst.gravity_acceleration;
    
    double dc = GetRocketVariable(&simulation->rocketvar, "drag_coeficient");
    double vl = GetRocketVariable(&simulation->rocketvar, "launch_tube_volume");
    double vb = GetRocketVariable(&simulation->rocketvar, "bottle_volume");
    double vw0 = GetRocketVariable(&simulation->rocketvar, "initial_water_volume");
    double lt = GetRocketVariable(&simulation->rocketvar, "launch_tube_size");
    double ati = GetRocketVariable(&simulation->rocketvar, "launch_tube_section_1");
    double ato = GetRocketVariable(&simulation->rocketvar, "launch_tube_section_2");
    double ab = GetBottleSectionalArea(&simulation->rocketvar);
    double m0 = GetInitialMass(&simulation->rocketvar, &simulation->physicsconst);
    double p0 = GetRocketVariable(&simulation->rocketvar, "initial_pressure");
    double T0 = GetRocketVariable(&simulation->rocketvar, "initial_temperature");
    double vinit = GetInitialGasVolume(&simulation->rocketvar);

    while (y_position < lt)
    {
        rho = vinit / (vinit + y_position * ato);
        pressure = p0 * pow(rho, gamma);

        drag_force = -1.0/2.0 * dc * rho_air * ab * y_velocity * fabs(y_velocity);
        thurst_force = (pressure - patm) * ato;
        
        y_acceleration = (thurst_force + drag_force) / m0 - g;
        y_velocity += y_acceleration * dt;
        y_position += y_velocity * dt;

        AppendDoubleVector(&simulation->launch_tube_phase_solution.time, time);
        AppendDoubleVector(&simulation->launch_tube_phase_solution.y_position, y_position);
        AppendDoubleVector(&simulation->launch_tube_phase_solution.y_velocity, y_velocity);

        time += dt;
    }
    
    simulation->launch_tube_phase_solution.pressure = pressure; 
    simulation->launch_tube_phase_solution.temperature = T0 * pow(rho, gamma - 1);
}

