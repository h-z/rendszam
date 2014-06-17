#ifndef _OCR_H_
#define _OCR_H_

#include "libhz.h"

/*
 *	Kepek listaja
 *
 */
typedef struct kl {
	KEP* kep;
	char betu;
	struct kl *kov;
	} lista, *listap;
	
/*
 *	Az a kepen megkeresi a karaktereknek
 *	tuno objektumokat, es a listaba helyezi oket
 *
 */
void characters(KEP* a, listap A);

/*
 *	Eltavolitja a felesleges reszeket a keprol
 *
 */
KEP* frameless(KEP* a);

/*
 *	Elarasztja a kepet az adott ponton
 *
 */
int flood(int x, int y, KEP* a);
	
/*
 *	Elarasztja a kepet az adott ponton
 *
 */
int floodinverse(int x, int y, KEP* a);

/*
 *	Elarasztasos modszerrel felmeri az ponto talalhato objektum
 *	meretet. KEP uj segedkep
 *
 */
void floodchar(KEP* a, KEP* uj, int x, int y, int* t, int* b, int* l, int* r);

/*
 *	Kiirja a hattertarolora a listaban szereplo kepeket.
 *
 */
void writechar(listap l);

/*
 *	A listaban szereplo karakterkepeket felismeri.
 *
 */
char* recognizeplate(listap plate, listap chars);

/*
 *	A listaelemben szereplo kepet felismeri.
 *
 */
char recognize(listap a, listap b);

/*
 *	A konzolra kiirja a felismert rendszamot
 *
 */
void writeplate(listap a);

/*
 *	Atmeretezi az adott kepet
 *
 */
KEP* scale(KEP* a, double sx, double sy);

/*
 *	Megkeresi a rendszamtablat az adott kepen
 *
 */
KEP* findplate(KEP* a);

/*
 *	Rendszamfelismero fofuggveny
 *
 */
char* rendszamfelismero(char* kepfile, listap B_lista);

/*lista*/

/*
 *	Uj listaelemet general
 *
 */
listap new();

/*
 *	Uj kepet szur be a listaba
 *
 */
void add(listap l, KEP* a);

/*
 *	Az ismert karaktereket listajat tolti be
 *
 */
listap loadchars();

/*
 *	Torli a memoriabol a listat.
 *
 */
void destroy(listap l);

#endif
