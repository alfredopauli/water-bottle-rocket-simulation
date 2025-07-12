#include "simulation/simulate.h"


void CreateSimulation(Simulation *simulation, double dt)
{
    LoadPhysicsConstants(&simulation->physicsconst);
    DefaultRocketVariables(&simulation->rocketvar, &simulation->physicsconst);
    simulation->dt = dt;
    CreateSolution(&simulation->launch_tube_phase_solution);
    CreateSolution(&simulation->water_phase_solution);
    CreateSolution(&simulation->gas_phase_solution);
    CreateSolution(&simulation->nothrust_phase_solution);
}

void ClearSimulation(Simulation *simulation)
{
    ClearSolution(&simulation->launch_tube_phase_solution);
    ClearSolution(&simulation->water_phase_solution);
    ClearSolution(&simulation->gas_phase_solution);
    ClearSolution(&simulation->nothrust_phase_solution);
    ClearSolution(&simulation->final_solution);
}

void CreateFinalSolution(Simulation *simulation)
{
    CopySolution(&simulation->final_solution, &simulation->launch_tube_phase_solution);
    JoinSolution(&simulation->final_solution, &simulation->water_phase_solution);
    JoinSolution(&simulation->final_solution, &simulation->gas_phase_solution);
    JoinSolution(&simulation->final_solution, &simulation->nothrust_phase_solution);
}

void FreeSimulation(Simulation *simulation)
{
    FreeRocketVariables(&simulation->rocketvar);
    FreeSolution(&simulation->launch_tube_phase_solution);
    FreeSolution(&simulation->water_phase_solution);
    FreeSolution(&simulation->gas_phase_solution);
    FreeSolution(&simulation->nothrust_phase_solution);
}

