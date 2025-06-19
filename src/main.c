#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <string.h>

#include "simulate.h"
#include "gui.h"


// TODO: Make RocketVariables a hashmap


int main(void)
{
    InitWindow(0, 0, "Water Bottle Rocket Simulation");
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    
    Simulation sim;
    CreateSimulation(&sim, 0.0001);
    SimulateLaunchTubePhase(&sim);
    SimulateWaterPhase(&sim);
    SimulateGasPhase(&sim);
    SimulateNoThrustPhase(&sim);
    CreateFinalSolution(&sim);

    Graph launch_tube_phase_graph;
    CreateGraphFromSolution(&launch_tube_phase_graph, &sim.launch_tube_phase_solution);
    launch_tube_phase_graph.tick_points = 6;
    launch_tube_phase_graph.pad = 2;
    strcpy(launch_tube_phase_graph.x_label, "Time [s]");
    strcpy(launch_tube_phase_graph.y_label, "Height [m]");
    strcpy(launch_tube_phase_graph.title, "Launch tube phase");
    launch_tube_phase_graph.tick_font_size = 10;
    launch_tube_phase_graph.label_font_size = 10;
    launch_tube_phase_graph.title_font_size = 20;
    launch_tube_phase_graph.spacing = 2;
    
    Graph water_phase_graph;
    CreateGraphFromSolution(&water_phase_graph, &sim.water_phase_solution);
    water_phase_graph.tick_points = 6;
    water_phase_graph.pad = 2;
    strcpy(water_phase_graph.x_label, "Time [s]");
    strcpy(water_phase_graph.y_label, "Height [m]");
    strcpy(water_phase_graph.title, "Water phase");
    water_phase_graph.tick_font_size = 10;
    water_phase_graph.label_font_size = 10;
    water_phase_graph.title_font_size = 20;
    water_phase_graph.spacing = 2;
    
    Graph gas_phase_graph;
    CreateGraphFromSolution(&gas_phase_graph, &sim.gas_phase_solution);
    gas_phase_graph.tick_points = 6;
    gas_phase_graph.pad = 2;
    strcpy(gas_phase_graph.x_label, "Time [s]");
    strcpy(gas_phase_graph.y_label, "Height [m]");
    strcpy(gas_phase_graph.title, "Gas phase");
    gas_phase_graph.tick_font_size = 10;
    gas_phase_graph.label_font_size = 10;
    gas_phase_graph.title_font_size = 20;
    gas_phase_graph.spacing = 2;
    
    Graph nothrust_phase_graph;
    CreateGraphFromSolution(&nothrust_phase_graph, &sim.nothrust_phase_solution);
    nothrust_phase_graph.tick_points = 6;
    nothrust_phase_graph.pad = 2;
    strcpy(nothrust_phase_graph.x_label, "Time [s]");
    strcpy(nothrust_phase_graph.y_label, "Height [m]");
    strcpy(nothrust_phase_graph.title, "No thrust phase");
    nothrust_phase_graph.tick_font_size = 10;
    nothrust_phase_graph.label_font_size = 10;
    nothrust_phase_graph.title_font_size = 20;
    nothrust_phase_graph.spacing = 2;
    
    Graph final_graph;
    CreateGraphFromSolution(&final_graph, &sim.final_solution);
    final_graph.tick_points = 6;
    final_graph.pad = 2;
    strcpy(final_graph.x_label, "Time [s]");
    strcpy(final_graph.y_label, "Height [m]");
    strcpy(final_graph.title, "Final phase");
    final_graph.tick_font_size = 10;
    final_graph.label_font_size = 10;
    final_graph.title_font_size = 20;
    final_graph.spacing = 2;
    
    // Left panel
    int left_panel = 200;
    int left_panel_margin = 10;
    int button_height = 50;
    int slider_height = 20;

    Rectangle button_rectangle;
    button_rectangle.x = left_panel_margin;
    button_rectangle.y = left_panel_margin;
    button_rectangle.width = left_panel - 2 * left_panel_margin;
    button_rectangle.height = button_height;
    
    // Right panel
    int gap = 20;
    int container_width = width - left_panel - gap;
    int container_height = height;

    int left_container_size = height / 2.0;
    int right_container_size = width - height - left_panel - gap;
    int right_panel_margin = 35;
    
    GuiLoadStyle("assets/style_terminal.rgs");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (GuiButton(button_rectangle, "Simulate"))
        {
            ClearSimulation(&sim);
            SimulateLaunchTubePhase(&sim);
            SimulateWaterPhase(&sim);
            SimulateGasPhase(&sim);
            SimulateNoThrustPhase(&sim);
            CreateFinalSolution(&sim);
            
            CreateGraphFromSolution(&launch_tube_phase_graph, &sim.launch_tube_phase_solution);
            CreateGraphFromSolution(&water_phase_graph, &sim.water_phase_solution);
            CreateGraphFromSolution(&gas_phase_graph, &sim.gas_phase_solution);
            CreateGraphFromSolution(&nothrust_phase_graph, &sim.nothrust_phase_solution);
            CreateGraphFromSolution(&final_graph, &sim.final_solution);
        }
        
        Rectangle rect;
        for (size_t i=0; i < sim.rocketvar.size; i++)
        {
            rect.x = left_panel_margin;
            rect.y = 3 * left_panel_margin + button_height + i * (2 * left_panel_margin + slider_height);
            rect.width = left_panel - 2 * left_panel_margin;
            rect.height = slider_height;
            
            float value = (float)sim.rocketvar.values[i];
            
            const char *value_text = TextFormat("%.5f", sim.rocketvar.values[i]);
            int text_width = MeasureText(value_text, 10);

            DrawText(
                sim.rocketvar.names[i], 
                left_panel_margin,
                rect.y - 10 - 2,
                10,
                WHITE
            );
            
            DrawText(
                value_text,
                left_panel - left_panel_margin - text_width,
                rect.y - 10 - 2,
                10,
                WHITE
            );

            GuiSlider(rect, 
                "", "",
                &value,
                sim.rocketvar.min_values[i],
                sim.rocketvar.max_values[i]
            );

            sim.rocketvar.values[i] = value;
        }
        
        EndScissorMode();
        
        DrawGraph(
            &launch_tube_phase_graph, 
            left_panel + gap + right_panel_margin, 
            right_panel_margin,
            left_container_size - 2 * right_panel_margin,
            left_container_size - 2 * right_panel_margin
        );
        
        DrawGraph(
            &water_phase_graph, 
            left_panel + gap + right_panel_margin + left_container_size, 
            right_panel_margin, 
            left_container_size - 2 * right_panel_margin, 
            left_container_size - 2 * right_panel_margin
        );
        
        DrawGraph(
            &gas_phase_graph, 
            left_panel + gap + right_panel_margin, 
            right_panel_margin + left_container_size,
            left_container_size - 2 * right_panel_margin, 
            left_container_size - 2 * right_panel_margin
        );
        
        DrawGraph(
            &nothrust_phase_graph, 
            left_panel + gap + right_panel_margin + left_container_size, 
            right_panel_margin + left_container_size, 
            left_container_size - 2 * right_panel_margin, 
            left_container_size - 2 * right_panel_margin
        );
        
        DrawGraph(
            &final_graph,
            left_panel + gap + right_panel_margin + 2 * left_container_size,
            right_panel_margin + height / 2.0 - right_container_size / 2.0,
            right_container_size - 2 * right_panel_margin, 
            right_container_size - 2 * right_panel_margin
        );

        EndDrawing();
    }
    
    FreeSimulation(&sim);
    FreeGraph(&launch_tube_phase_graph);
    FreeGraph(&water_phase_graph);
    FreeGraph(&gas_phase_graph);
    FreeGraph(&nothrust_phase_graph);
    
    CloseWindow();

    return 0;
}


