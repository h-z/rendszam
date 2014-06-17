#ifndef _LIBHZ_H_
#define _LIBHZ_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

//#include <jpeglib.h>
#include "/opt/inlcude/jpeglib.h"

#define MAX(x,y) x > y  ?  x : y
#define MIN(x,y) x < y  ?  x : y

#define HATTER 0
#define OBJEKTUM 255
#define RGB_R 0
#define RGB_G 1
#define RGB_B 2



struct _kep {
	int          width;             /* szelesseg*/
	int          height;            /* magassag   */
	int          samples_per_pixel; /* szinkodolas */
	JSAMPARRAY   pic;               /* maga a kep */
	char*        filename;
	char*        picname;
};

typedef struct _kep  KEP;

typedef struct _kep *KEPP;

struct _label 
{
	int r,g,b;
};

typedef struct _label label;


struct _pont
{
	int x,y;
};

typedef struct _pont PONT;

/*
 *	Az (xk, yk) koordinata kozepponttal 
 *	egy keresztet helyez el a kepen.
 *
 */  
KEP* kereszt(KEP* a, int xk, int yk, int v);

/*
 *	Visszaadja a binaris kep sulypontjat.
 *
 */  
PONT kozep(KEP* a);

/*
 *	Invertalja a kepet
 *
 */  
KEP* iminverse(KEP* a);

/*
 *	A logikai a & b muveletet vegzi
 *	el a kepeken.
 *
 */  
KEP* imand(KEP* a, KEP* b);

/*
 *	A logikai a | b muveletet vegzi 
 *	el a kepeken.
 *
 */  
KEP* imor(KEP* a, KEP* b);

/*
 *	
 *
 */  
KEP* colorcut(KEP* a, int r, int g, int b, int tolerance);

/*
 *	Az adott cimkeju pixelekbol keszit egy listat
 *
 */  
PONT* findlabel(KEP* a, int l);

/*
 *	
 *
 */  
label incr(label a);

/*
 * 	A binaris kepet cimkezi fel.
 *
 */  
KEP* bwlabel(KEP* a);

/*
 * 	Dilatacio muvelete
 * 
 * 	A hasznalt strukturalo elem:
 *
 *   +
 *  +++
 *   +
 */  
KEP* imdilate(KEP* a);

/*
 * 	Erozio muvelete
 * 
 * 	A hasznalt strukturalo elem:
 *
 *   +
 *  +++
 *   +
 */  
KEP* imerode(KEP* a);

/*
 * 	Ket n*n meretu matrixot szoroz ossze.	
 *
 */  
double* multiply(double* a, double* b, int n);

/*
 * 	2D konvolucio muvlete
 *
 */  
KEP* conv2(KEP* a, double* m, int sizex, int sizey);

/*
 * 	2D konvolucio belso fuggveny
 *
 */  
KEP* conv2_rgb(KEP* a, double* m, int sizex, int sizey);

/*
 * 	2D konvolucio belso fuggveny
 *
 */  
KEP* conv2_gray(KEP* a, double* m, int sizex, int sizey);

/*
 * 	Egy kep harom szinsikjat egyesiti
 *
 */  
KEP* merge_rgb(KEP* r, KEP* g, KEP* b);

/*
 *	
 *
 */  
JSAMPLE window(JSAMPLE pixels[3][3], double m[3][3]);

/*
 *	Egy kep hisztogramjat adja vissza
 *	Visszateresi tipus: int[256]
 *
 */  
int* histogram(KEP *a);

/*
 *	Egy kep egy szinsikjat adja vissza
 *
 */  
KEP* colorimage(KEP* a, int color);

/*
 *	Belso fuggveny, a kep szelen talalhato pixelek
 *	szomszedait adja vissza helyesen
 *
 */  
JSAMPLE pixel(KEP* a, int x, int y, int i, int j);

/*
 *	Belso fuggveny, a kep szelen talalhato pixelek
 *	szomszedait adja vissza helyesen
 *
 */  
JSAMPLE pixel_0(KEP* a, int x, int y, int i, int j);

/*
 *	Hisztogram-kiegyenlites muvelet.
 *
 */  
KEP* histeq(KEP* a);

/*
 *	Tomb medianerteket adja vissza, belso fuggveny.
 *
 */  
JSAMPLE median(JSAMPLE m[], int n);

/*
 *	2D median szures muvelete
 *
 */  
KEP* medfilt2(KEP* a, int window);

/*
 *	2D median szures muvelete, belso fuggveny
 *
 */  
KEP* medfilt2_rgb(KEP* a, int window);

/*
 *	2D median szures muvelete, belso fuggveny
 *
 */  
KEP* medfilt2_gray(KEP* a, int window);

/*
 *	1D median szures
 *
 */  
KEP* medfilt(KEP* a, int window);

/*
 *	Binarizalas muvelete
 *
 */  
KEP* im2bw(KEP* a, int int_level);

/*
 *	
 *
 */  
KEP* im2bwa(KEP* a, int window);

/*
 *	
 *
 */  
KEP* im2bwhyst(KEP* a, int tl, int th, int window);

/*
 *	Ket kep kulonbsege.
 *	Visszateresi ertek: binaris kep,
 *	objektumpixel, ahol a ket kep jobban
 *	kulonbozik, mint a toleranciaszint
 *
 */  
KEP* subtract(KEP* a, KEP* b, int tolerance);

/*
 *	Kulonbsegkepzes belso fuggvenye
 *
 */  
KEP* subtract_gray(KEP* a, KEP* b, int tolerance);

/*
 *	Kulonbsegkepzes belso fuggvenye
 *
 */  
KEP* subtract_rgb(KEP* a, KEP* b, int tolerance);

/*
 *	Uj kep objektumot hoz letre
 *
 */  
KEP* imnew( int xdim, int ydim, int colnum );

/*
 *	Kep objektumot torol a memoriabol
 *
 */  
KEP* imclear( KEP* pic );

/*
 *	Kepet olvas be a hattertarolorol.
 *
 */  
KEP* imread( char* filename );

/*
 *	Kepet ir ki a hattertarolora
 *
 */  
int imwrite( KEP* bild, char* filename );

/*
 *	Kep reszletet adja vissza
 *
 */  
KEP* imcut( KEP* original, int x1, int x2, int y1, int y2 );

#endif
