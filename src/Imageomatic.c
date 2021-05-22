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
#include <stdlib.h>

/*** Type Int2 ***/

/* More Int2 functions, in case you need them */

/*** Type Pixel ***/

/*
 * Inverts a specific color.
 */
Byte negative(Byte a) {
    return MAX_COLOR - a;
}

/*
 * Inverts the color of a pixel.
 */
Pixel pixelNegative(Pixel p) {
    return pixel(negative(p.red), negative(p.green), negative(p.blue));
}

/*
 * Converts an hexcode to an int.
 */
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

/*
 * Converts an hex to a pixel.
 */
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

/*
 * Searches the colors.txt file for a given color name.
 */
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

/*** Type Image ***/

void initialization(void) {
    
}

/*
 * Copies the image received in "img" to "res".
 */
Int2 imageCopy(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.x = 0; i.x < n.x; i.x++) {
		for (i.y = 0; i.y < n.y; i.y++) {
			res[i.x][i.y] = img[i.x][i.y];
		}
	}

	return n;
}

/*
 * Creates a monocromatic image with a specified color and size.
 */
Int2 imagePaint(String color, Int2 n, Image res) {
    Pixel monoColor = searchColorsFile(color);
    Int2 i;

	for (i.x = 0; i.x < n.x; i.x++) {
		for (i.y = 0; i.y < n.y; i.y++) {
			res[i.x][i.y] = monoColor;
		}
	}

	return n;
}

/*
 * Converts an image to it's negative colors.
 */
Int2 imageNegative(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.x = 0; i.x < n.x; i.x++) {
		for (i.y = 0; i.y < n.y; i.y++) {
			res[i.x][i.y] = pixelNegative(img[i.x][i.y]);;
		}
	}

	return n;
}

Pixel grayLevel(double distance) {
    return pixelGray(0.7 * MAX_COLOR + 0.3 * sin(distance/20.0) * MAX_COLOR);
}

/*
 * Creates an image with gray levels simulating the visual result of a drop of water
 * falling in the surface of a lake.
 */
Int2 imageDroplet(Int2 n, Image res) {
    Int2 i, middle = int2Half(n);

    for (i.x = 0; i.x < n.x; i.x++) {
        for (i.y = 0; i.y < n.y; i.y++) {
            res[i.x][i.y] = grayLevel(int2Distance(middle, i));
        }
    }

    return n;
}

/*
 * Calculates the resulting pixel by overlapping a a pixel over another pixel.
 */
Pixel maskPixel(Pixel p1, Pixel p2) {
    Pixel px;
    px.red = p1.red * (p2.red / (double) MAX_COLOR);
    px.green = p1.green * (p2.green / (double) MAX_COLOR);
    px.blue = p1.blue * (p2.blue / (double) MAX_COLOR);
    return px;
}

/*
 * Creates a new img (res) by overlapping "img2" over "img1"
 */
Int2 imageMask(Image img1, Int2 n1, Image img2, Int2 n2, Image res) {
    Int2 i;

	for (i.x = 0; i.x < n1.x; i.x++) {
		for (i.y = 0; i.y < n1.y; i.y++) {
			res[i.x][i.y] = maskPixel(img1[i.x][i.y], img2[i.x][i.y]);
		}
	}

	return n1;
}

/*
 * Converts an image to it's grayscale format.
 */
Int2 imageGrayscale(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.x = 0; i.x < n.x; i.x++) {
		for (i.y = 0; i.y < n.y; i.y++) {
			res[i.x][i.y] = pixelGray(pixelGrayAverage(img[i.x][i.y]));
		}
	}

	return n;
}

/*
 * Calculates the average colors of a pixel in a matrix of pixels around it.
 */
Pixel calculateAverage(Image img, Int2 p, Int2 n, int level) {
    Int2 i;
    int rgb[3] = {0, 0, 0}, numPixels = 0;

    for (i.x = p.x - level; i.x <= p.x + level; i.x++) {
		for (i.y = p.y - level; i.y <= p.y + level; i.y++) {
			if (i.x >= 0 && i.x < n.x && i.y >= 0 && i.y < n.y) {
				Pixel px = img[i.x][i.y];
				rgb[0] += px.red;
				rgb[1] += px.green;
				rgb[2] += px.blue;
				numPixels++;
			}
		}
	}

    return pixel(rgb[0] / numPixels, rgb[1] / numPixels, rgb[2] / numPixels);
}

/*
 * Creates a new image by blurring the image received in "img".
 * Blurs the image a specific amount, given by the "level" argument.
 */
Int2 imageBlur(Image img, Int2 n, int level, Image res) {
    Int2 i;

	for (i.x = 0; i.x < n.x; i.x++) {
		for (i.y = 0; i.y < n.y; i.y++) {
			res[i.x][i.y] = calculateAverage(img, i, n, level);
		}
    }

    return n;
}

