#define RAYGUI_IMPLEMENTATION
#include "gui/application.h"
#include <raygui.h>


void ResizeWindow(Application *app)
{
    app->width = GetScreenWidth();
    app->height = GetScreenHeight();

    app->left_panel_width = 250;
    app->left_panel_margin = 10;
    app->left_slider_height = 20;
    app->left_slider_text = 10;

    app->right_panel_width = app->width - app->left_panel_width;
    app->right_panel_margin = 10;
    app->right_button_gap = 20;
    app->right_button_width = (app->right_panel_width - 5.0 * app->right_button_gap - 2 * app->right_panel_margin) / 6.0;
    app->right_graph_gap = 50;
    app->right_graph_size = (app->right_panel_width - 2 * app->right_panel_margin - 4 * app->right_graph_gap) / 2.0;
    app->right_graph_y = app->height / 2.0 - app->right_graph_size / 2.0;

    app->simulate_button_rect.x = app->left_panel_margin;
    app->simulate_button_rect.y = app->left_panel_margin;
    app->simulate_button_rect.width = app->left_panel_width - 2 * app->left_panel_margin; 
    app->simulate_button_rect.height = 25;

    app->save_button_rect.width = 100;
    app->save_button_rect.height = 20;
    app->save_button_rect.x = app->left_panel_width + app->right_panel_width / 2 - app->save_button_rect.width / 2;
    app->save_button_rect.y = app->height - app->save_button_rect.height - 50;

    for (int i=0; i < 6; i++)
    {
        app->right_buttons_rect[i].x = app->left_panel_width + app->right_panel_margin + (app->right_button_width + app->right_button_gap) * i;
        app->right_buttons_rect[i].y = app->right_panel_margin;
        app->right_buttons_rect[i].width = app->right_button_width;
        app->right_buttons_rect[i].height = 25;
    }
}

static void UpdateGraphs(Application *app)
{
    if (app->right_scene_index == 0)
        FreeGraph(&app->bottle_graph);
    else
    {
        FreeGraph(&app->position_graph);
        FreeGraph(&app->velocity_graph);
    }

    switch (app->right_scene_index)
    {
        case 0:
            CreateGraphFromBottleRadius(&app->bottle_graph, &app->simulation.rocketvar, 100);
            break;
        case 1:
            CreateGraphFromPosition(&app->position_graph, &app->simulation.launch_tube_phase_solution);
            CreateGraphFromVelocity(&app->velocity_graph, &app->simulation.launch_tube_phase_solution);
            sprintf(app->position_graph.title, "Launch tube phase position");
            sprintf(app->velocity_graph.title, "Launch tube phase velocity");
            break;
        case 2:
            CreateGraphFromPosition(&app->position_graph, &app->simulation.water_phase_solution);
            CreateGraphFromVelocity(&app->velocity_graph, &app->simulation.water_phase_solution);
            sprintf(app->position_graph.title, "Water phase position");
            sprintf(app->velocity_graph.title, "Water phase velocity");
            break;
        case 3:
            CreateGraphFromPosition(&app->position_graph, &app->simulation.gas_phase_solution);
            CreateGraphFromVelocity(&app->velocity_graph, &app->simulation.gas_phase_solution);
            sprintf(app->position_graph.title, "Gas phase position");
            sprintf(app->velocity_graph.title, "Gas phase velocity");
            break;
        case 4:
            CreateGraphFromPosition(&app->position_graph, &app->simulation.nothrust_phase_solution);
            CreateGraphFromVelocity(&app->velocity_graph, &app->simulation.nothrust_phase_solution);
            sprintf(app->position_graph.title, "No thrust phase position");
            sprintf(app->velocity_graph.title, "No thrust phase velocity");
            break;
        case 5:
            CreateGraphFromPosition(&app->position_graph, &app->simulation.final_solution);
            CreateGraphFromVelocity(&app->velocity_graph, &app->simulation.final_solution);
            sprintf(app->position_graph.title, "Final position");
            sprintf(app->velocity_graph.title, "Final velocity");
            break;
    }
}

