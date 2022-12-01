#ifndef PPM_RENDERER_
#define PPM_RENDERER_

#include <stdint.h>

typedef int16_t Errorno;
typedef uint32_t Color32;

Errorno save_image_to_ppm(uint32_t* pixels, const uint16_t pixel_width,
                          const uint16_t pixel_height, const char* filename);

void fill_image(uint32_t* pixels, const uint16_t pixel_width,
                const uint16_t pixel_height, Color32 color);

void render_circle(uint32_t* pixels, const uint16_t pixel_width,
                   const uint16_t pixel_height, Color32 color, int16_t center_x,
                   int16_t center_y, int16_t radius);

void render_rectangle(uint32_t* pixels, const uint16_t pixel_width,
                      const uint16_t pixel_height, Color32 color,
                      int16_t top_left_x, int16_t top_left_y, uint16_t rect_width, uint16_t rect_height);

#endif  // PPM_RENDERER_
