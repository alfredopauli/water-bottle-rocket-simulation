#include "gui/plot.h"


void CreateGraphFromPosition(Graph *graph, Solution *solution)
{
    graph->graph_points = 1;
    graph->points[0].size = solution->time.size;
    graph->points[0].X = (double *)malloc(sizeof(double) * graph->points[0].size);
    graph->points[0].Y = (double *)malloc(sizeof(double) * graph->points[0].size);
    memcpy(graph->points[0].X, solution->time.data, sizeof(double) * graph->points[0].size);
    memcpy(graph->points[0].Y, solution->y_position.data, sizeof(double) * graph->points[0].size);
}

void CreateGraphFromVelocity(Graph *graph, Solution *solution)
{
    graph->graph_points = 1;
    graph->points[0].size = solution->time.size;
    graph->points[0].X = (double *)malloc(sizeof(double) * graph->points[0].size);
    graph->points[0].Y = (double *)malloc(sizeof(double) * graph->points[0].size);
    memcpy(graph->points[0].X, solution->time.data, sizeof(double) * graph->points[0].size);
    memcpy(graph->points[0].Y, solution->y_velocity.data, sizeof(double) * graph->points[0].size);
}

void CreateGraphFromBottleRadius(Graph *graph, RocketVariables *rocketvar, int points)
{
    graph->graph_points = 2;
    graph->points[0].size = points;
    graph->points[0].X = (double *)malloc(sizeof(double) * graph->points[0].size);
    graph->points[0].Y = (double *)malloc(sizeof(double) * graph->points[0].size);
    graph->points[1].size = points;
    graph->points[1].X = (double *)malloc(sizeof(double) * graph->points[0].size);
    graph->points[1].Y = (double *)malloc(sizeof(double) * graph->points[0].size);
    
    double n1 = GetRocketVariable(rocketvar, "nozzle_size_1");
    double n2 = GetRocketVariable(rocketvar, "nozzle_size_2");
    double r1 = GetRocketVariable(rocketvar, "bottle_radius_1");
    double r2 = GetRocketVariable(rocketvar, "bottle_radius_2");
    double bh = GetRocketVariable(rocketvar, "bottle_height");
    
    float z;
    float y;
    for (int i=0; i < points; i++)
    {
        z = i * bh / points;
        y = GetRocketRadius(n1, n2, r1, r2, bh, z);
        graph->points[0].X[i] = z;
        graph->points[1].X[i] = z;
        graph->points[0].Y[i] = y;
        graph->points[1].Y[i] = -y;
    }
}

void DrawGraph(Graph *graph, int x_pos, int y_pos, int width, int height)
{
    float x_max = -INFINITY, x_min = INFINITY;
    float y_max = -INFINITY, y_min = INFINITY;
    for (int i=0; i < graph->graph_points; i++)
    {
        for (size_t j=0; j < graph->points[i].size; j++)
        {
            x_max = fmax(x_max, graph->points[i].X[j]); 
            x_min = fmin(x_min, graph->points[i].X[j]); 
            y_max = fmax(y_max, graph->points[i].Y[j]); 
            y_min = fmin(y_min, graph->points[i].Y[j]); 
        }
    }
    float delta_x = fmax(fabs(x_max), fabs(x_min)) * 0.05;
    float delta_y = fmax(fabs(y_max), fabs(y_min)) * 0.05;

    x_max += delta_x;
    x_min -= delta_x;
    y_max += delta_y;
    y_min -= delta_y;

    char tick_label[16];
    int text_width;
    for (size_t i=0; i < graph->tick_points; i++)
    {
        sprintf(tick_label, "%.2f", (i / (float)(graph->tick_points-1)) * (x_max - x_min) + x_min);

        DrawTextEx(
            GetFontDefault(),
            tick_label, 
            (Vector2){
                width * (i / (float)(graph->tick_points-1)) + x_pos, 
                y_pos + height + graph->pad
            },
            graph->tick_font_size, 
            graph->spacing,
            graph->text_color
        );
    }

    text_width = MeasureTextEx(GetFontDefault(), graph->x_label, graph->label_font_size, graph->spacing).x;
    DrawText(
        graph->x_label,
        x_pos + width / 2.0 - text_width / 2.0,
        y_pos + height + graph->tick_font_size + 2.0 * graph->pad,
        graph->label_font_size,
        graph->text_color
    );

    float max_text_width = -INFINITY;
    for (size_t i=0; i < graph->tick_points; i++)
    {
        sprintf(tick_label, "%.2f", (i / (float)(graph->tick_points-1)) * (y_min - y_max) + y_max);
        text_width = MeasureTextEx(GetFontDefault(), tick_label, graph->tick_font_size, graph->spacing).x;
        max_text_width = fmax(max_text_width, text_width);

        DrawTextEx(
            GetFontDefault(),
            tick_label, 
            (Vector2){
                x_pos - text_width - graph->pad, 
                height * (i / (float)(graph->tick_points-1)) + y_pos
            },
            graph->tick_font_size, 
            graph->spacing,
            graph->text_color
        );
    }

    text_width = MeasureTextEx(GetFontDefault(), graph->y_label, graph->label_font_size, graph->spacing).x;
    DrawTextPro(
        GetFontDefault(),
        graph->y_label, 
        (Vector2){
            x_pos - max_text_width - 2.0*graph->pad,
            y_pos + height / 2.0
        },
        (Vector2){
            text_width / 2.0,
            graph->label_font_size
        },
        -90,
        graph->label_font_size, 
        graph->spacing,
        graph->text_color
    );

    text_width = MeasureTextEx(GetFontDefault(), graph->title, graph->title_font_size, graph->spacing).x;
    DrawTextEx(
        GetFontDefault(),
        graph->title,
        (Vector2){
        x_pos + width / 2.0 - text_width / 2.0,
        y_pos - graph->title_font_size - graph->pad
        },
        graph->title_font_size,
        graph->spacing,
        graph->text_color
    );

    for (int i=0; i < graph->graph_points; i++)
    {
        float x_point, y_point, x_prev_point=floor(x_pos);
        Vector2 line_strip[graph->points[i].size];
        size_t size = 0;

        for (size_t j=0; j < graph->points[i].size; j++)
        {
            x_point = width * (graph->points[i].X[j] - x_min) / (x_max - x_min) + x_pos;
            y_point = height * (graph->points[i].Y[j] - y_max) / (y_min - y_max) + y_pos;

            if (floor(x_point) > x_prev_point)
            {
                x_prev_point = floor(x_point);
                line_strip[size] = (Vector2){ x_point, y_point };
                size++;
            }
        }

        DrawSplineLinear(line_strip, size, 2.0, graph->graph_color); 
    }
    
    DrawRectangleLines(x_pos, y_pos, width, height, GRAY);
}

void FreeGraph(Graph *graph)
{
    for (int i=0; i < graph->graph_points; i++)
    {
        free(graph->points[i].X);
        free(graph->points[i].Y);
    }
}

