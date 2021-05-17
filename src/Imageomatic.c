/*
	Programming Languages and Environments - Second Assignment 2020/2021

	Imageomatic Module Body

	Students:
	Goncalo Virginia - 56773
	Antonio Duarte - 58278
*/

#include "Imageomatic.h"
#include <stdio.h>
#include <math.h>

/*** Type Int2 ***/

/* More Int2 functions, in case you need them */

/*** Type Pixel ***/

Byte negative(Byte a) {
    return MAX_COLOR - a;
}

Pixel pixelNegative(Pixel p) {
    return pixel(negative(p.red), negative(p.green), negative(p.blue));
}

int hexToInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

Pixel hexStringToPixel(String hex) {
    Byte rgb[3];

    for (int i = 0; i < 3; i++) {
        int firstHexDigit = hexToInt(hex[i*2]), secondHexDigit = hexToInt(hex[(i*2)+1]);

        if (firstHexDigit == -1 || secondHexDigit == -1) {
            return black;
        }

        rgb[i] = (firstHexDigit << 4) + secondHexDigit;
    }

    return pixel(rgb[0], rgb[1], rgb[2]);
}

Pixel searchColorsFile(String color) {
	FILE *colorsFile = fopen(colorsFileName, "r");
	String line, hex, name;

    while (fgets(line, MAX_STRING, colorsFile) != NULL) {
        sscanf(line, "%s %s", hex, name);

        if (strcmp(color, name) == 0) {
            return hexStringToPixel(hex);
        }
    }

    // If the color name isn't found in the colors file, it is then attempted to read as a hex value.
    return hexStringToPixel(color);
}

/*** TYPE Image ***/

void initialization(void) {
    
}

Int2 imageCopy(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.y = 0; i.y < n.y; i.y++) {
        for (i.x = 0; i.x < n.x; i.x++) {
            res[i.y][i.x] = img[i.y][i.x];
        }
    }

	return n;
}

Int2 imagePaint(String color, Int2 n, Image res) {
    Pixel monoColor = searchColorsFile(color);
    Int2 i;

    for (i.y = 0; i.y < n.y; i.y++) {
        for (i.x = 0; i.x < n.x; i.x++) {
            res[i.y][i.x] = monoColor;
        }
    }

    return n;
}

Int2 imageNegative(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.y = 0; i.y < n.y; i.y++) {
		for (i.x = 0; i.x < n.x; i.x++) {
			res[i.y][i.x] = pixelNegative(img[i.y][i.x]);;
		}
	}

	return n;
}

Pixel grayLevel(int distance) {
    return pixelGray(0.7 * MAX_COLOR + 0.3 * sin(distance/20.0) * MAX_COLOR);
}

Int2 imageDroplet(Int2 n, Image res) {
    Int2 i;

    for (i.y = 0; i.y < n.y; i.y++) {
        for (i.x = 0; i.x < n.x; i.x++) {
            res[i.y][i.x] = grayLevel(int2Distance(int2Half(n), i));
        }
    }

    return n;
}

Pixel maskPixel(Pixel p1, Pixel p2) {
    Pixel px;
    px.red = p1.red * (p2.red/(float)MAX_COLOR);
    px.green = p1.green * (p2.green/(float)MAX_COLOR);
    px.blue = p1.blue * (p2.blue/(float)MAX_COLOR);
    return px;
}

// pre: int2Equals(n1, n2)
Int2 imageMask(Image img1, Int2 n1, Image img2, Int2 n2, Image res) {
    Int2 i;

    for (i.y = 0; i.y < n1.y; i.y++) {
        for (i.x = 0; i.x < n1.x; i.x++) {
            res[i.y][i.x] = maskPixel(img1[i.y][i.x], img2[i.y][i.x]);
        }
    }

    return n1;
}

Int2 imageGrayscale(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.y = 0; i.y < n.y; i.y++) {
		for (i.x = 0; i.x < n.x; i.x++) {
			res[i.y][i.x] = pixelGray(pixelGrayAverage(img[i.y][i.x]));
		}
	}

	return n;
}

Pixel calculateAverage(Image img, Int2 p, Int2 n, int level) {
    Int2 i;
    int rgb[3] = {0, 0, 0}, numPixels = 0;

    for (i.y = p.y - level; i.y <= p.y + level; i.y++) {
        for (i.x = p.x - level; i.x <= p.x + level; i.x++) {
            if (i.y >= 0 && i.y < n.y && i.x >= 0 && i.x < n.x) {
                Pixel px = img[i.y][i.x];
                rgb[0] += px.red;
                rgb[1] += px.green;
                rgb[2] += px.blue;
                numPixels++;
            }
        }
    }

    return pixel(rgb[0]/numPixels, rgb[1]/numPixels, rgb[2]/numPixels);
}

Int2 imageBlur(Image img, Int2 n, int level, Image res) {
    Int2 i;

    for (i.y = 0; i.y < n.y; i.y++) {
        for (i.x = 0; i.x < n.x; i.x++) {
            res[i.y][i.x] = calculateAverage(img, i, n, level);
        }
    }

    return n;
}

