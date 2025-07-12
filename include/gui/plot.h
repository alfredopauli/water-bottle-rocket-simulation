#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "simulation/solution.h"
#include "simulation/rocket.h"

typedef struct
{
    double *X;
    double *Y;
    size_t size;
} GraphPoints;

typedef struct
{
    GraphPoints points[2];
    int graph_points;
    size_t tick_points;
    int pad;
    char x_label[32];
    char y_label[32];
    char title[32];
    int tick_font_size;
    int label_font_size;
    int title_font_size;
    int spacing;
    Color graph_color;
    Color text_color;
} Graph;

void CreateGraphFromPosition(Graph *graph, Solution *solution);
void CreateGraphFromVelocity(Graph *graph, Solution *solution);
void CreateGraphFromBottleRadius(Graph *graph, RocketVariables *rocketvar, int points);
void DrawGraph(Graph *graph, int x_pos, int y_pos, int width, int height);
void FreeGraph(Graph *graph);

