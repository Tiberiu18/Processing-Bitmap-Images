#include <stdlib.h>
#include "bmp.h"
#include "fill.h"

void paint(Pixel** pixelMat, int height, int width, int x, int y, Pixel* rgbPencil, Pixel* origColor)
{
	if (y >= width || x >= height )
		return;
	if (y < 0 || x < 0)
		return;
	if (pixelMat[x][y].blue == origColor->blue && pixelMat[x][y].red == origColor->red && pixelMat[x][y].green == origColor->green)
	{
		pixelMat[x][y].blue = rgbPencil->blue;
		pixelMat[x][y].red = rgbPencil->red;
		pixelMat[x][y].green = rgbPencil->green;
		paint(pixelMat, height, width, x + 1, y, rgbPencil, origColor);
		paint(pixelMat, height, width, x - 1, y, rgbPencil, origColor);
		paint(pixelMat, height, width, x, y - 1, rgbPencil, origColor);
		paint(pixelMat, height, width, x, y + 1, rgbPencil, origColor);
	}
}



Pixel** fill(Pixel** pixelMat, int imageSize, int width, int x, int y, Pixel* rgbPencil)
{
	Pixel* origColor = (Pixel*)calloc(1, sizeof(Pixel));
	if (origColor == NULL)
		return NULL;
	origColor->blue = pixelMat[x][y].blue;
	origColor->red = pixelMat[x][y].red;
	origColor->green = pixelMat[x][y].green;


	if (rgbPencil->blue != pixelMat[x][y].blue || rgbPencil->green != pixelMat[x][y].green || rgbPencil->red != pixelMat[x][y].red)
	{
		paint(pixelMat, imageSize,width, x, y, rgbPencil, origColor);
	}
	
	return pixelMat;
}
