#include <TFT_eSPI.h>

// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Roboto_Thin_14Bitmaps[] PROGMEM = {

    // Bitmap Data:
    0x00,                                                                                                                   // ' '
    0xAA, 0xA8, 0x20,                                                                                                       // '!'
    0xDB, 0x00,                                                                                                             // '"'
    0x24, 0x24, 0xFE, 0x48, 0x48, 0x48, 0xFE, 0x48, 0x90, 0x90,                                                             // '#'
    0x20, 0xF2, 0x14, 0x28, 0x0E, 0x02, 0x02, 0x85, 0x09, 0xE1, 0x00,                                                       // '$'
    0xE0, 0x53, 0x29, 0x15, 0x0E, 0x80, 0xB8, 0xA4, 0x52, 0x49, 0x03, 0x80,                                                 // '%'
    0x30, 0x48, 0x88, 0x50, 0x20, 0xE0, 0x92, 0x8A, 0x84, 0x7A,                                                             // '&'
    0x92, 0x00,                                                                                                             // '''
    0x24, 0x88, 0x88, 0x88, 0x88, 0x88, 0x42,                                                                               // '('
    0x84, 0x44, 0x22, 0x22, 0x22, 0x24, 0x48,                                                                               // ')'
    0x21, 0x49, 0xE2, 0x84, 0x80,                                                                                           // '*'
    0x20, 0x40, 0x87, 0xE2, 0x04, 0x08, 0x00,                                                                               // '+'
    0xA8,                                                                                                                   // ','
    0xF0,                                                                                                                   // '-'
    0x80,                                                                                                                   // '.'
    0x18, 0x41, 0x08, 0x21, 0x04, 0x10, 0x82, 0x08, 0x00,                                                                   // '/'
    0x79, 0x12, 0x14, 0x28, 0x50, 0xA1, 0x42, 0x88, 0xF0,                                                                   // '0'
    0x36, 0x84, 0x21, 0x08, 0x42, 0x10, 0x80,                                                                               // '1'
    0x79, 0x12, 0x10, 0x40, 0x82, 0x08, 0x20, 0x81, 0xF8,                                                                   // '2'
    0x79, 0x0A, 0x10, 0x43, 0x01, 0x81, 0x42, 0x84, 0xF0,                                                                   // '3'
    0x08, 0x18, 0x28, 0x28, 0x48, 0x88, 0x88, 0xFE, 0x08, 0x08,                                                             // '4'
    0x7D, 0x02, 0x04, 0x07, 0x90, 0x81, 0x42, 0x84, 0xF0,                                                                   // '5'
    0x18, 0xC2, 0x07, 0xC8, 0x50, 0xA1, 0x42, 0x44, 0xF0,                                                                   // '6'
    0xFC, 0x08, 0x20, 0x41, 0x02, 0x08, 0x10, 0x40, 0x80,                                                                   // '7'
    0x79, 0x0A, 0x14, 0x27, 0x99, 0xA1, 0x42, 0x84, 0xF0,                                                                   // '8'
    0x79, 0x12, 0x14, 0x28, 0x51, 0x9D, 0x02, 0x08, 0xE0,                                                                   // '9'
    0x80, 0x02,                                                                                                             // ':'
    0x80, 0x02, 0xA0,                                                                                                       // ';'
    0x08, 0x66, 0x20, 0x60, 0x60, 0x80,                                                                                     // '<'
    0xFC, 0x00, 0x07, 0xE0,                                                                                                 // '='
    0x83, 0x03, 0x02, 0x33, 0x08, 0x00,                                                                                     // '>'
    0xF4, 0x84, 0x21, 0x31, 0x00, 0x02, 0x00,                                                                               // '?'
    0x1F, 0x02, 0x0C, 0x40, 0x48, 0x62, 0x99, 0x29, 0x12, 0xA2, 0x2A, 0x22, 0x93, 0x28, 0xDC, 0x40, 0x06, 0x10, 0x1E, 0x00, // '@'
    0x10, 0x0C, 0x0A, 0x04, 0x82, 0x42, 0x21, 0x08, 0xFC, 0x82, 0x40, 0x80,                                                 // 'A'
    0xF9, 0x0A, 0x14, 0x2F, 0x90, 0xA1, 0x42, 0x85, 0xF0,                                                                   // 'B'
    0x3C, 0x42, 0x82, 0x80, 0x80, 0x80, 0x80, 0x82, 0x42, 0x3C,                                                             // 'C'
    0xF8, 0x84, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x84, 0xF8,                                                             // 'D'
    0xFD, 0x02, 0x04, 0x0F, 0xD0, 0x20, 0x40, 0x81, 0xF8,                                                                   // 'E'
    0xFE, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x80, 0x80, 0x80, 0x80,                                                             // 'F'
    0x3C, 0x21, 0x20, 0x50, 0x08, 0x04, 0x7A, 0x05, 0x02, 0x41, 0x1F, 0x00,                                                 // 'G'
    0x82, 0x82, 0x82, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x82, 0x82,                                                             // 'H'
    0x92, 0x49, 0x24, 0x90,                                                                                                 // 'I'
    0x04, 0x08, 0x10, 0x20, 0x40, 0xA1, 0x44, 0x88, 0xE0,                                                                   // 'J'
    0x82, 0x84, 0x88, 0x90, 0xA0, 0xD0, 0x90, 0x88, 0x84, 0x82,                                                             // 'K'
    0x81, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x81, 0xF8,                                                                   // 'L'
    0x80, 0x58, 0x1B, 0x05, 0x60, 0xAA, 0x15, 0x44, 0xA4, 0x94, 0xA2, 0x8C, 0x51, 0x08,                                     // 'M'
    0x81, 0x60, 0xB0, 0x54, 0x29, 0x14, 0x4A, 0x25, 0x0A, 0x83, 0x40, 0x80,                                                 // 'N'
    0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38,                                                             // 'O'
    0xF9, 0x0A, 0x14, 0x28, 0x5F, 0x20, 0x40, 0x81, 0x00,                                                                   // 'P'
    0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38, 0x04, 0x02,                                                 // 'Q'
    0xFC, 0x84, 0x82, 0x84, 0x84, 0xF8, 0x88, 0x84, 0x84, 0x82,                                                             // 'R'
    0x78, 0x84, 0x84, 0xC0, 0x78, 0x04, 0x02, 0x82, 0x84, 0x78,                                                             // 'S'
    0xFF, 0x08, 0x04, 0x02, 0x01, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00,                                                 // 'T'
    0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x46, 0x3C,                                                             // 'U'
    0x81, 0x41, 0x20, 0x88, 0x44, 0x42, 0x20, 0x90, 0x50, 0x18, 0x08, 0x00,                                                 // 'V'
    0x84, 0x28, 0xC2, 0x8C, 0x28, 0xA4, 0x92, 0x49, 0x24, 0x51, 0x46, 0x18, 0x61, 0x82, 0x08,                               // 'W'
    0x83, 0x21, 0x09, 0x05, 0x01, 0x80, 0xC0, 0xA0, 0x88, 0x42, 0x41, 0x80,                                                 // 'X'
    0x81, 0x41, 0x11, 0x04, 0x82, 0x80, 0xC0, 0x40, 0x20, 0x10, 0x08, 0x00,                                                 // 'Y'
    0x7E, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x40, 0x80, 0xFE,                                                             // 'Z'
    0xD2, 0x49, 0x24, 0x92, 0x4C,                                                                                           // '['
    0x82, 0x04, 0x10, 0x40, 0x82, 0x04, 0x10, 0x40, 0x80,                                                                   // '\'
    0xC9, 0x24, 0x92, 0x49, 0x2C,                                                                                           // ']'
    0x43, 0x29, 0x29, 0x00,                                                                                                 // '^'
    0xFC,                                                                                                                   // '_'
    0x88,                                                                                                                   // '`'
    0x38, 0x8A, 0x10, 0x23, 0xD8, 0xA1, 0x3E,                                                                               // 'a'
    0x81, 0x02, 0x05, 0xCC, 0x50, 0xA1, 0x42, 0x85, 0x0B, 0xE0,                                                             // 'b'
    0x72, 0x28, 0xA0, 0x82, 0x08, 0x9C,                                                                                     // 'c'
    0x04, 0x08, 0x13, 0xA8, 0xD0, 0xA1, 0x42, 0x85, 0x09, 0xF0,                                                             // 'd'
    0x72, 0x28, 0xBE, 0x82, 0x08, 0x1E,                                                                                     // 'e'
    0x32, 0x11, 0xE4, 0x21, 0x08, 0x42, 0x10,                                                                               // 'f'
    0x75, 0x1A, 0x14, 0x28, 0x50, 0xA1, 0x3E, 0x05, 0x11, 0xC0,                                                             // 'g'
    0x81, 0x02, 0x05, 0xCC, 0x50, 0xA1, 0x42, 0x85, 0x0A, 0x10,                                                             // 'h'
    0x82, 0xAA, 0xA8,                                                                                                       // 'i'
    0x20, 0x02, 0x22, 0x22, 0x22, 0x22, 0x2C,                                                                               // 'j'
    0x81, 0x02, 0x04, 0x49, 0x14, 0x30, 0x50, 0x91, 0x22, 0x30,                                                             // 'k'
    0xAA, 0xAA, 0xA8,                                                                                                       // 'l'
    0xB9, 0xCC, 0x62, 0x84, 0x28, 0x42, 0x84, 0x28, 0x42, 0x84, 0x28, 0x42,                                                 // 'm'
    0xB9, 0x8A, 0x14, 0x28, 0x50, 0xA1, 0x42,                                                                               // 'n'
    0x79, 0x0A, 0x14, 0x28, 0x50, 0xA1, 0x3C,                                                                               // 'o'
    0xB9, 0x8A, 0x14, 0x28, 0x50, 0xA1, 0x7C, 0x81, 0x02, 0x00,                                                             // 'p'
    0x75, 0x1A, 0x14, 0x28, 0x50, 0xA1, 0x3E, 0x04, 0x08, 0x10,                                                             // 'q'
    0xB6, 0x21, 0x08, 0x42, 0x10,                                                                                           // 'r'
    0x72, 0x28, 0x18, 0x18, 0x28, 0x9C,                                                                                     // 's'
    0x42, 0x38, 0x84, 0x21, 0x08, 0x41, 0x80,                                                                               // 't'
    0x85, 0x0A, 0x14, 0x28, 0x50, 0xA3, 0x3E,                                                                               // 'u'
    0x86, 0x84, 0x44, 0x48, 0x48, 0x28, 0x30, 0x10,                                                                         // 'v'
    0x88, 0xA6, 0x29, 0x92, 0x54, 0xA5, 0x19, 0x42, 0x21, 0x08,                                                             // 'w'
    0x8A, 0x45, 0x18, 0x61, 0x49, 0x22,                                                                                     // 'x'
    0x86, 0x84, 0x44, 0x48, 0x28, 0x28, 0x30, 0x10, 0x10, 0x20, 0x60,                                                       // 'y'
    0xF8, 0x10, 0x41, 0x04, 0x08, 0x20, 0x7E,                                                                               // 'z'
    0x32, 0x10, 0x84, 0x21, 0x10, 0x42, 0x10, 0x84, 0x18,                                                                   // '{'
    0xAA, 0xAA, 0xAA,                                                                                                       // '|'
    0x82, 0x08, 0x42, 0x10, 0x82, 0x21, 0x08, 0x44, 0x40                                                                    // '}'
};
const GFXglyph Roboto_Thin_14Glyphs[] PROGMEM = {
    // bitmapOffset, width, height, xAdvance, xOffset, yOffset
    {0, 1, 1, 4, 0, 0},        // ' '
    {1, 2, 10, 4, 1, -10},     // '!'
    {4, 3, 3, 5, 1, -11},      // '"'
    {6, 8, 10, 9, 1, -10},     // '#'
    {16, 7, 12, 9, 1, -11},    // '$'
    {27, 9, 10, 11, 1, -10},   // '%'
    {39, 8, 10, 10, 1, -10},   // '&'
    {49, 3, 3, 3, 0, -11},     // '''
    {51, 4, 14, 5, 1, -11},    // '('
    {58, 4, 14, 5, 0, -11},    // ')'
    {65, 7, 5, 7, 0, -10},     // '*'
    {70, 7, 7, 9, 1, -8},      // '+'
    {77, 2, 3, 4, 1, -1},      // ','
    {78, 5, 1, 5, 0, -5},      // '-'
    {79, 2, 1, 4, 1, -1},      // '.'
    {80, 6, 11, 6, 0, -10},    // '/'
    {89, 7, 10, 9, 1, -10},    // '0'
    {98, 5, 10, 9, 1, -10},    // '1'
    {105, 7, 10, 9, 1, -10},   // '2'
    {114, 7, 10, 9, 1, -10},   // '3'
    {123, 8, 10, 9, 1, -10},   // '4'
    {133, 7, 10, 9, 1, -10},   // '5'
    {142, 7, 10, 9, 1, -10},   // '6'
    {151, 7, 10, 9, 1, -10},   // '7'
    {160, 7, 10, 9, 1, -10},   // '8'
    {169, 7, 10, 9, 1, -10},   // '9'
    {178, 2, 8, 3, 1, -8},     // ':'
    {180, 2, 10, 3, 1, -8},    // ';'
    {183, 6, 7, 8, 1, -8},     // '<'
    {189, 7, 4, 9, 1, -6},     // '='
    {193, 6, 7, 8, 1, -8},     // '>'
    {199, 5, 10, 7, 1, -10},   // '?'
    {206, 12, 13, 14, 1, -10}, // '@'
    {226, 9, 10, 9, 0, -10},   // 'A'
    {238, 7, 10, 9, 1, -10},   // 'B'
    {247, 8, 10, 10, 1, -10},  // 'C'
    {257, 8, 10, 10, 1, -10},  // 'D'
    {267, 7, 10, 9, 1, -10},   // 'E'
    {276, 8, 10, 9, 1, -10},   // 'F'
    {286, 9, 10, 11, 1, -10},  // 'G'
    {298, 8, 10, 11, 2, -10},  // 'H'
    {308, 3, 10, 5, 1, -10},   // 'I'
    {312, 7, 10, 9, 1, -10},   // 'J'
    {321, 8, 10, 10, 1, -10},  // 'K'
    {331, 7, 10, 8, 1, -10},   // 'L'
    {340, 11, 10, 13, 1, -10}, // 'M'
    {354, 9, 10, 11, 1, -10},  // 'N'
    {366, 8, 10, 10, 1, -10},  // 'O'
    {376, 7, 10, 9, 1, -10},   // 'P'
    {385, 8, 12, 10, 1, -10},  // 'Q'
    {397, 8, 10, 10, 1, -10},  // 'R'
    {407, 8, 10, 9, 1, -10},   // 'S'
    {417, 9, 10, 9, 0, -10},   // 'T'
    {429, 8, 10, 10, 1, -10},  // 'U'
    {439, 9, 10, 9, 0, -10},   // 'V'
    {451, 12, 10, 14, 1, -10}, // 'W'
    {466, 9, 10, 9, 0, -10},   // 'X'
    {478, 9, 10, 9, 0, -10},   // 'Y'
    {490, 8, 10, 9, 0, -10},   // 'Z'
    {500, 3, 13, 4, 1, -11},   // '['
    {505, 6, 11, 6, 0, -10},   // '\'
    {514, 3, 13, 4, 0, -11},   // ']'
    {519, 5, 5, 7, 1, -10},    // '^'
    {523, 7, 1, 7, 0, 0},      // '_'
    {524, 3, 2, 5, 1, -11},    // '`'
    {525, 7, 8, 8, 0, -8},     // 'a'
    {532, 7, 11, 9, 1, -11},   // 'b'
    {542, 6, 8, 8, 1, -8},     // 'c'
    {548, 7, 11, 9, 1, -11},   // 'd'
    {558, 6, 8, 8, 1, -8},     // 'e'
    {564, 5, 11, 5, 0, -11},   // 'f'
    {571, 7, 11, 9, 1, -8},    // 'g'
    {581, 7, 11, 9, 1, -11},   // 'h'
    {591, 2, 11, 4, 1, -11},   // 'i'
    {594, 4, 14, 4, -1, -11},  // 'j'
    {601, 7, 11, 8, 1, -11},   // 'k'
    {611, 2, 11, 4, 1, -11},   // 'l'
    {614, 12, 8, 14, 1, -8},   // 'm'
    {626, 7, 8, 9, 1, -8},     // 'n'
    {633, 7, 8, 9, 1, -8},     // 'o'
    {640, 7, 11, 9, 1, -8},    // 'p'
    {650, 7, 11, 9, 1, -8},    // 'q'
    {660, 5, 8, 6, 1, -8},     // 'r'
    {665, 6, 8, 8, 1, -8},     // 's'
    {671, 5, 10, 5, 0, -10},   // 't'
    {678, 7, 8, 9, 1, -8},     // 'u'
    {685, 8, 8, 8, 0, -8},     // 'v'
    {693, 10, 8, 12, 1, -8},   // 'w'
    {703, 6, 8, 8, 1, -8},     // 'x'
    {709, 8, 11, 8, 0, -8},    // 'y'
    {720, 7, 8, 8, 1, -8},     // 'z'
    {727, 5, 14, 6, 1, -11},   // '{'
    {736, 2, 12, 4, 1, -10},   // '|'
    {739, 5, 14, 6, 0, -11}    // '}'
};
const GFXfont Roboto_Thin_14 PROGMEM = {
    (uint8_t *)Roboto_Thin_14Bitmaps, (GFXglyph *)Roboto_Thin_14Glyphs, 0x20, 0x7E, 17};