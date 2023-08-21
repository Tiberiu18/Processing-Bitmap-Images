
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Tema/bmp_header.h"
Pixel** edit(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih);
void save(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih, Pixel** pixelMat);
void quit(bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih);
void insert(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih, unsigned int* yx, Pixel** pixelMat);
void draw_Line(char* path, Pixel** pixelMat, Pixel* rgbPencil, Point* point1,Point* point2, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh);
int draw_rectangle(char* path, Pixel** pixelMat, Pixel* rgbPencil, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh, Rectangle* rectDetails, Point* singlePoint);
int draw_triangle(char* path, Pixel** pixelmat, Pixel* rgbPencil, unsigned int line_width, bmp_infoheader* bmp_ih, bmp_fileheader* bmp_fh, Point* point1, Point* point2, Point* point3);
Pixel** fill(Pixel** pixelMat, int imageSize, int width, int x, int y, Pixel* rgbPencil);


unsigned int getPaddingDiff(unsigned int width) // w = width
{
	unsigned int diff = 0;
	unsigned int width_with_padd = width;
	while (width_with_padd % 4 != 0)
		width_with_padd++;
	diff = width_with_padd - width;
	return diff;
}


int Min(int a, int b)
{
	if (a <= b)
		return a;
	else
		return b;
}

int Max(int a, int b)
{
	if (a >= b)
		return a;
	else
		return b;
}

