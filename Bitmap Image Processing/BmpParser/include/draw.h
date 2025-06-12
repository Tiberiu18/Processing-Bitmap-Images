#ifndef DRAW_H
#define DRAW_H
#include "bmp.h"

void draw_Line(char* path, Pixel** pixelMat, Pixel* rgbPencil, Point* point1, Point* point2, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh);
int draw_rectangle(char* path, Pixel** pixelMat, Pixel* rgbPencil, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh, Rectangle* rectDetails, Point* singlePoint);
int draw_triangle(char* path, Pixel** pixelmat, Pixel* rgbPencil, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh, Point* point1, Point* point2, Point* point3);

#endif // DRAW_H
