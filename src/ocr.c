#include "ocr.h"
#include "libhz.h"
#include "jpeg.h"

int flood(int x, int y, KEP* a)
{
	if(a->pic[y][x] == OBJEKTUM)
	{
		a->pic[y][x] = HATTER;
		if(x != 0)
		{
			flood(x-1, y, a);
		}
		if(y != 0)
		{
			flood(x, y-1, a);
		}
		if(x != a->width-1)
		{
			flood(x+1, y, a);
		}
		if(y != a->height-1)
		{
			flood(x, y+1, a);
		}
	}
	return 0;
}

int floodinverse(int x, int y, KEP* a)
{
	if(a->pic[y][x] == HATTER)
	{
		a->pic[y][x] = OBJEKTUM;
		if(x != 0)
		{
			floodinverse(x-1, y, a);
		}
		if(y != 0)
		{
			floodinverse(x, y-1, a);
		}
		if(x != a->width-1)
		{
			floodinverse(x+1, y, a);
		}
		if(y != a->height-1)
		{
			floodinverse(x, y+1, a);
		}
	}
	return 0;
}

KEP* frameless(KEP* a)
{
	int x,y;
	KEP* uj;
	int top = 0;
	int bottom = a->height;
	int left = 0;
	int right = a->width;

	// keret (kulso feher eltavolitasa)
	
	for(x = 1; x < a->width-1; x++)
	{
		flood(x, 1, a);
		flood(x, a->height-1, a);
	}

	
	for(y = 1; y < a->height-1; y++)
	{
		flood(1, y, a);
		flood(a->width-1, y, a);
	}

	// kulso fekete eltavolitasa
	for(y = 0; y < a->height; y++)
	{
		for(x = 0; x < a->width; x++)
		{
			if(a->pic[y][x] == OBJEKTUM) // ez az elso
			{
				if(!top)
				{					
					top = y;
				}
				bottom = y;//mindegyikre
			}
			//break;
		}
	}
	
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height; y++)
		{
			if(a->pic[y][x] == OBJEKTUM) // ez az elso
			{
				if(!left)
				{					
					left = x;
				}
				right = x; //mindegyikre
			}
			//break;
		}
	}
	
	write_pic(a, "/home/hz/kep06-cut.jpg");
	uj = cut_pic(a, left, right, top, bottom );
	return uj;
}


void characters(KEP* a, listap A)
{
	int x,y;
	
	int top, bottom, left, right; // aktualis karakter teglalapja
	
	listap M = A;
	top = a->height;
	bottom = 0;
	right = 0;
	left = a->width;	
	
	//printf("meret: szel: %d mag: %d terulet: %d\n", a->width, a->height, a->width*a->height);
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height; y++)
		{
			if(a->pic[y][x] == OBJEKTUM)
			{
				KEP* uj;
				uj = (KEP*)new_pic(a->width, a->height, 1);
				floodchar(a, uj, x, y, &top, &bottom, &left, &right);
				destroy_pic(uj);
				uj = cut_pic(a, left, right, top, bottom); //cutpic mar letrehozza az uj ujat
				if(((a->height*a->width/90) <= (uj->height*uj->width)) || (a->height/3 < uj->height)) 
				{
					if(1)
					{
						//printf("OK\n");
						// eleg nagy, ahhoz hogy betu lehessen
						//printf("character boundaries: top: %d, bottom: %d, left: %d, right: %d\n", top, bottom, left, right);
						//printf("meret: szel: %d mag: %d terulet: %d\n", uj->width, uj->height, uj->width*uj->height);
						KEP* kar;
						kar = scale(uj, 200.0, 200.0 );	
						add(M, kar);
					}
				}
				destroy_pic(uj);
				flood(x, y, a);				
				top = a->height;
				bottom = 0;
				right = 0;
				left = a->width;		
			}
		}
	}
}

listap new()
{
	listap l;
	l = (listap)malloc(sizeof(listap));
	l->kov = NULL;  //valgrind?
	
	l->kep = NULL;  //valgrind?
	l->betu = 64;
	
	return l;
}

void add(listap l, KEP* a)
{
	listap q,p;
	
	for(p = l; p->kov != NULL; p = p->kov)
		;
	
	if(p != NULL)
	{
		q = new();
		if(q != NULL)
		{
			q->kov = p->kov;
			p->kov = q;
			q->kep = a;
		}
	}
}	

