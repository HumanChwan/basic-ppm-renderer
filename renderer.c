#include "renderer.h"

#include <stdio.h>

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

void fill_image(uint32_t* pixels, const uint16_t pixel_width,
                const uint16_t pixel_height, Color32 color) {
    for (size_t i = 0; i < pixel_width * pixel_height; ++i) pixels[i] = color;
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
