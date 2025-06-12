#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "bmp_utils.h"
#include "draw.h"
#include "fill.h"
#include "utils.h"
#include "cmd.h"

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