void floodchar(KEP* a, KEP* uj, int x, int y, int* t, int* b, int* l, int* r)
{
	if((a->pic[y][x] == OBJEKTUM) && (uj->pic[y][x] != OBJEKTUM))
	{
		uj->pic[y][x] = OBJEKTUM;
		
		if(*t > y)
			*t = y;
		if(*b < y)
			*b = y;
		if(*l > x)
			*l = x;
		if(*r < x)
			*r = x;
		
		if(x != 0)
		{
			floodchar(a, uj, x-1, y, t, b, l, r);
		}
		if(y != 0)
		{
			floodchar(a, uj, x, y-1, t, b, l, r);
		}
		if(x != a->width-1)
		{
			floodchar(a, uj, x+1, y, t, b, l, r);
		}
		if(y != a->height-1)
		{
			floodchar(a, uj, x, y+1, t, b, l, r);
		}
	}
}

void writechar(listap l)
{
	int k = 0;
	listap p;
	for(p = l; p/*->kov*/ != NULL; p = p->kov)
	{
		char s[14];
		if(k < 10)
		{
			sprintf(s, "/home/hz/kepkar-0%d.jpg", k);
		}
		else
		{
			sprintf(s, "/home/hz/kepkar-%d.jpg", k);
		}
		k++;
		if(p->kep != NULL)
		{
			write_pic(p->kep, s);	
		}	
	}
}

void clear(KEP* a)
{
	int x,y;
	
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height; y++)
		{
			if(a->pic[y][x] > 128)
			{
				a->pic[y][x] = OBJEKTUM;
			}
			else
			{
				a->pic[y][x] = HATTER;
			}
		}
	}
}

listap loadchars()
{
	//eddigi karakterek
	//a: filenev
	//b: ertek
	
	char* a[42] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "c", "e", "o", "a", "h", "v", "i"};
	char  b[42] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '@', '@', '@', '@', 'h', '|', '@'};
	int i;
	
	listap L = new();
	listap q = L;		
		
	for(i = 0; i < 42; i++)
	{
		char s[20];
		sprintf(s, "/home/hz/betuk/%s.jpg", a[i]);
		KEP* k = load_pic(s);
		clear(k);
		listap B = new();
		B->kep = k;
		B->betu = b[i];
		q->kov = B;
		q = B;
	}
	return L;
}

char* recognizeplate(listap plate, listap chars)
{
	listap q;
	
	for(q = plate; q != NULL; q = q->kov)
	{
		char c;
		if(q->kep != NULL)
		{
			c = recognize(q, chars);
		}
	}
	
	return NULL;
}


char recognize(listap a, listap b)
{
	int x,y;
	int match, max;
	char tipp;
	listap p;
	
	max = 0;
	match = 0;
	tipp = 64;
	int karter = 0;
	
	for(p = b; p !=NULL; p = p->kov)
	{
		if(p->kep != NULL)
		{	
			for(x = 0; x < a->kep->width; x++)
			{
				for(y = 0; y < a->kep->height; y++)
				{
					/*
					if(abs(a->kep->pic[y][x] - p->kep->pic[y][x]) < 128 )
					{
						match++;
					}
					*/
					if(p->kep->pic[y][x] > 127)
					{
						karter++;
						if(a->kep->pic[y][x] > 127)
						{
							match++;
						}
					}
				}
			}
			if((match > max) && (match > karter*0.75))
			{
				max = match;
				tipp = p->betu;
			}
			match = 0;
			karter = 0;
		}
	}
	a->betu = tipp;
	return tipp;
}


void writeplate(listap a)
{
	if(a->kov != NULL)
	{
		writeplate(a->kov);
	}
	if(a->betu != 64)
	{
		printf("%c", a->betu);
	}
}

KEP* scale(KEP* a, double newx, double newy) //double, de intet erdemes beirni, csak grayscalet fogad
{
	KEP* uj;
	int x,y;
	uj = new_pic((int)newx, (int)newy, 1);
	
	for(x = 0; x < uj->width; x++)
	{
		for(y = 0; y < uj->height; y++)
		{
			uj->pic[y][x] = a->pic[ (int)(y*(a->height/newy)) ][ (int)(x*(a->width/newx)) ];
		}
	}
	return uj;
}


