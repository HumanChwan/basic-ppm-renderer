#ifndef PPM_RENDERER_
#define PPM_RENDERER_

#include <stdint.h>

typedef int16_t Errorno;
typedef uint32_t Color32;

typedef struct {
    uint32_t* pixels;
    uint16_t width, height;
} Canvas;

typedef struct {
    int16_t x, y;
} Coordinate;

void swap_coordinates(Coordinate** coordinate_a, Coordinate** coordinate_b);
void swap_int16_t(int16_t* x, int16_t* y);

uint8_t valid_coordinate_for_canvas(Canvas* canvas, Coordinate* coordinate);
uint8_t valid_point_for_canvas(Canvas* canvas, int16_t X, int16_t Y);

Errorno save_image_to_ppm(uint32_t* pixels, const uint16_t pixel_width,
                          const uint16_t pixel_height, const char* filename);
Errorno save_canvas_to_ppm(Canvas* canvas, const char* filename);

void fill_image(uint32_t* pixels, const uint16_t pixel_width,
                const uint16_t pixel_height, Color32 color);
void fill_canvas(Canvas* canvas, Color32 color);

void render_circle(uint32_t* pixels, const uint16_t pixel_width,
                   const uint16_t pixel_height, Color32 color, int16_t center_x,
                   int16_t center_y, int16_t radius);
void render_circle_to_canvas(Canvas* canvas, Color32 color, Coordinate* center,
                             int16_t radius);

void render_rectangle(uint32_t* pixels, const uint16_t pixel_width,
                      const uint16_t pixel_height, Color32 color,
                      int16_t top_left_x, int16_t top_left_y,
                      uint16_t rect_width, uint16_t rect_height);
void render_rectangle_to_canvas(Canvas* canvas, Color32 color,
                                Coordinate* top_left, uint16_t rect_width,
                                uint16_t rect_height);

void render_line_to_canvas(Canvas* canvas, Coordinate* point_a,
                           Coordinate* point_b, Color32 color);

// Basic MATH

#define PI 3.1416

int min(int a, int b);
int max(int a, int b);

float deg2rad(float deg);
float rad2deg(float rad);

#endif  // PPM_RENDERER_
