#ifndef FONTS_H
#define FONTS_H

#include <stdbool.h>
#include "hal.h"


typedef struct{
  uint32_t num_lines;
  VideoLine lines[6];
}VideoFont;

void fonts_init(void);
VideoFont* fonts_char_to_font(uint8_t);
bool fonts_is_implemented(uint8_t);

#endif
