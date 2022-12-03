#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer.h"

#define HEIGHT 600
#define WIDTH 800

#define FOREGROUND_COLOR 0xFF54ff54

Color32 image[HEIGHT * WIDTH];

#define CIRCLE_RADIUS 50

void simple_circle(void) {
    fill_image(image, WIDTH, HEIGHT, 0xFF000000);

    render_circle(image, WIDTH, HEIGHT, FOREGROUND_COLOR, WIDTH / 2, HEIGHT / 2,
                  CIRCLE_RADIUS);

    Errorno err = save_image_to_ppm(image, WIDTH, HEIGHT, "simple_circle.ppm");
    if (err) fprintf(stderr, "ERROR: %s", strerror(err));
}

#define H_STRIP (HEIGHT / 20)
#define W_STRIP (WIDTH / 20)

#define ODD_COLOR 0xFF707070
#define EVEN_COLOR 0xFF454545

void checkerboard(void) {
    uint16_t strips, strip_size;
    if (HEIGHT / H_STRIP > WIDTH / W_STRIP)
        strips = WIDTH / W_STRIP, strip_size = W_STRIP;
    else
        strips = HEIGHT / H_STRIP, strip_size = H_STRIP;

    uint16_t offset_x = WIDTH / 2 - (strips * strip_size) / 2,
             offset_y = HEIGHT / 2 - (strips * strip_size) / 2;

    for (size_t y = 0; y < strips; ++y) {
        for (size_t x = 0; x < strips; ++x) {
            Color32 color = ((x + y) & 1 ? ODD_COLOR : EVEN_COLOR);
            render_rectangle(image, WIDTH, HEIGHT, color,
                             offset_x + x * strip_size,
                             offset_y + y * strip_size, strip_size, strip_size);
        }
    }

    Errorno err = save_image_to_ppm(image, WIDTH, HEIGHT, "checkerboard.ppm");
    if (err) fprintf(stderr, "ERROR: %s", strerror(err));
}

#define CIRCLE_COLOR 0xFFa0a0f2

float lerpf16(float perc, uint16_t low, uint16_t high) {
    return low + perc * (high - low);
}

void lerpy_circle(void) {
    uint16_t strips, strip_size;
    if (HEIGHT / H_STRIP > WIDTH / W_STRIP)
        strips = WIDTH / W_STRIP, strip_size = W_STRIP;
    else
        strips = HEIGHT / H_STRIP, strip_size = H_STRIP;

    uint16_t offset_x = WIDTH / 2 - (strips * strip_size) / 2,
             offset_y = HEIGHT / 2 - (strips * strip_size) / 2;

    const uint16_t radius_low = strip_size / 8, radius_high = strip_size / 2;
    printf("%d\n", radius_high);
    for (size_t y = 0; y < strips; ++y) {
        for (size_t x = 0; x < strips; ++x) {
            const uint16_t radius =
                lerpf16((min(x + y, 2 * strips - 2 - y - x)) / (1.0f * strips),
                        radius_low, radius_high);
            render_circle(image, WIDTH, HEIGHT, CIRCLE_COLOR,
                          offset_x + x * strip_size + strip_size / 2,
                          offset_y + y * strip_size + strip_size / 2, radius);
        }
    }

    Errorno err = save_image_to_ppm(image, WIDTH, HEIGHT, "lerpy_circles.ppm");
    if (err) fprintf(stderr, "ERROR: %s", strerror(err));
}

#define LINE_COLOR 0xFFa0a0f2
#define RED_LINE 0xFF0000FF
void draw_line(void) {
    Canvas canvas = {image, WIDTH, HEIGHT};
    Coordinate A = {0, 0}, D = {WIDTH, HEIGHT}, B = {WIDTH, 0}, C = {0, HEIGHT};
    render_line_to_canvas(&canvas, &A, &D, LINE_COLOR);
    render_line_to_canvas(&canvas, &B, &C, LINE_COLOR);

    Coordinate Z = {100, 450};
    render_line_to_canvas(&canvas, &A, &Z, RED_LINE);

    Errorno err = save_canvas_to_ppm(&canvas, "line.ppm");
    if (err) fprintf(stderr, "ERROR: %s", strerror(err));
}

#define BACKGROUND_COLOR 0xFF101010
#define YELLOW_CIRCLE 0xFF00FFFF
#define RED_POLYGON 0xFF0000FF
void draw_circumcircle_with_regular_polygon(Canvas* canvas, uint32_t n,
                                            int16_t side_length) {
    fill_canvas(canvas, BACKGROUND_COLOR);
    Coordinate center = {WIDTH / 2, HEIGHT / 2};

    float theta = deg2rad(360.0f / n);

    int16_t radius = side_length / sinf(theta);
    int16_t in_radius = radius * cosf(theta / 2.0f);

    // SCAM way of making a non-filled circle
    render_circle_to_canvas(canvas, YELLOW_CIRCLE, &center, in_radius);
    render_circle_to_canvas(canvas, BACKGROUND_COLOR, &center, in_radius - 1);

    Coordinate vertices[n];
    for (size_t i = 0; i < n; ++i) {
        float sin_value, cos_value;
        sincosf(i * theta, &sin_value, &cos_value);
        vertices[i] = (Coordinate){center.x + radius * sin_value,
                                   center.y - radius * cos_value};
    }

    for (size_t i = 0; i < n; ++i) {
        render_line_to_canvas(canvas, vertices + i, vertices + (i + 1) % n,
                              RED_POLYGON);
    }

    Errorno err = save_canvas_to_ppm(canvas, "polygon.ppm");
    if (err) fprintf(stderr, "ERROR: %s", strerror(err));
}

int main(int argc, char* argv[]) {
    int side_length = 10;
    if (argc == 2) side_length = atoi(argv[1]);
    Canvas canvas = {image, WIDTH, HEIGHT};
    draw_circumcircle_with_regular_polygon(&canvas, side_length, 100);
    /* draw_line(); */
    /* simple_circle(); */
    /* checkerboard(); */
    /* lerpy_circle(); */
    return 0;
}
