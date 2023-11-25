#ifndef COLORS_H
#define COLORS_H

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define TEAL 0x0410
#define VIOLET 0xEC1D
#define OLIVE 0x5345
#define GOLD 0xFEA0
#define SILVER 0xC618
#define DARK_GREEN 0x0320
#define FOREST_GREEN 0x2444
#define CORAL 0xFBEA
#define SALMON 0xFC0E
#define ROSE 0xF8A6
#define PEACH 0xFEA6

const int palletSize = 4;
uint32_t colorPallet[palletSize] = {BLUE, CYAN, VIOLET, WHITE};
// int currentPalette = 1;
// const int colorPalletCount = 7;
// uint32_t colorPallets[colorPalletCount][palletSize] = {
//     {GREEN, YELLOW, ORANGE, RED},       {BLUE, CYAN, VIOLET, WHITE},
//     {MAGENTA, MAGENTA, VIOLET, VIOLET}, {CORAL, SALMON, SALMON, ROSE},
//     {ROSE, ROSE, ROSE, ROSE},           {GREEN, GREEN, GREEN, GREEN},
//     {WHITE, WHITE, WHITE, WHITE},
// };
#endif