static void ButtonSaveSolution(Application *app)
{
    switch (app->right_scene_index)
    {
        case 1:
            SaveSolution(&app->simulation.launch_tube_phase_solution, "launch_tube_phase_position", "launch_tube_phase_velocity");
            break;
        case 2:
            SaveSolution(&app->simulation.water_phase_solution, "water_phase_position", "water_phase_velocity");
            break;
        case 3:
            SaveSolution(&app->simulation.gas_phase_solution, "gas_phase_position", "gas_phase_velocity");
            break;
        case 4:
            SaveSolution(&app->simulation.nothrust_phase_solution, "no_thrust_phase_position", "no_thrust_phase_velocity");
            break;
        case 5:
            SaveSolution(&app->simulation.final_solution, "final_position", "final_velocity");
            break;
    }
}


void SetupApplication(Application *app)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    InitWindow(800, 450, "Water Bottle Rocket Simulation");
    //GuiLoadStyle("assets/style_terminal.rgs");

    CreateSimulation(&app->simulation, 0.0001);
    SimulateLaunchTubePhase(&app->simulation);
    SimulateWaterPhase(&app->simulation);
    SimulateGasPhase(&app->simulation);
    SimulateNoThrustPhase(&app->simulation);
    CreateFinalSolution(&app->simulation);

    //app->roboto_font = LoadFont("assets/Roboto/Roboto-VariableFont_wdth,wght.ttf");
    //app->roboto_font = LoadFont("assets/Roboto/static/Roboto-Bold.ttf");

    app->background_color = (Color){ 255, 255, 255, 255 };
    app->foreground_color = (Color){ 0, 0, 0, 255 };

    ResizeWindow(app);

    app->right_scene_index = 1;

    sprintf(app->right_buttons_labels[0], "Rocket shape");
    sprintf(app->right_buttons_labels[1], "Launch tube phase");
    sprintf(app->right_buttons_labels[2], "Water Phase");
    sprintf(app->right_buttons_labels[3], "Gas phase");
    sprintf(app->right_buttons_labels[4], "No thrust phase");
    sprintf(app->right_buttons_labels[5], "Final phase");

    CreateGraphFromBottleRadius(&app->bottle_graph, &app->simulation.rocketvar, 100);
    sprintf(app->bottle_graph.title, "Bottle graph");
    app->bottle_graph.tick_points = 6;
    app->bottle_graph.pad = 2;
    strcpy(app->bottle_graph.x_label, "X [m]");
    strcpy(app->bottle_graph.y_label, "Y [m]");
    app->bottle_graph.tick_font_size = 10;
    app->bottle_graph.label_font_size = 20;
    app->bottle_graph.title_font_size = 20;
    app->bottle_graph.spacing = 2;
    app->bottle_graph.graph_color = BLUE;
    app->bottle_graph.text_color = BLACK;

    CreateGraphFromPosition(&app->position_graph, &app->simulation.launch_tube_phase_solution);
    sprintf(app->position_graph.title, "Launch tube phase position");
    app->position_graph.tick_points = 6;
    app->position_graph.pad = 2;
    strcpy(app->position_graph.x_label, "Time [s]");
    strcpy(app->position_graph.y_label, "Height [m]");
    app->position_graph.tick_font_size = 10;
    app->position_graph.label_font_size = 20;
    app->position_graph.title_font_size = 20;
    app->position_graph.spacing = 2;
    app->position_graph.graph_color = BLUE;
    app->position_graph.text_color = BLACK;

    CreateGraphFromVelocity(&app->velocity_graph, &app->simulation.launch_tube_phase_solution);
    sprintf(app->velocity_graph.title, "Launch tube phase velocity");
    app->velocity_graph.tick_points = 6;
    app->velocity_graph.pad = 2;
    strcpy(app->velocity_graph.x_label, "Time [s]");
    strcpy(app->velocity_graph.y_label, "Velocity [m/s]");
    app->velocity_graph.tick_font_size = 10;
    app->velocity_graph.label_font_size = 20;
    app->velocity_graph.title_font_size = 20;
    app->velocity_graph.spacing = 2;
    app->velocity_graph.graph_color = BLUE;
    app->velocity_graph.text_color = BLACK;
}

