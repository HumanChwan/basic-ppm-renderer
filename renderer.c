#include "renderer.h"

#include <stdio.h>

void swap_coordinates(Coordinate** coordinate_a, Coordinate** coordinate_b) {
    Coordinate* temp = *coordinate_a;
    *coordinate_a = *coordinate_b;
    *coordinate_b = temp;
}
void swap_int16_t(int16_t* X, int16_t* Y) {
    int16_t temp = *X;
    *X = *Y;
    *Y = temp;
}
uint8_t valid_coordinate_for_canvas(Canvas* canvas, Coordinate* coordinate) {
    return (uint8_t)(0 <= coordinate->x && coordinate->x < canvas->width &&
                     0 <= coordinate->y && coordinate->y < canvas->height);
}
uint8_t valid_point_for_canvas(Canvas* canvas, int16_t X, int16_t Y) {
    Coordinate A = {X, Y};
    return valid_coordinate_for_canvas(canvas, &A);
}

Errorno save_image_to_ppm(uint32_t* pixels, const uint16_t pixel_width,
                          const uint16_t pixel_height, const char* filename) {
    FILE* f = fopen(filename, "wb");

    if (f == NULL) return -1;

    fprintf(f, "P6\n%d %d 255\n", pixel_width, pixel_height);

    if (ferror(f)) return fclose(f), -1;

    for (size_t i = 0; i < pixel_width * pixel_height; ++i) {
        const uint32_t pixel = pixels[i];
        uint8_t bytes[3] = {(pixel >> 8 * 0) & 0xFF, (pixel >> 8 * 1) & 0xFF,
                            (pixel >> 8 * 2) & 0xFF};
        fwrite(bytes, sizeof(bytes), 1, f);
        if (ferror(f)) return fclose(f), -1;
    }

    return fclose(f), 0;
}

Errorno save_canvas_to_ppm(Canvas* canvas, const char* filename) {
    return save_image_to_ppm(canvas->pixels, canvas->width, canvas->height,
                             filename);
}

void fill_image(uint32_t* pixels, const uint16_t pixel_width,
                const uint16_t pixel_height, Color32 color) {
    for (size_t i = 0; i < pixel_width * pixel_height; ++i) pixels[i] = color;
}
void fill_canvas(Canvas* canvas, Color32 color) {
    fill_image(canvas->pixels, canvas->width, canvas->height, color);
}

void render_circle(uint32_t* pixels, const uint16_t pixel_width,
                   const uint16_t pixel_height, Color32 color, int16_t center_x,
                   int16_t center_y, int16_t radius) {
    int16_t x0 = center_x - radius;
    int16_t y0 = center_y - radius;
    int16_t x1 = center_x + radius;
    int16_t y1 = center_y + radius;

    for (int16_t y = y0; y <= y1; ++y) {
        if (y < 0 || (int16_t)pixel_height <= y) continue;
        for (int16_t x = x0; x <= x1; ++x) {
            if (x < 0 || (int16_t)pixel_width <= x) continue;
            uint32_t dx = 1u * (x - center_x) * (x - center_x),
                     dy = 1u * (y - center_y) * (y - center_y);
            if (dx + dy <= 1u * radius * radius)
                pixels[y * pixel_width + x] = color;
        }
    }
}
void render_circle_to_canvas(Canvas* canvas, Color32 color, Coordinate* center,
                             int16_t radius) {
    render_circle(canvas->pixels, canvas->width, canvas->height, color,
                  center->x, center->y, radius);
}

void render_rectangle(uint32_t* pixels, const uint16_t pixel_width,
                      const uint16_t pixel_height, Color32 color,
                      int16_t top_left_x, int16_t top_left_y,
                      uint16_t rect_width, uint16_t rect_height) {
    for (uint16_t dy = 0; dy < rect_height; ++dy) {
        int16_t y = top_left_y + (int16_t)dy;
        if (y < 0 || (int16_t)pixel_height <= y) continue;
        for (uint16_t dx = 0; dx < rect_width; ++dx) {
            int16_t x = top_left_x + (int16_t)dx;
            if (x < 0 || (int16_t)pixel_width <= x) continue;
            pixels[y * pixel_width + x] = color;
        }
    }
}
void render_rectangle_to_canvas(Canvas* canvas, Color32 color,
                                Coordinate* top_left, uint16_t rect_width,
                                uint16_t rect_height) {
    render_rectangle(canvas->pixels, canvas->width, canvas->height, color,
                     top_left->x, top_left->y, rect_width, rect_height);
}

void render_line_to_canvas(Canvas* canvas, Coordinate* point_a,
                           Coordinate* point_b, Color32 color) {
    if (point_a->x > point_b->x) swap_coordinates(&point_a, &point_b);

    int16_t dx = point_b->x - point_a->x, dy = point_b->y - point_a->y, c;
    if (dx != 0)
        c = point_a->y - point_a->x * dy / dx;
    else {
        if (point_a->y > point_b->y) swap_coordinates(&point_a, &point_b);
        for (int16_t y = point_a->y; y <= point_b->y; ++y)
            if (valid_point_for_canvas(canvas, point_a->x, y))
                canvas->pixels[y * canvas->width + point_a->x] = color;
        return;
    }

    for (int16_t x = point_a->x; x < point_b->x; ++x) {
        int16_t y0 = x * dy / dx + c, y1 = (x + 1) * dy / dx + c;
        if (y0 > y1) swap_int16_t(&y0, &y1);
        for (int y = y0; y <= y1; ++y)
            if (valid_point_for_canvas(canvas, x, y))
                canvas->pixels[y * canvas->width + x] = color;
    }
}

// Basic MATH

int min(int a, int b) { return (a > b ? b : a); }
int max(int a, int b) { return (a > b ? a : b); }

float deg2rad(float deg) { return deg * 2.0f * PI / 360.0f; }
float rad2deg(float rad) { return rad * 360.0f / (2.0f * PI); }