KEP* findplate(KEP* a)
{
	int x, y, max = 0, l;
	int h[a->height];
	int la[a->height];

	for(y = 0; y < a->height; y++)
	{
		h[y] = 0;
		la[y] = 0;
		for(x = 0; x < a->width; x++)
		{
			if(x != (a->width -1))
			{
				if(a->pic[y][x] != a->pic[y][x+1])
				{
					h[y]++;
				}
			}				
		}

		/*
		//mini hisztogram a szelere
		
				
		for(x = 0; x < h[y]; x++)
		{
			a->pic[y][x] = HATTER;
		}
		a->pic[y][x+1] = OBJEKTUM;
		a->pic[y][x+2] = OBJEKTUM;
		a->pic[y][x+3] = OBJEKTUM;
		a->pic[y][x+4] = OBJEKTUM;
		write_pic(a, "/home/hz/kep05-minihist.jpg");
		max = (max > h[y]) ? max : h[y];
		*/
	}
	//maxh*.2 felett
	l = 0;
	
	for(y = 1; y < a->height; y++) //0 kellene, de igy konnyebb
	{
		if(h[y] >= (max * 0.2))
		{
			if(la[y-1] != 0)
			{
				la[y] = la[y-1];
			}
			else
			{
				la[y] = ++l;				
			}
		}			
	}
	
	l++;
	
	int lacnt[l];
	
	for(y = 0; y < l; y++)
	{
		lacnt[y] = 0;
	}
	
	for(y = 0; y < a->height; y++)
	{
		if(la[y] != 0)
		{
			lacnt[la[y]]++;
		}
	}
	
	int ml = 0;
	int mli = 0;
	
	for(y = 0; y < l; y++)
	{
		if(lacnt[y] > ml)
		{
			ml = lacnt[y];
			mli = y;
		}

	}
	
	for(y = 0; y < a->height; y++)
	{
		if(la[y] == mli)
		{
			break;
		}
	}
	
	KEP* uj;
	int x1, x2, y1, y2;
	
	x1 = 0;
	x2 = a->width;
	y1 = (int)(y - 0.2 * ml);
	y1 = y1 < 0 ? 0 : y1;
	y2 = (int)(y + 1.2 * ml);
	
	y2 = y2 > a->height ? a->height : y2;
	
	uj = cut_pic(a, x1, x2, y1, y2 );

	return uj;
}

void destroy(listap l)
{
	if(l->kov != NULL)
	{
		destroy(l->kov);
	}
	
	destroy_pic(l->kep);
	free(l);
	
}


char* rendszamfelismero(char* kepfile, listap B_lista) //v: kozepso 1/3, f: 1/6
{
	KEP *kep, *kep2;

	kep = (KEP*)load_pic( kepfile );
	kep2 = (KEP*)rgb2gray(kep);
	destroy_pic(kep);
	write_pic(kep2, "/home/hz/kep01-gray.jpg");
	
	kep = (KEP*)cut_pic(kep2, (int)(kep2->width/3), (int)(2*kep2->width/3), (int)(kep2->height/2), (int)(4*kep2->height/6)); //(int)(kep2->height/2), (int)(4*kep2->height/6)
	//kep = kep2;
	destroy_pic(kep2);
	write_pic(kep, "/home/hz/kep02-cut.jpg");
	
	kep2 = (KEP*)medfilt(kep, 12);
	destroy_pic(kep);
	write_pic(kep2, "/home/hz/kep03-medfilt.jpg");
	kep =  kep2; //(KEP*)histeq(kep2);
	//destroy_pic(kep2);
	//write_pic(kep, "/home/hz/ujkep03-mhisteq.jpg");
	kep2 = (KEP*)im2bw(kep, 128);
	destroy_pic(kep);
	write_pic(kep2, "/home/hz/kep04-bw.jpg");

	kep = imerode(kep2);
	destroy_pic(kep2);
	kep2 = imdilate(kep);
	destroy_pic(kep);
	
	
	kep = imdilate(kep2);
	destroy_pic(kep2);
	kep2 = imerode(kep);
	destroy_pic(kep);

	
	write_pic(kep2, "/home/hz/kep05-erdi.jpg");
	
	kep = findplate(kep2);
	
	destroy_pic(kep2);
	kep2 = iminverse(kep);
	destroy_pic(kep);
	kep = frameless(kep2);
	destroy_pic(kep2);

	write_pic(kep, "/home/hz/kep06-plate.jpg");

	//listap B_lista = loadchars(); 	//Az ismert karakterek listaja
	listap A_lista = new(); 		//A felismert karakterek listaja
	 
	characters(kep, A_lista);
	recognizeplate(A_lista, B_lista);
	
	listap w;
	int z = 0;
	char* s = malloc(sizeof(char[20]));

	for(w = A_lista; w!=NULL; w = w->kov)
	{
		if(w->betu != 64)
		{
			s[z] = w->betu;
			z++;
		}
	}
	s[z]='\0';

	destroy(A_lista);
	destroy_pic(kep);
	return s;
}
