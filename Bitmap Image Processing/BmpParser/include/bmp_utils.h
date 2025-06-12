#ifndef BMP_UTILS_H
#define BMP_UTILS_H
#include "bmp.h"

Pixel** edit(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih);
void save(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih, Pixel** pixelMat);
void quit(bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih);
Pixel** flipBitMap(Pixel** pixMatOrig, int height, int width);
void insert(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih, unsigned int* yx, Pixel** pixelMat);

#endif // BMP_UTILS_H
