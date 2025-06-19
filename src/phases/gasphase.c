#include "simulate.h"


void SimulateGasPhase(Simulation *simulation)
{
    double dt = simulation->dt;
    double time = 0;
    double y_position = GetLastDoubleVector(&simulation->water_phase_solution.y_position);
    double y_velocity = GetLastDoubleVector(&simulation->water_phase_solution.y_velocity);
    double y_acceleration = 0;
    double initial_pressure = simulation->water_phase_solution.pressure;
    double initial_temperature = simulation->water_phase_solution.temperature;
    double pressure = initial_pressure;
    double rho;
    double thrust_force;
    double drag_force;
    
    double rho_air = simulation->physicsconst.air_density;
    double g = simulation->physicsconst.gravity_acceleration;
    double patm = simulation->physicsconst.atmospheric_pressure;
    double gamma = simulation->physicsconst.specific_heat_ratio;
    double rm = simulation->physicsconst.gas_constant;

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
    double ab0 = GetRocketSection(n1, n2, r1, r2, bh, 0);
    
    double initial_rho = rho_air * (vb - vw0) / vb;
    
    double tau = 
        vb / (ab0 * sqrt(gamma * rm * initial_temperature))
        * (2.0 / (gamma - 1.0))
        * pow((gamma + 1.0) / 2.0, (gamma + 1.0) / (2.0 * (gamma - 1.0)));

    while (pressure > patm)
    {
        pressure = initial_pressure * pow(1 + time/tau, (2.0 * gamma) / (1.0 - gamma));
        rho = pow(pressure / initial_pressure, 1.0 / gamma) * initial_rho;

        thrust_force = 
            2.0 * pressure * ab0 * pow(2.0 / (gamma + 1), 1.0 / (gamma - 1.0)) 
            - pressure * ab0;

        drag_force = -1.0/2.0 * dc * rho_air * ab * y_velocity * fabs(y_velocity);

        y_acceleration = (thrust_force + drag_force) / (mb + rho * vb) - g;
        y_velocity += y_acceleration * dt;
        y_position += y_velocity * dt;
        time += dt;
        
        AppendDoubleVector(&simulation->gas_phase_solution.time, time);
        AppendDoubleVector(&simulation->gas_phase_solution.y_position, y_position);
        AppendDoubleVector(&simulation->gas_phase_solution.y_velocity, y_velocity);
    }
    
}