void cmdParser(bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih)
{
	char buffer[256];
	char* token = NULL;
	char delim[] = " ";
	char* path = NULL;
	Pixel** pixelMat = NULL;
	Pixel* rgbDrawingPen = (Pixel*)calloc(1, 1 * sizeof(Pixel));
	unsigned int l_width = 1;
	Point* point1 = NULL ;
	Point* point2 = NULL;
	Point* point3 = NULL;
	Rectangle* rectDetails = NULL;
	Point* singlePoint = NULL;
	while (1)
	{
		if (!fgets(buffer, sizeof(buffer), stdin)) // if it can't read
		{
			printf("Comanda invalida.\n");
			break;
		}
		size_t len = strlen(buffer);
		if (len + 1 == sizeof(buffer)) // + 1 because strlen doesn't count null character,sizeof does
		{
			printf("Input invalid ( prea lung ).\n");
			exit(1); // reports the abnormal termination of the program
		}
		token = strtok(buffer, delim);
		if (token != NULL)
		{
			if (*token != '0')
				if (token[strlen(token) - 1] == '\n')
					token[strlen(token) - 1] = '\0';
		}
		if (token != NULL)
		{
			if (strcmp(token, "save") == 0)
			{
				token = strtok(NULL, delim);
				path = (char*)malloc(strlen(token) + 1);
				if (token != NULL && path != NULL)
				{
					if (token[strlen(token) - 1] == '\n')
						token[strlen(token) - 1] = '\0';
					strcpy(path, token);
				}
				save(path, bmp_fh, bmp_ih, pixelMat);
			}
			else if (strcmp(token, "edit") == 0)
			{
				token = strtok(NULL, delim);
				path = (char*)malloc(strlen(token) + 1);
				if (token != NULL && path != NULL)
				{
					if (token[strlen(token) - 1] == '\n')
						token[strlen(token) - 1] = '\0';
					strcpy(path, token);
				}
				pixelMat = edit(path, bmp_fh, bmp_ih); 
			}
			else if (strcmp(token, "insert") == 0)
			{
				token = strtok(NULL, delim);
				path = (char*)malloc(strlen(token) + 1);
				if (token != NULL && path != NULL)
					strcpy(path, token);
				unsigned int* yx = (unsigned int*)calloc(2, 2 * sizeof(int));
				int k = 0;
				while (token != NULL)
				{
					token = strtok(NULL, delim);
					if (yx != NULL && token != NULL)
						yx[k++] = atoi(token);
				}
				insert(path, bmp_fh, bmp_ih, yx, pixelMat);
			}
			else if (strcmp(token, "set") == 0)
			{
				token = strtok(NULL, delim); 
				if (token != NULL && path != NULL)
				{
					if (token[strlen(token) - 1] == '\n')
						token[strlen(token) - 1] = '\0';
				}
				if (token != NULL)
				{

					if (strcmp(token, "draw_color") == 0)
					{
						int k = 0;
						while (token != NULL && rgbDrawingPen != NULL)
						{
							if (token != NULL)
							{
								if (token[strlen(token) - 1] == '\n')
									token[strlen(token) - 1] = '\0';
							}
							token = strtok(NULL, delim);
							if (token != NULL)
							{
								int val = atoi(token);
								if (k == 0)
									rgbDrawingPen->blue = val;
								else if (k == 1)
									rgbDrawingPen->green = val;
								else if (k == 2)
									rgbDrawingPen->red = val;
							}
							k++;
						}
					}
					else if (strcmp(token, "line_width") == 0)
					{
						token = strtok(NULL, delim); // GET x value
						if (token != NULL && path != NULL)
						{
							if (token[strlen(token) - 1] == '\n')
								token[strlen(token) - 1] = '\0';
						}
						if(token != NULL)
							l_width = atoi(token); // note that x should be odd, >= 1 and <= 255
					}
				}
			}
			else if (strcmp(token, "draw") == 0)
			{
				token = strtok(NULL, delim);
				if (token != NULL && path != NULL)
				{
					if (token[strlen(token) - 1] == '\n')
						token[strlen(token) - 1] = '\0';
				}
				if (token != NULL)
				{
					if (strcmp(token, "line") == 0)
					{
						int k = 0;
						point1 = (Point*)calloc(1, sizeof(Point));
						point2 = (Point*)calloc(1, sizeof(Point));
						while (token != NULL) // GET y1 x1 y2 x2
						{
							token = strtok(NULL, delim);
							if (token != NULL && path != NULL)
							{
								if (token[strlen(token) - 1] == '\n')
									token[strlen(token) - 1] = '\0';
							}
							if (point1 != NULL && token != NULL && point2 != NULL)
							{
								if (k == 0)
									point1->Col_No = atoi(token);
								else if (k == 1)
									point1->Row_No = atoi(token);
								else if (k == 2)
									point2->Col_No = atoi(token);
								else if (k == 3)
									point2->Row_No = atoi(token);
							}
							k++;
						}
						draw_Line(path, pixelMat, rgbDrawingPen, point1, point2, l_width, bmp_ih, bmp_fh);
						free(point1);
						free(point2);
					}
					else if (strcmp(token, "rectangle") == 0)
					{
						int k = 0;
						rectDetails = (Rectangle*)calloc(1, 1 * sizeof(Rectangle));
						singlePoint = (Point*)calloc(1, 1 * sizeof(Point));
						while (token != NULL)
						{
							token = strtok(NULL, delim);
							if (token != NULL && rectDetails != NULL && singlePoint != NULL)
							{
								if (k == 0)
									singlePoint->Col_No = atoi(token);
								else if (k == 1)
									singlePoint->Row_No = atoi(token);
								else if (k == 2)
									rectDetails->width = atoi(token);
								else if (k == 3)
									rectDetails->height = atoi(token);
								k++;
							}
						}
						draw_rectangle(path, pixelMat, rgbDrawingPen, l_width, bmp_ih, bmp_fh, rectDetails, singlePoint);
						free(rectDetails);
					}
					else if (strcmp(token, "triangle") == 0)
					{
						int k = 0;
						point1 = (Point*)calloc(1,sizeof(Point));
						point2 = (Point*)calloc(1, sizeof(Point));
						point3 = (Point*)calloc(1, sizeof(Point));
						while (token != NULL)
						{
							token = strtok(NULL, delim);
							if (token != NULL && point1 != NULL && point2 != NULL && point3 != NULL)
							{
								if (k == 0 )
									point1->Col_No = atoi(token);
								else if (k == 1 )
									point1->Row_No = atoi(token);
								else if (k == 2 )
									point2->Col_No = atoi(token);
								else if (k == 3 )
									point2->Row_No = atoi(token);
								else if (k == 4 )
									point3->Col_No = atoi(token);
								else if (k == 5 )
									point3->Row_No = atoi(token);
							}
							k++;
						}
						draw_triangle(path, pixelMat, rgbDrawingPen, l_width, bmp_ih, bmp_fh, point1, point2, point3);
						free(point1);
						free(point2);
						free(point3);
						
					}
				}
			}
			else if (strcmp(token, "fill") == 0)
			{
				int k = 0;
				int y = 0;
				int x = 0;
				while (token != NULL)
				{
					token = strtok(NULL, delim);
					if (token != NULL)
					{
						if (k == 0)
							y = atoi(token);
						else if (k == 1)
							x = atoi(token);	
					}
					k++;
				}
				pixelMat = fill(pixelMat, bmp_ih->height, bmp_ih->width, x, y, rgbDrawingPen);
			}
			else if (strcmp(token, "quit") == 0)
			{
				if (path != NULL)
					free(path);
				if(singlePoint != NULL)
					free(singlePoint);
				quit(bmp_fh, bmp_ih);
			}
			else
			{
				printf("\nComanda nerecunoscuta\n");
			}
		}
	}

}