void RunApplication(Application *app)
{
    while (!WindowShouldClose())
    {
        ResizeWindow(app);

        BeginDrawing();
        ClearBackground(app->background_color);

        if (GuiButton(app->simulate_button_rect, "Simulate"))
        {
            ClearSimulation(&app->simulation);
            SimulateLaunchTubePhase(&app->simulation);
            SimulateWaterPhase(&app->simulation);
            SimulateGasPhase(&app->simulation);
            SimulateNoThrustPhase(&app->simulation);
            CreateFinalSolution(&app->simulation);

            UpdateGraphs(app);
        }

        for (int i=0; i < 6; i++)
            if (GuiButton(app->right_buttons_rect[i], app->right_buttons_labels[i]))
            {
                app->right_scene_index = i;
                UpdateGraphs(app);
            }

        DrawLine(app->left_panel_width, 0, app->left_panel_width, app->height, GRAY);

        Rectangle rect;
        for (size_t i=0; i < app->simulation.rocketvar.size; i++)
        {
            rect.x = app->left_panel_margin;
            rect.y = 3 * app->left_panel_margin + app->simulate_button_rect.height + i * (2 * app->left_panel_margin + app->left_slider_height);
            rect.width = app->left_panel_width - 2 * app->left_panel_margin; 
            rect.height = app->left_slider_height;

            float value = (float)app->simulation.rocketvar.values[i];
            const char *value_text = TextFormat("%.5f", app->simulation.rocketvar.values[i]);
            int text_width = MeasureText(value_text, app->left_slider_text);

            //DrawTextEx(app->roboto_font, app->simulation.rocketvar.names[i], (Vector2){ app->left_panel_margin, rect.y - app->left_slider_text - 2 }, app->left_slider_text, 1, app->foreground_color);
            //DrawTextEx(app->roboto_font, value_text, (Vector2){ app->left_panel_width - app->left_panel_margin - text_width, rect.y - app->left_slider_text - 2 }, app->left_slider_text, 1, app->foreground_color);
            DrawText(app->simulation.rocketvar.names[i], app->left_panel_margin, rect.y - app->left_slider_text - 2, app->left_slider_text, app->foreground_color);
            DrawText(value_text, app->left_panel_width - app->left_panel_margin - text_width, rect.y - app->left_slider_text - 2, app->left_slider_text, app->foreground_color);
            GuiSlider(rect, "", "", &value, app->simulation.rocketvar.min_values[i], app->simulation.rocketvar.max_values[i]);

            app->simulation.rocketvar.values[i] = value;
        }

        if (app->right_scene_index == 0)
        {
            DrawGraph(&app->bottle_graph, app->left_panel_width + app->right_panel_margin + app->right_graph_gap, app->right_graph_y, app->right_graph_size * 2 + 2 * app->right_graph_gap, app->right_graph_size);
        } else
        {
            DrawGraph(&app->position_graph, app->left_panel_width + app->right_panel_margin + app->right_graph_gap, app->right_graph_y, app->right_graph_size, app->right_graph_size);
            DrawGraph(&app->velocity_graph, app->left_panel_width + app->right_panel_margin + 3 * app->right_graph_gap + app->right_graph_size, app->right_graph_y, app->right_graph_size, app->right_graph_size);
        }

        // GuiTextBox((Rectangle){ 100, 100, 200, 20 }, text_box, 64, edit_mode);

        if (GuiButton(app->save_button_rect, "Save"))
            ButtonSaveSolution(app);

        EndDrawing();
    }
}

void CloseApplication(Application *app)
{
    FreeSimulation(&app->simulation);
    FreeGraph(&app->bottle_graph);
    FreeGraph(&app->position_graph);
    FreeGraph(&app->velocity_graph);
    CloseWindow();
}

