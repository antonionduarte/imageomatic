/*
	Linguagens e Ambientes de Programação - Projeto de 2020/2021

	Imageomatic module body

	largura maxima = 100 colunas
	tab = 4 espaços
	0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

	Aluno 1: 56773, Goncalo Virginia
	Aluno 2: 58278, Antonio Duarte

	Comentarios:
*/

#include "Imageomatic.h"
#include <stdio.h>

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
        return ch - '0';
    }
    if (c >= 'A' && c <= 'F') {
        return ch - 'A' + 10;
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

        rgb[i] = (firstDigit << 4) + secondDigit;
    }

    return pixel(rgb[0], rgb[1], rbg[2]);
}

Pixel searchColorsFile(String color) {
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
    FILE *colorsFile = fopen(colorsFileName, "r");
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

Int2 imageDroplet(Int2 n, Image res) {
	return int2Error;
}

Int2 imageMask(Image img1, Int2 n1, Image img2, Int2 n2, Image res) { // pre: int2Equals(n1, n2)
	return int2Error;
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

Int2 imageBlur(Image img, Int2 n, int nivel, Image res) {
	return int2Error;
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

Int2 imagePosterize(Image img, Int2 n, int factor, Image res) {
	return int2Error;
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

Int2 imageFunctionPlotting(DoubleFun fun, int scale, Int2 n, Image res) {
	return int2Error;
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
	return int2Error;
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

