#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>															//	and GLUT
char fileNameRoot[]="./ppm/frame";
int frameNo = 0;

void WriteToFile()
	{ // WriteToFile()
	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	int nPixelBytes = 3;
	int nRowBytes = windowWidth * nPixelBytes;
	int nMapBytes = windowWidth * windowHeight * 3;								//	compute size of pixel map
	unsigned char *pixelMap = (unsigned char *) malloc(nMapBytes);						//	allocate memory for pixel map
	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelMap);	//	copy from the frame buffer
	char fileName[100];

	sprintf(fileName, "%s%03d.ppm", fileNameRoot, frameNo++);
	char ppmHeader[100];
	sprintf(ppmHeader, "P6\n%d %d\n255\n", windowWidth, windowHeight);				//	prepare the PPM header
	FILE *outFile = fopen(fileName, "wb");											//	prepare binary file for writing
	if (outFile == NULL) { printf("Unable to write file %s\n", fileName);	return; }				//	error-check
	fwrite(ppmHeader, strlen(ppmHeader), 1, outFile);								//	write the PPM header
	unsigned char *pixelRow;
	for (pixelRow = pixelMap + nMapBytes - nRowBytes; pixelRow >= pixelMap; pixelRow  -= nRowBytes)
		fwrite(pixelRow, nRowBytes, 1, outFile);
	fclose(outFile);
	} // WriteToFile()


	
	