Pixel** edit(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih) // path = poza1.bmp
{
	FILE* f = fopen(path, "rb");
	if (f == NULL)
		return NULL;
	// read BFH
	fread(&bmp_fh->fileMarker1, 1, sizeof(unsigned char), f); 
	fread(&bmp_fh->fileMarker2, 1, sizeof(unsigned char), f);
	fread(&bmp_fh->bfSize, 1, sizeof(unsigned int), f);
	fread(&bmp_fh->unused1, 1, sizeof(unsigned short), f);
	fread(&bmp_fh->unused2, 1, sizeof(unsigned short), f);
	fread(&bmp_fh->imageDataOffset, 1, sizeof(unsigned int), f);
	// read BIH

	fread(&bmp_ih->biSize, 1, sizeof(unsigned int), f);
	fread(&bmp_ih->width, 1, sizeof(signed int), f);
	fread(&bmp_ih->height, 1, sizeof(signed int), f);
	fread(&bmp_ih->planes, 1, sizeof(unsigned short), f);
	fread(&bmp_ih->bitPix, 1, sizeof(unsigned short), f);
	fread(&bmp_ih->biCompression, 1, sizeof(unsigned int), f);
	fread(&bmp_ih->biSizeImage, 1, sizeof(unsigned int), f);
	fread(&bmp_ih->biXPelsPerMeter, 1, sizeof(int), f);
	fread(&bmp_ih->biYPelsPerMeter, 1, sizeof(int), f);
	fread(&bmp_ih->biClrUsed, 1, sizeof(unsigned int), f);
	fread(&bmp_ih->biClrImportant, 1, sizeof(unsigned int), f);
	if (bmp_fh->fileMarker1 != 0x42 || bmp_fh->fileMarker2 != 0x4D)
		// check if indeed it's a BMP image
	{
		fclose(f);
		return NULL;
	}
	
	fseek(f, bmp_fh->imageDataOffset, SEEK_SET); // go to the beginning of data image

	Pixel** pixelMat = (Pixel**)calloc(bmp_ih->height, bmp_ih->height * sizeof(Pixel*));
	if (pixelMat == NULL)
		return NULL;
	for (int i = 0; i < bmp_ih->height; i++)
		pixelMat[i] = (Pixel*)calloc(bmp_ih->width, bmp_ih->width * sizeof(Pixel));

	int diff_pad = getPaddingDiff(bmp_ih->width);
	for (int i = 0; i < bmp_ih->height; i++)
	{
		for (int j = 0; j < bmp_ih->width; j++)
		{
			if (j != bmp_ih->width - 1 || diff_pad == 0)
			{
				fread(&pixelMat[i][j].red, sizeof(unsigned char), 1, f);
				fread(&pixelMat[i][j].green, sizeof(unsigned char), 1, f);
				fread(&pixelMat[i][j].blue, sizeof(unsigned char), 1, f);
			}
			else if ( j == bmp_ih->width - 1 && diff_pad != 0) // if end of row
			{
				fseek(f, diff_pad, SEEK_CUR); // skip padding bytes
			}
		}
	}

	

	fclose(f);
	return pixelMat;
}

