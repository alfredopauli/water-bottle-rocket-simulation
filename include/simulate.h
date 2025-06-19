#pragma once
#include "doublevector.h"
#include "physics.h"
#include "rocket.h"
#include "solution.h"

typedef struct
{
    PhysicsConstants physicsconst;
    RocketVariables rocketvar;
    double dt;
    Solution launch_tube_phase_solution;
    Solution water_phase_solution;
    Solution gas_phase_solution;
    Solution nothrust_phase_solution;
    Solution final_solution;
} Simulation;

void CreateSimulation(Simulation *simulation, double dt);
void ClearSimulation(Simulation *simulation);

void SimulateLaunchTubePhase(Simulation *simulation);
void SimulateWaterPhase(Simulation *simulation);
void SimulateGasPhase(Simulation *simulation);
void SimulateNoThrustPhase(Simulation *simulation);
void CreateFinalSolution(Simulation *simulation);

void FreeSimulation(Simulation *simulation);

