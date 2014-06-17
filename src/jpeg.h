#ifndef _JPEG_H_
#define _JPEG_H_

#include "libhz.h"

/*
 *	Uj kepet hoz letre
 *
 */
KEP* new_pic( int xdim, int ydim, int colnum );

/*
 *	Kepet torol a memoriabol
 *
 */
KEP* destroy_pic( KEP* pic );

/*
 *	Kepet betolt a hattertarolobol
 *
 */
KEP* load_pic( char* filename );

/*
 *	Kepet kiir a hattertarora
 *
 */
int write_pic( KEP* bild, char* filename );

/*
 *	Kep reszletet adja vissza
 *
 */
KEP* cut_pic( KEP* original, int x1, int x2, int y1, int y2 );

/*
 *	Szines kepet konvertal szurkearnyalatuva
 *
 */
KEP* rgb2gray( KEP* rgb );

/*
 *	cut_pic belso fuggveny
 *
 */
int int_clipping( int wert, int h1, int h2 );

/*
 *	cut_pic belso fuggveny
 *
 */
void lower_first( int* first, int* second );

#endif
