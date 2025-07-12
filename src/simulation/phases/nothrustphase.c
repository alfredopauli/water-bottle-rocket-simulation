#include "simulation/simulate.h"


void SimulateNoThrustPhase(Simulation *simulation)
{
    double dt = simulation->dt;
    double time = 0;
    double y_acceleration = 0;
    double y_velocity = GetLastDoubleVector(&simulation->gas_phase_solution.y_velocity);
    double y_position = GetLastDoubleVector(&simulation->gas_phase_solution.y_position);
    double drag_force; 

    double g = simulation->physicsconst.gravity_acceleration;
    double rho_air = simulation->physicsconst.air_density;

    double mass = GetRocketVariable(&simulation->rocketvar, "dry_mass");
    double dc = GetRocketVariable(&simulation->rocketvar, "drag_coeficient");
    double ab = GetBottleSectionalArea(&simulation->rocketvar);

    while (y_position > 0)
    {
        drag_force = -1.0/2.0 * dc * rho_air * ab * y_velocity * fabs(y_velocity);
        y_velocity += (drag_force / mass - g) * dt;
        y_position += y_velocity * dt;
        time += dt;
        
        AppendDoubleVector(&simulation->nothrust_phase_solution.time, time);
        AppendDoubleVector(&simulation->nothrust_phase_solution.y_position, y_position);
        AppendDoubleVector(&simulation->nothrust_phase_solution.y_velocity, y_velocity);
    }
}

