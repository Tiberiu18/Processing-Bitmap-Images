#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "utils.h"
#include "draw.h"

void draw_Line(char* path, Pixel** pixelMat, Pixel* rgbPencil, Point* point1, Point* point2, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh)
{
	int gapX = 0, gapY = 0, xMax = 0, xMin = 0, yMax = 0, yMin = 0;
	int bigger_gap = 0;
	int* interval = (int*)calloc(2, 2 * sizeof(int));
	if (interval == NULL)
		return NULL;
	yMin = Min(point1->Col_No, point2->Col_No);
	yMax = Max(point1->Col_No, point2->Col_No);
	gapY = yMax - yMin;

	xMin = Min(point1->Row_No, point2->Row_No);
	xMax = Max(point1->Row_No, point2->Row_No);
	gapX = xMax - xMin;

	if (gapX < gapY)
	{
		bigger_gap = gapY;
		interval[0] = yMin;
		interval[1] = yMax;
	}
	else if ( gapX > gapY)
	{
		bigger_gap = gapX;
		interval[0] = xMin;
		interval[1] = xMax;
	}
	else if (gapX == gapY)
	{
		interval[0] = xMin;
		interval[1] = xMax; 
	}
	
	int* find_XorY = (int*)calloc(interval[1], (interval[1]) * sizeof(int)); 
	if (find_XorY == NULL)
		return NULL;
	int upper_lim = interval[1]; 
	int lower_lim = interval[0]; 

	//Draw initial points
	for (int i = 0; i <= line_width / 2 ; i++)
	{
		for (int j = 0; j <= line_width / 2 ; j++)
		{ 
				
			if (point1->Row_No - i >= 0 && point1->Col_No - j >= 0 && point1->Row_No + i <= bmp_ih->height && point1->Col_No + j <= bmp_ih->width)
			{
				pixelMat[point1->Row_No + i][point1->Col_No + j].red = rgbPencil->red;
				pixelMat[point1->Row_No + i][point1->Col_No + j].green = rgbPencil->green;
				pixelMat[point1->Row_No + i][point1->Col_No + j].blue = rgbPencil->blue;

				pixelMat[point1->Row_No - i][point1->Col_No - j].red = rgbPencil->red;
				pixelMat[point1->Row_No - i][point1->Col_No - j].green = rgbPencil->green;
				pixelMat[point1->Row_No - i][point1->Col_No - j].blue = rgbPencil->blue;

				pixelMat[point1->Row_No + i][point1->Col_No - j].red = rgbPencil->red;
				pixelMat[point1->Row_No + i][point1->Col_No - j].green = rgbPencil->green;
				pixelMat[point1->Row_No + i][point1->Col_No - j].blue = rgbPencil->blue;

				pixelMat[point1->Row_No - i][point1->Col_No + j].red = rgbPencil->red;
				pixelMat[point1->Row_No - i][point1->Col_No + j].green = rgbPencil->green;
				pixelMat[point1->Row_No - i][point1->Col_No + j].blue = rgbPencil->blue;
			}
		}
	}

	for (int i = 0; i <= line_width / 2; i++)
	{
		for (int j = 0; j <= line_width / 2; j++)
		{

			if (point2->Row_No - i >= 0 && point2->Col_No - j >= 0 && point2->Row_No + i <= bmp_ih->height && point2->Col_No + j <= bmp_ih->width)
			{
				pixelMat[point2->Row_No + i][point2->Col_No + j].red = rgbPencil->red;
				pixelMat[point2->Row_No + i][point2->Col_No + j].green = rgbPencil->green;
				pixelMat[point2->Row_No + i][point2->Col_No + j].blue = rgbPencil->blue;

				pixelMat[point2->Row_No - i][point2->Col_No - j].red = rgbPencil->red;
				pixelMat[point2->Row_No - i][point2->Col_No - j].green = rgbPencil->green;
				pixelMat[point2->Row_No - i][point2->Col_No - j].blue = rgbPencil->blue;

				pixelMat[point2->Row_No + i][point2->Col_No - j].red = rgbPencil->red;
				pixelMat[point2->Row_No + i][point2->Col_No - j].green = rgbPencil->green;
				pixelMat[point2->Row_No + i][point2->Col_No - j].blue = rgbPencil->blue;

				pixelMat[point2->Row_No - i][point2->Col_No + j].red = rgbPencil->red;
				pixelMat[point2->Row_No - i][point2->Col_No + j].green = rgbPencil->green;
				pixelMat[point2->Row_No - i][point2->Col_No + j].blue = rgbPencil->blue;
			}
		}
	}


	if (bigger_gap == gapY)
	{
		//(y - yA) / (yB - yA) = (x - xA) / (xB - xA)
		for (int i = interval[0]; i <= interval[1]; i++)
		{
			find_XorY[i] = ( ( ceil( ((double)(i - point1->Col_No) * (point2->Row_No - point1->Row_No) ) ) ) / ( (point2->Col_No - point1->Col_No))) + point1->Row_No;
		//		y		=							 i - xA				// yB		-     yA						xB			-		xA

		}
		for (int i = 0; i <= line_width / 2 ; i++)
		{
			for (int l = 0; l <= line_width / 2  ; l++)
			{
				upper_lim = interval[1];
				while (upper_lim >= interval[0])
				{
					if (find_XorY[upper_lim] + i <= bmp_ih->height && upper_lim + l <= bmp_ih->width && find_XorY[upper_lim] - i >= 0 && upper_lim - l >= 0)
					{
						pixelMat[find_XorY[upper_lim] + i][upper_lim + l].red = rgbPencil->red;
						pixelMat[find_XorY[upper_lim] + i][upper_lim + l].green = rgbPencil->green;
						pixelMat[find_XorY[upper_lim] + i][upper_lim + l].blue = rgbPencil->blue;

						pixelMat[find_XorY[upper_lim] - i][upper_lim - l].red = rgbPencil->red;
						pixelMat[find_XorY[upper_lim] - i][upper_lim - l].green = rgbPencil->green;
						pixelMat[find_XorY[upper_lim] - i][upper_lim - l].blue = rgbPencil->blue;

						pixelMat[find_XorY[upper_lim] + i][upper_lim - l].red = rgbPencil->red;
						pixelMat[find_XorY[upper_lim] + i][upper_lim - l].green = rgbPencil->green;
						pixelMat[find_XorY[upper_lim] + i][upper_lim - l].blue = rgbPencil->blue;

						pixelMat[find_XorY[upper_lim] - i][upper_lim + l].red = rgbPencil->red;
						pixelMat[find_XorY[upper_lim] - i][upper_lim + l].green = rgbPencil->green;
						pixelMat[find_XorY[upper_lim] - i][upper_lim + l].blue = rgbPencil->blue;
						upper_lim--;
					}
					else
						break;
				}
			}
		}
	
	}
	else if (bigger_gap == gapX  )
	{
		for (int i = interval[0]; i <= interval[1]; i++)
		{
			find_XorY[i] = (   ceil(((double)(i - point1->Row_No) * (point2->Col_No - point1->Col_No) ))  / ((point2->Row_No - point1->Row_No)) ) + point1->Col_No;
		}
		for (int i = 0; i <= line_width / 2; i++) 
		{
			for (int l = 0; l <= line_width / 2 ; l++)
			{
				upper_lim = interval[1];
				while (upper_lim >= interval[0])
				{
					if (upper_lim + l <= bmp_ih->height && find_XorY[upper_lim] + i <= bmp_ih->width && upper_lim - l >= 0 && find_XorY[upper_lim] - i >= 0)
					{
						pixelMat[upper_lim + l][find_XorY[upper_lim] + i].red = rgbPencil->red;
						pixelMat[upper_lim + l][find_XorY[upper_lim] + i].green = rgbPencil->green;
						pixelMat[upper_lim + l][find_XorY[upper_lim] + i].blue = rgbPencil->blue;

						pixelMat[upper_lim - l][find_XorY[upper_lim] - i].red = rgbPencil->red;
						pixelMat[upper_lim - l][find_XorY[upper_lim] - i].green = rgbPencil->green;
						pixelMat[upper_lim - l][find_XorY[upper_lim] - i].blue = rgbPencil->blue;

						pixelMat[upper_lim - l][find_XorY[upper_lim] + i].red = rgbPencil->red;
						pixelMat[upper_lim - l][find_XorY[upper_lim] + i].green = rgbPencil->green;
						pixelMat[upper_lim - l][find_XorY[upper_lim] + i].blue = rgbPencil->blue;

						pixelMat[upper_lim + l][find_XorY[upper_lim] - i].red = rgbPencil->red;
						pixelMat[upper_lim + l][find_XorY[upper_lim] - i].green = rgbPencil->green;
						pixelMat[upper_lim + l][find_XorY[upper_lim] - i].blue = rgbPencil->blue;
						upper_lim--;
					}
					else
						break;
				}
			}
		}
		
	}

	else if (gapX == gapY)
	{

		for (int i = interval[0]; i <= interval[1]; i++)
		{
			find_XorY[i] = i - point1->Col_No + point1->Row_No ;
		}
		for (int i = 0; i < line_width / 2; i++)
		{
			for (int l = 0; l < line_width / 2; l++)
			{
				upper_lim = interval[1];
				while (upper_lim >= interval[0])
				{
					if (upper_lim + l <= bmp_ih->height && find_XorY[upper_lim] + i <= bmp_ih->width && upper_lim - l >= 0 && find_XorY[upper_lim] - i >= 0)
					{
						pixelMat[upper_lim + l][find_XorY[upper_lim] + i].red = rgbPencil->red;
						pixelMat[upper_lim + l][find_XorY[upper_lim] + i].green = rgbPencil->green;
						pixelMat[upper_lim + l][find_XorY[upper_lim] + i].blue = rgbPencil->blue;

						pixelMat[upper_lim - l][find_XorY[upper_lim] - i].red = rgbPencil->red;
						pixelMat[upper_lim - l][find_XorY[upper_lim] - i].green = rgbPencil->green;
						pixelMat[upper_lim - l][find_XorY[upper_lim] - i].blue = rgbPencil->blue;

						pixelMat[upper_lim - l][find_XorY[upper_lim] + i].red = rgbPencil->red;
						pixelMat[upper_lim - l][find_XorY[upper_lim] + i].green = rgbPencil->green;
						pixelMat[upper_lim - l][find_XorY[upper_lim] + i].blue = rgbPencil->blue;

						pixelMat[upper_lim + l][find_XorY[upper_lim] - i].red = rgbPencil->red;
						pixelMat[upper_lim + l][find_XorY[upper_lim] - i].green = rgbPencil->green;
						pixelMat[upper_lim + l][find_XorY[upper_lim] - i].blue = rgbPencil->blue;
						upper_lim--;
					}
					else
						break;
				}
			}
		}
	}
	
	
	free(interval);
	free(find_XorY);
}

