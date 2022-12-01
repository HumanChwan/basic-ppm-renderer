#include <stdio.h>
#include <string.h>

#include "renderer.h"

#define HEIGHT 1080
#define WIDTH 1920

#define BACKGROUND_COLOR 0xFF303030
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

int min(int a, int b) { return (a > b ? b : a); }

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

int main(void) {
    simple_circle();
    /* checkerboard(); */
    /* lerpy_circle(); */
    return 0;
}
