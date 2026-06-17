#ifndef CONSTANTS_H
#define CONSTANTS_H

/*	Change the value of theme to toggle between dark and light theme
/	0 - Light Theme
/	1 - Dark Theme
/   2 - Pixel Art Theme
*/
#define THEME 1
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define BOUNDARY_WIDTH 1600
#define BOUNDARY_HEIGHT 800
#define NUM_OF_BOUNDARIES 16

#define VIEW_WIDTH 1280
#define VIEW_HEIGHT 720
#define VIEW_START_X 0
#define VIEW_START_Y 0

#define DRAW_WIDTH 854
#define DRAW_HEIGHT 480
#define NUM_OF_RAYS DRAW_WIDTH
#define FOV 60
#define D_SPACING ((float)FOV / (float)NUM_OF_RAYS)

#define D_R 0.0174533
#endif
