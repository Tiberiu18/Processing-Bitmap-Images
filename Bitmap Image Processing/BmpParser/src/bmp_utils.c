#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "utils.h"
#include "bmp_utils.h"

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