Int2 imageRotation90(Image img, Int2 n, Image res) {
  Int2 i;
  int max_y = n.y - 1;

	for (i.y = 0; i.y < n.y; i.y++) {
		for (i.x = 0; i.x < n.x; i.x++) {
			Pixel pixel = img[i.x][i.y];
			res[max_y][i.x] = pixel;
		}
		max_y -= 1;
	}

  return int2(n.y, n.x);
}

Byte nearestColor(int value, int interval) {
    return value - (value % interval);
}

Pixel posterizePixel(Pixel p, int interval) {
    return pixel(nearestColor(p.red, interval), 
            nearestColor(p.green, interval), nearestColor(p.blue, interval));
}

Int2 imagePosterize(Image img, Int2 n, int factor, Image res) {
    Int2 i;
    int interval = (MAX_COLOR + 1) >> factor;

    for (i.y = 0; i.y < n.y; i.y += 1) {
        for (i.x = 0; i.x < n.x; i.x += 1) {
            res[i.y][i.x] = posterizePixel(img[i.y][i.x], interval);
        }
    }

    return n;
}

Int2 imageHalf(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.y = 0; i.y < n.y; i.y += 2) {
		for (i.x = 0; i.x < n.x; i.x += 2) {
			res[i.y/2][i.x/2] = img[i.y][i.x];
		}
	}

	return int2(n.x/2, n.y/2);
}

void drawAxis(Int2 n, Image res) {
	imagePaint("white", n, res);

	Int2 i, m = int2Half(n);

	for (i.y = 0; i.y < n.y; i.y++) {
		res[i.y][m.x] = black;
	}

	for (i.x = 0; i.x < n.x; i.x++) {
		res[m.y][i.x] = black;
	}
}

Int2 imageFunctionPlotting(DoubleFun fun, int scale, Int2 n, Image res) {
	drawAxis(n, res);

	Int2 i;
	int yLevel = n.y/2;

	for (i.x = 0; i.x < n.x; i.x++) {
		res[yLevel + (int)(fun(i.x) * scale)][i.x] = black;
	}

	return n;
}

Int2 imageOrderedDithering(Image img, Int2 n, Image res) {
	#define INDEX_SIDE  8

	Byte indexMatrix[INDEX_SIDE][INDEX_SIDE] = {
					{ 0, 32,  8, 40,  2, 34, 10, 42}, 
					{48, 16, 56, 24, 50, 18, 58, 26},
					{12, 44,  4, 36, 14, 46,  6, 28},
					{60, 28, 52, 20, 62, 30, 54, 22},
					{ 3, 35, 11, 43,  1, 33,  9, 41},
					{51, 19, 59, 27, 49, 17, 57, 25},
					{15, 47,  7, 39, 13, 45,  5, 37},
					{63, 31, 55, 23, 61, 29, 53, 21}
	};

	return int2Error;
}

Int2 imageSteganography(Image img, Int2 n, String s, Image res) {




	return n;
}

void imageTests(void) {
	static Image img, img2, res;
	Int2 n;

	// Q
	n = imageLoad("img/frutos.png", img);
	n = imageGrayscale(img, n, res);
	imageStore("img/cinzento.png", res, n);

	// N
	n = imageLoad("img/frutos.png", img);
	n = imageNegative(img, n, res);
	imageStore("img/negativo.png", res, n);	

	// H
	n = imageLoad("img/frutos.png", img);
	n = imageHalf(img, n, res);
	imageStore("img/metade.png", res, n);

	// P
	n = int2(512, 512);
	n = imagePaint("green", n, res);
	imageStore("img/pintar.png", res, n);

	// R
	n = imageLoad("img/frutos.png", img);
	n = imageRotation90(img, n, res);
	imageStore("img/rotacao_90.png", res, n);

	// O
	n = imageLoad("img/frutos.png", img);
	n = imagePosterize(img, n, 3, res);
	imageStore("img/poster.png", res, n);

	// G
	n = int2(512, 512);
	n = imageDroplet(n, res);
	imageStore("img/goticula.png", res, n);

	// D
	n = imageLoad("img/frutos.png", img);
	n = imageBlur(img, n, 5, res);
	imageStore("img/desfocado.png", res, n);

	// M
	n = imageLoad("img/frutos.png", img);
	n = imageDroplet(n, img2);
	n = imageMask(img, n, img2, n, res);
	imageStore("img/mascarar.png", res, n);

	// F
	n = int2(512, 512);
	n = imageFunctionPlotting(sin, 50, n, res);
	imageStore("img/funcao.png", res, n);

	// T
	n = imageLoad("img/frutos.png", img);
	n = imageOrderedDithering(img, n, res);
	imageStore("img/matizacao.png", res, n);

	// E
	n = imageLoad("img/frutos.png", img);
	n = imageSteganography(img, n, "atacamos ao amanhecer", res);
	imageStore("img/esteganografia.png", res, n);
}

