#pragma once
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <raylib.h>
#include "simulation/simulate.h"
#include "plot.h"

typedef struct
{
    int width;
    int height;
    Simulation simulation;

    Color background_color;
    Color foreground_color;
    
    float left_panel_width;
    float left_panel_margin;
    float left_slider_height;
    float left_slider_text;

    float right_panel_width;
    float right_panel_margin;
    float right_button_gap;
    float right_button_width;
    float right_graph_y;
    float right_graph_size;
    float right_graph_gap;
    int right_scene_index;
    
    Rectangle simulate_button_rect;

    Rectangle right_buttons_rect[6];
    char right_buttons_labels[6][32];
    
    Rectangle save_button_rect;

    Graph bottle_graph;
    Graph position_graph;
    Graph velocity_graph;
} Application;

void SetupApplication(Application *app);
void RunApplication(Application *app);
void CloseApplication(Application *app);