void save(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih, Pixel** pixelMat) 
{
	FILE* f = fopen(path, "wb");
	if (f != NULL)
	{
		int diff_pad = getPaddingDiff(bmp_ih->width);
		fwrite(bmp_fh, sizeof(bmp_fileheader), 1, f);
		fwrite(bmp_ih, sizeof(bmp_infoheader), 1, f);
		for (unsigned int i = 0; i < bmp_fh->imageDataOffset - sizeof(bmp_fileheader) - sizeof(bmp_infoheader); i++)
			fputc(0, f); // write 0byte from last byte of header till start of BitMap
		fseek(f, bmp_fh->imageDataOffset, SEEK_SET);
		for(int i = 0; i < bmp_ih->height; i++)
			for (int j = 0; j < bmp_ih->width; j++)
			{
				if (j != bmp_ih->width - 1 || diff_pad == 0)
				{
					fwrite(&pixelMat[i][j].red, sizeof(unsigned char), 1, f);
					fwrite(&pixelMat[i][j].green, sizeof(unsigned char), 1, f);
					fwrite(&pixelMat[i][j].blue, sizeof(unsigned char), 1, f);
				}
				else if (j == bmp_ih->width - 1 && diff_pad != 0) 
				{
					for (int k = 0; k < diff_pad; k++)
						fputc(0, f);
				}
			}
		
		fclose(f);
	}

}

void quit(bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih)
{
	free(bmp_fh);
	free(bmp_ih);
	exit(0);
}

Pixel** flipBitMap(Pixel** pixMatOrig, int height, int width)
{
	Pixel** pixelMat = (Pixel**)calloc(height, sizeof(Pixel*));
	if (pixelMat == NULL)
		return NULL;	
	for (int i = 0; i < height; i++)
		pixelMat[i] = (Pixel*)calloc(width,sizeof(Pixel));
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pixelMat[i][j].blue = pixMatOrig[height - i - 1][j].blue;
			pixelMat[i][j].red = pixMatOrig[height - i - 1][j].red;
			pixelMat[i][j].green = pixMatOrig[height - i - 1][j].green;
		}
	}
	return pixelMat;
}


void insert(char* path, bmp_fileheader* bmp_fh, bmp_infoheader* bmp_ih, unsigned int* yx, Pixel** pixelMat)
{
	
	bmp_fileheader* bmp_fhIns = (bmp_fileheader*)calloc(1, 1 * sizeof(bmp_fileheader));
	bmp_infoheader* bmp_ihIns = (bmp_infoheader*)calloc(1, 1 * sizeof(bmp_infoheader));
	Pixel** pixelMatIns = edit(path, bmp_fhIns, bmp_ihIns);
	
	if (pixelMatIns == NULL)
		return;
	if (bmp_fhIns == NULL || bmp_ihIns == NULL)
		return;
	



	for (unsigned int i = yx[1], k = 0; i < bmp_ih->height && k < bmp_ihIns->height ; i++, k++)
	{
		for (unsigned int j = yx[0], l = 0; j < bmp_ih->width && l < bmp_ihIns->width; j++, l++)
		{
			pixelMat[i][j].blue = pixelMatIns[k][l].blue;
			pixelMat[i][j].green = pixelMatIns[k][l].green;
			pixelMat[i][j].red = pixelMatIns[k][l].red;
		}
	}



	for (int i = 0; i < bmp_ihIns->height; i++)
		free(pixelMatIns[i]);
	free(pixelMatIns);
	free(bmp_fhIns);
	free(bmp_ihIns);
	return;
}


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
int main()
{
	bmp_fileheader* bmp_fh = (bmp_fileheader*)calloc(1, 1 * sizeof(bmp_fileheader));
	bmp_infoheader* bmp_ih = (bmp_infoheader*)calloc(1, 1 * sizeof(bmp_infoheader));
	cmdParser(bmp_fh, bmp_ih);
}

