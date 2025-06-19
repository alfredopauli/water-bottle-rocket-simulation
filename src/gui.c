#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


void CreateGraphFromSolution(Graph *graph, Solution *solution)
{
    graph->points.size = solution->time.size;
    graph->points.X = (double *)malloc(sizeof(double) * graph->points.size);
    graph->points.Y = (double *)malloc(sizeof(double) * graph->points.size);
    memcpy(graph->points.X, solution->time.data, sizeof(double) * graph->points.size);
    memcpy(graph->points.Y, solution->y_position.data, sizeof(double) * graph->points.size);
}

void DrawGraph(Graph *graph, int x_pos, int y_pos, int width, int height)
{
    float x_max = -INFINITY, x_min = INFINITY;
    float y_max = -INFINITY, y_min = INFINITY;
    for (size_t i=0; i < graph->points.size; i++)
    {
        x_max = fmax(x_max, graph->points.X[i]); 
        x_min = fmin(x_min, graph->points.X[i]); 
        y_max = fmax(y_max, graph->points.Y[i]); 
        y_min = fmin(y_min, graph->points.Y[i]); 
    }
    
    float x_point, y_point, x_prev_point=floor(x_pos);
    Vector2 line_strip[graph->points.size];
    size_t size = 0;
    for (size_t i=0; i < graph->points.size; i++)
    {
        x_point = width * (graph->points.X[i] - x_min) / (x_max - x_min) + x_pos;
        y_point = height * (graph->points.Y[i] - y_max) / (y_min - y_max) + y_pos;
        
        if (floor(x_point) > x_prev_point)
        {
            x_prev_point = floor(x_point);
            line_strip[size] = (Vector2){ x_point, y_point }; 
            size++;
        }
    }

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
            WHITE
        );
    }
    
    text_width = MeasureTextEx(GetFontDefault(), graph->x_label, graph->label_font_size, graph->spacing).x;
    DrawText(
        graph->x_label,
        x_pos + width / 2.0 - text_width / 2.0,
        y_pos + height + graph->tick_font_size + 2.0 * graph->pad,
        graph->label_font_size,
        WHITE
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
            WHITE
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
        WHITE
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
        WHITE
    );
    
    DrawSplineLinear(line_strip, size, 2.0, GREEN); 
    DrawRectangleLines(x_pos, y_pos, width, height, GRAY);
}

void FreeGraph(Graph *graph)
{
    free(graph->points.X);
    free(graph->points.Y);
}