/*
 * Creates a new image by performing a 90 degree right rotation on "img".
 */
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

/*
 * Finds the nearest available color in the reduced color space for the pixel.
 * Finds the nearest multiple of interval, rounding down, starting from value.
 */
Byte nearestColor(int value, int interval) {
    return value - (value % interval);
}

/*
 * Posterizes a pixel.
 */
Pixel posterizePixel(Pixel p, int interval) {
    return pixel(nearestColor(p.red, interval), 
            nearestColor(p.green, interval), nearestColor(p.blue, interval));
}

/*
 * Posterizes an image by reducing by a specified factor, the amount of available
 * colors.
 */
Int2 imagePosterize(Image img, Int2 n, int factor, Image res) {
    Int2 i;
    int interval = (MAX_COLOR + 1) >> factor;

    for (i.x = 0; i.x < n.x; i.x++) {
        for (i.y = 0; i.y < n.y; i.y++) {
            res[i.x][i.y] = posterizePixel(img[i.x][i.y], interval);
        }
    }

    return n;
}

/*
 * Creates a new image with half of the dimensions of the received one.
 */
Int2 imageHalf(Image img, Int2 n, Image res) {
	Int2 i;

	for (i.x = 0; i.x < n.x; i.x += 2) {
		for (i.y = 0; i.y < n.y; i.y += 2) {
			res[i.x/2][i.y/2] = img[i.x][i.y];
		}
	}

	return int2Half(n);
}

/*
 * Draws the (y = 0) and (x = 0) axis of the function.
 */
void drawAxis(Int2 n, Image res) {
	imagePaint("white", n, res);

	Int2 i, m = int2Half(n);

	for (i.x = 0; i.x < n.x; i.x++) {
		res[i.x][m.y] = black;
	}
	
	for (i.y = 0; i.y < n.y; i.y++) {
		res[m.x][i.y] = black;
	}
}

/*
 * Creates an image by plotting the graph of a function, with a specific scale
 * and specific dimensions.
 */
Int2 imageFunctionPlotting(DoubleFun fun, int scale, Int2 n, Image res) {
	drawAxis(n, res);

	Int2 i, center = int2Half(n);

	for (i.x = 0; i.x < n.x; i.x++) {
		i.y = (int) (center.y - fun((i.x - center.x) / (double) scale) * scale);

		if (i.y >= 0 && i.y < n.y) {
			res[i.x][i.y] = black;
		}
	}

	return n;
}

/*
 * Determines if a pixel "px" should be black or white according to a specified base value 
 * obtained from indexMatrix.
 */
Pixel ditherPixel(Pixel px, int value) {
  return pixelGrayAverage(px) / 4.0 > value ? white : black;
}

/*
 * Creates a new image by applying ordered dithering to an image.
 */
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

  Int2 i;

  for (i.x = 0; i.x < n.x; i.x++) {
    for (i.y = 0; i.y < n.y; i.y++) {
      int value = indexMatrix[i.x % INDEX_SIDE][i.y % INDEX_SIDE];
			res[i.x][i.y] = ditherPixel(img[i.x][i.y], value);
		}
	}

	return n;
}

/*
 * Converts a 7 bit ASCII char into a 6 bit ASCII char based on 
 * the AIS standard which can be found here: 
 * https://en.wikipedia.org/wiki/Six-bit_character_code
 */
char toSixBitASCII(char c) {
	char out = c;
	if (c < 0x20 || c > 0x5F) {
		if (c >= 'a' && c <= 'z') {
			out -= 0x20;
		}
		else {
			out = '?';
		}
	}
	if (out >= 0x40) {
		out -= 0x40;
	}

	return out == 0 ? '?' : out;
}

/*
 * Hides a 6 bit ASCII char into a pixels' RGB. Each color component hides
 * 2 bits of the code in their 2 least significant bits, barely altering the original image.
 */
Pixel addCodeToPixel(char code, Pixel pixel) {
	Pixel out;
	out.red = (pixel.red & 0b11111100) | (code >> 4);
	out.green = (pixel.green & 0b11111100) | ((code >> 2) & 0b11);
	out.blue = (pixel.blue & 0b11111100) | (code & 0b11);
	return out;
}

/*
 * Hides a string of chars into an images' pixels using both of the previous
 * auxiliary functions.
 */
Int2 imageSteganography(Image img, Int2 n, String s, Image res) {
	imageCopy(img, n, res);

	int i;

	for (i = 0; s[i] != '\0' && i < MAX_STRING && i < (n.x * n.y); i++) {
		res[i % n.x][i / n.x] = addCodeToPixel(toSixBitASCII(s[i]), img[i % n.x][i / n.x]);
	}

	if (i == (n.x * n.y)) {
		i--;
	}
	res[i % n.x][i / n.x] = addCodeToPixel('\0', img[i % n.x][i / n.x]);

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