int draw_rectangle(char* path, Pixel** pixelMat, Pixel* rgbPencil, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh, Rectangle* rectDetails, Point* singlePoint)
{
	
	Point* point2 = (Point*)calloc(1, sizeof(Point));
	Point* point3 = (Point*)calloc(1, sizeof(Point));
	Point* point4 = (Point*)calloc(1, sizeof(Point));
	if (point2 == NULL || point3 == NULL || point4 == NULL)
		return NULL;
	point2->Col_No = singlePoint->Col_No + rectDetails->width;
	point2->Row_No = singlePoint->Row_No;

	point3->Col_No = point2->Col_No;
	point3->Row_No = singlePoint->Row_No + rectDetails->height;

	point4->Col_No = singlePoint->Col_No;
	point4->Row_No = singlePoint->Row_No + rectDetails->height;



	draw_Line(path, pixelMat, rgbPencil, singlePoint, point2, line_width, bmp_ih, bmp_fh);
	draw_Line(path, pixelMat, rgbPencil, point2, point3, line_width, bmp_ih, bmp_fh);
	draw_Line(path, pixelMat, rgbPencil, point3, point4, line_width, bmp_ih, bmp_fh);
	draw_Line(path, pixelMat, rgbPencil, singlePoint, point4, line_width, bmp_ih, bmp_fh);

	free(point2);
	free(point3);
	free(point4); 
	return 0;
}

int draw_triangle(char* path, Pixel** pixelmat, Pixel* rgbPencil, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh, Point* point1, Point* point2, Point* point3)
{
	draw_Line(path, pixelmat, rgbPencil, point1, point2, line_width, bmp_ih, bmp_fh);
	draw_Line(path, pixelmat, rgbPencil, point2,point3, line_width, bmp_ih, bmp_fh);
	draw_Line(path, pixelmat, rgbPencil, point1, point3, line_width, bmp_ih, bmp_fh);
	
	return  0;
}

