#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "libhz.h"
#include "jpeg.h"

KEP* imnew( int xdim, int ydim, int colnum )
{
	return new_pic( xdim, ydim, colnum );
}

KEP* imclear( KEP* pic )
{
	return destroy_pic( pic );
}

KEP* imread( char* filename )
{
	return load_pic( filename );
}

int imwrite( KEP* kep, char* filename )
{
	return write_pic( kep, filename );
}

KEP* imcut( KEP* original, int x1, int x2, int y1, int y2 )
{
	return cut_pic( original, x1, x2, y1, y2 );
}


KEP* kereszt(KEP* a, int xk, int yk, int v)
{
	KEP* uj;
	int x,y;
	uj = (KEP*)new_pic(a->width, a->height, 3);
	for(x = 0; x < a->width*3; x+=3)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(((abs(x - xk*3) <= 3*v) && (abs( y - yk) <= 10*v)) || ((abs(y - yk) <= v) && (abs( x - xk*3) <= 30*v)))
			{
				uj->pic[y][x]   = 0xff;
				uj->pic[y][x+1] = 0;
				uj->pic[y][x+2] = 0;
				
			}
			else
			{
				uj->pic[y][x]   = a->pic[y][x];
				uj->pic[y][x+1] = a->pic[y][x+1];
				uj->pic[y][x+2] = a->pic[y][x+2];
			}
		}
	}	
	
	return uj;
}


PONT kozep(KEP* a)
{
	int x,y;
	int xk=0, yk=0;
	int db=0;
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(a->pic[y][x] == OBJEKTUM)
			{
				xk += x;
				yk += y;
				db++;
			}
		}
	}
	xk /= db;
	yk /= db;
	PONT p;
	if(db >= (a->width*a->height*0.00002))
	{
		p.x = xk;
		p.y = yk;
	}
	else
	{
		p.x = -1;
		p.y = -1;
	}
	printf("darab: %d\n", db);
	return p;
}

KEP* iminverse(KEP* a)
{
	KEP* uj;
	uj = (KEP*)new_pic(a->width, a->height, 1);
	int x,y;
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(a->pic[y][x] == OBJEKTUM)
			{
				uj->pic[y][x] = HATTER;
			}
			else
			{
				uj->pic[y][x] = OBJEKTUM;
			}
		}
	}	
	return uj;
}


KEP* imand(KEP* a, KEP* b)
{
	if((a->width != b->width) || (a->height != b->height) || (a->samples_per_pixel != 1) || (b->samples_per_pixel != 1))
	{
		return 0;
	}
	KEP* uj;
	uj = (KEP*)new_pic(a->width, a->height, 1);
	int x,y;
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			if((a->pic[y][x] == OBJEKTUM) && (b->pic[y][x] == OBJEKTUM))
				uj->pic[y][x] = OBJEKTUM;
		}
	}	
	return uj;
}


KEP* imor(KEP* a, KEP* b)
{
	if((a->width != b->width) || (a->height != b->height) || (a->samples_per_pixel != 1) || (b->samples_per_pixel != 1))
	{
		return 0;
	}
	KEP* uj;
	uj = (KEP*)new_pic(a->width, a->height, 1);
	int x,y;
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			if((a->pic[y][x] == OBJEKTUM) || (b->pic[y][x] == OBJEKTUM))
			{
				uj->pic[y][x] = OBJEKTUM;
			}
		}
	}	
	return uj;
}


KEP* colorcut(KEP* a, int r, int g, int b, int tolerance)
{
	int x,y;
	KEP* uj;
	
	uj = (KEP*)new_pic( a->width, a->height, 1);
	for(x = 0; x < a->width*3; x+=3)
	{
		for(y = 0; y < a->height ; y++)
		{
			if( (abs(a->pic[y][x] - r) < tolerance) && (abs(a->pic[y][x+1] - g) < tolerance) && (abs(a->pic[y][x+2] - b) < tolerance) )
			{
				uj->pic[y][x/3] = OBJEKTUM;
			}
			else
			{
				uj->pic[y][x/3] = HATTER;
			}
		}
	}
	return uj;
}
/*
int labelsize(PONT* p)
{
	int a=0;
	PONT* q;
	for(q = p; q->x != -1; q++)
		a++;
	
	return a;
}
*/

PONT* findlabel(KEP* a, int l) //labeles kepbol kiadja az adott label koordinatait.
{
	int x,y;
	int darab=0;
	int d=0;
	PONT* e;
	PONT q;
	//vegignezzuk, hogy hany darab lesz
	for(x = 0; x < a->width; x+=1)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(a->pic[y][x] == l)
			{
				darab++;
			}
		}
	}
	e = malloc(sizeof(PONT[darab+1]));

	for(x = 0; x < a->width; x+=1)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(a->pic[y][x] == l)
			{
				PONT p;
				p.x = x;
				p.y = y;
				e[d++] = p;
			}
		}
	}
	// strazsa a vegere
	q.x = -1;
	q.y = -1;
	e[d] = q;
	
	return e;
}
/*
label incr(label a)
{
	label l;
	int n = 1; // vmi 2 hatvanya kell ide
	if((a.r == 0xff) && (a.g == 0xff) && (a.b == 0xff))
	{
		l.r = 0;
		l.g = 0;
		l.b = 0;
		return l;
	}
	else
	if((a.g == 0xff) && (a.b == 0xff))
	{
		l.r = a.r + n;
		l.g = 0;
		l.b = 0;
		return l;		
	}
	else
	if(a.b == 0xff)
	{
		l.r = a.r;
		l.g = a.g + n;
		l.b = 0;
		return l;		
	}
	else
	{
		l.r = a.r;
		l.g = a.g;
		l.b = a.b + n;
		return l;
	}
}
*/
KEP* bwlabel(KEP* a)
{
	if (a->samples_per_pixel != 1)
	{
		return 0;
	}

	KEP* uj;
	int x,y;
	//label l;
	int cimke=0;
	int no=1;

	uj = (KEP*)new_pic( a->width, a->height, 1);
	//l.r = l.g = l.b = 0; // szin-informaciokent mentjuk el a cimkeket majd, ha...
	
	for(x = 0; x < a->width; x+=1)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(a->pic[y][x] == OBJEKTUM ) // FF keprol van szo
			{	
				/*
				osszenovesztes nem indulhat innen
				*/
				if((x == 0) || (y == 0)) // bonyolult szele
				{
					if((x == 0) && (y == 0)) // elso pixel objektum
					{
						cimke += no;  //lathato novekmeny
						uj->pic[y][x] = cimke;
					}
					if(x == 0) // y mar nem lehet 0...
					{
						if(a->pic[y-1][x] == OBJEKTUM) // mar vizsgaltuk
						{
							uj->pic[y][x] = uj->pic[y-1][x];
						}
						else
						{
							cimke += no;  //lathato novekmeny
							uj->pic[y][x] = cimke;
						}
					}
					if(y == 0)
					{
						if(a->pic[y][x-1] == OBJEKTUM) // mar vizsgaltuk
						{
							uj->pic[y][x] = uj->pic[y][x-1];
						}
						else
						{
							cimke += no;  //lathato novekmeny
							uj->pic[y][x] = cimke;
						}
					}
				}
				else // kep belseje, bonyolult szel nelkul, hiba nem lehet, csak visszajatszasnal
				{
					if((a->pic[y-1][x] == HATTER) &&(a->pic[y][x-1] == HATTER))
					{
						cimke += no;  //lathato novekmeny
						uj->pic[y][x] = cimke;						
					}
					if((a->pic[y-1][x] == OBJEKTUM) &&(a->pic[y][x-1] == HATTER))
					{
						uj->pic[y][x] = uj->pic[y-1][x];
					}
					if((a->pic[y-1][x] == HATTER) &&(a->pic[y][x-1] == OBJEKTUM))
					{
						uj->pic[y][x] = uj->pic[y][x-1];
					}
					if((a->pic[y-1][x] == OBJEKTUM) &&(a->pic[y][x-1] == OBJEKTUM))
					{
						if(uj->pic[y-1][x] == uj->pic[y][x-1]) // ismerosok ha talalkoznak
						{
							uj->pic[y][x] = uj->pic[y][x-1]; // mindegy, melyik, ugyanaz
						}
						else
						{ // osszenovesztes
							uj->pic[y][x] = uj->pic[y][x-1];
							int xb, yb;
							// eddig bejart teglalap
							for(xb = 0; xb < x; xb++)
							{
								for(yb = 0; yb < uj->height; yb++)
								{
									if(uj->pic[yb][xb] == uj->pic[y-1][x])
									{
										uj->pic[yb][xb] = uj->pic[y][x];
									}
								}
							}
							for(yb = 0; yb < y; yb++)
							{
								if(uj->pic[yb][x] == uj->pic[y-1][x])
								{
									uj->pic[yb][x] = uj->pic[y][x];
								}
							}
						}
					}
				}
			}
		}
	}
	return uj;
}

KEP* imdilate(KEP* a)
{
	if (a->samples_per_pixel != 1)
	{
		return 0;
	}
	
	KEP* uj;
	int x,y;
	uj = (KEP*)new_pic( a->width, a->height, 1);
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(a->pic[y][x] == OBJEKTUM )
			{
				uj->pic[y][x] = OBJEKTUM;
				if(x < a->width-1)
				{
					uj->pic[y][x+1] = OBJEKTUM;
				}
				if(x > 0)
				{
					uj->pic[y][x-1] = OBJEKTUM;
				}
				if(y < a->height-1)
				{
					uj->pic[y+1][x] = OBJEKTUM;
				}
				if(y > 0)
				{
					uj->pic[y-1][x] = OBJEKTUM;
				}
			}
		}
	}
	return uj;
}

KEP* imerode(KEP* a)
{
	if (a->samples_per_pixel != 1)
	{
		return 0;
	}
	
	KEP* uj;
	int x,y;
	uj = (KEP*)new_pic( a->width, a->height, 1);
	
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			if(a->pic[y][x] == OBJEKTUM )
			{
				if(( pixel_0( a, x, y, 1, 0) == OBJEKTUM ) && ( pixel_0( a, x, y, -1, 0) == OBJEKTUM ) && ( pixel_0( a, x, y, 0, 1) == OBJEKTUM ) && ( pixel_0( a, x, y, 0, -1) == OBJEKTUM ))
				{
					uj->pic[y][x] = OBJEKTUM;
				}
			}
			else
			{
				uj->pic[y][x] = HATTER;
			}
		}
	}
	return uj;
}

double* multiply(double* a, double* b, int n)
{
	double* e;
	int i,j,k;
	
	e = malloc(sizeof(double[n*n]));
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			e[n*i+j] = 0;
			for(k=0; k<n; k++)
			{
				e[n*i+j] += a[n*i + k] * b[n*k + j];
			}
		}
	}
	return e;
}

KEP* conv2(KEP* a, double* m, int sizex, int sizey)
{
	if(a->samples_per_pixel == 3)
	{
		return conv2_rgb(a, m, sizex, sizey);
	}
	else
	{
		return conv2_gray(a, m, sizex, sizey);
	}
}

KEP* conv2_rgb(KEP* a, double* m, int sizex, int sizey)
{
	KEP* uj;
	int x,y;
	
	uj = (KEP*)new_pic( a->width, a->height, 3);
	for(x = 0; x < a->width*3; x+=3)
	{
		for(y = 0; y < a->height ; y++)
		{
			double sumr=0.0, sumg=0.0, sumb=0.0;
			int i,j, k=0, t=sizex/2;
			for(i=0; i<sizex; i++)
			{
				for(j=0; j<sizey; j++)
				{
					sumr += (double)pixel(a, x, y, (i-t)*3, j-t)   * m[k];
					sumg += (double)pixel(a, x+1, y, (i-t)*3, j-t) * m[k];
					sumb += (double)pixel(a, x+2, y, (i-t)*3, j-t) * m[k++];
				}
			}
			uj->pic[y][x]   = (JSAMPLE)sumr;
			uj->pic[y][x+1] = (JSAMPLE)sumg;
			uj->pic[y][x+2] = (JSAMPLE)sumb;
			k=0;
		}
	}	
	return uj;
}

KEP* conv2_gray(KEP* a, double* m, int sizex, int sizey)
{
	KEP* uj;
	int x,y;
	
	uj = (KEP*)new_pic( a->width, a->height, 1);
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			double sum=0.0;
			int i,j, k=0;
			for(i=0; i<sizex; i++)
			{
				for(j=0; j<sizey; j++)
				{
					sum += pixel(a, x, y, i, j) * m[k++];
				}
			}
			uj->pic[y][x] = (JSAMPLE)sum;
			k=0;
		}
	}	
	return uj;
}

KEP* merge_rgb(KEP* r, KEP* g, KEP* b)
{
	KEP* rgb;
	if((r->width == g->width) && (r->width == b->width) && (r->height == g->height) && (r->height == b->height) && ( r->samples_per_pixel * g->samples_per_pixel * b->samples_per_pixel == 1)) //lehet
	{
		int x,y;
		
		rgb = (KEP*)new_pic( r->width, r->height, 3);
		for(x = 0; x < rgb->width*rgb->samples_per_pixel; x+=rgb->samples_per_pixel)
		{
			for(y = 0; y < rgb->height; y++)
			{
				rgb->pic[y][x]   = r->pic[y][x/3];
				rgb->pic[y][x+1] = g->pic[y][x/3];
				rgb->pic[y][x+2] = b->pic[y][x/3];
			}
		}
		return rgb;
	}
	else
	{
		return 0;
	}
}

JSAMPLE window(JSAMPLE pixels[3][3], double m[3][3])
{
	int i, j;
	double s=0;
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			s += pixels[i][j] * m[i][j];
		}
	}
	
	return (JSAMPLE)s;
}

int* histogram(KEP *a)
{
	KEP* bw;
	int i;
	int x, y;

	if( a->samples_per_pixel == 3 ) // RGB
	{
		bw = (KEP*)rgb2gray( a );
	}
	else							// alapbol szurke
	{
		bw = a;
	}
	int *h = malloc(sizeof(int[256]));

	if (h == NULL)
	{
		return NULL;
	}

	for(i = 0; i < 256; i++)
	{
		h[i] = 0;
	}

	for(x = 0; x < bw->width*bw->samples_per_pixel; x+=bw->samples_per_pixel)
	{
		for(y = 0; y < bw->height; y++)
		{
			h[(int)bw->pic[y][x]]++;
		}
	}
	
	return h;
}

KEP* colorimage(KEP* a, int color)
{
	KEP *uj;
	
	if(a->samples_per_pixel == 1) // grayscale
	{
		return 0;
	}

	uj = (KEP*)new_pic( a->width, a->height, 1 );
	int x,y;
	for(x = 0; x < a->width*a->samples_per_pixel; x+=a->samples_per_pixel)
	{
		for(y = 0; y < a->height-1; y++)
		{
			uj->pic[y][x/(a->samples_per_pixel)] = a->pic[y][x+color];
		}
	}

	return uj;
}

JSAMPLE pixel(KEP* a, int x, int y, int i, int j) //szin-info az x-ben !!!
{
	if( (x+i >= a->width*a->samples_per_pixel) || (y+j >= a->height) || (x+i < 0) || (y+j < 0) )
	{
		return a->pic[y][x];
	}
	else
	{
		return a->pic[y+j][x+i];
	}
}

JSAMPLE pixel_0(KEP* a, int x, int y, int i, int j)
{
	if( (x+i >= a->width*a->samples_per_pixel) || (y+j >= a->height) || (x+i < 0) || (y+j < 0) )
	{
		return HATTER;
	}
	else
	{
		return a->pic[y+j][x+i];
	}
}

KEP* histeq(KEP* a)
{
	KEP* uj, *bw;
	if( a->samples_per_pixel == 3 ) // RGB
	{
		bw = (KEP*)rgb2gray( a );
	}
	else							// alapbol szurke
	{
		bw = a;
	}
	
	int *h;
	int histsum[256];
	int i;
	int x, y;
	
	for(i = 0; i < 256; i++)
	{
		histsum[i] = 0;
	}

	h = histogram(bw);
	histsum[0] = h[0];
	for(i = 1; i<256;i++)
	{
		histsum[i] = histsum[i-1] + h[i];
	}
	
	int pixels = bw->height*bw->width;
	double alpha = 255.0 / pixels;

	uj = (KEP*)new_pic( bw->width, bw->height, bw->samples_per_pixel );

	for(x = 0; x < bw->width*bw->samples_per_pixel; x+=bw->samples_per_pixel)
	{
		for(y = 0; y < bw->height; y++)
		{
			uj->pic[y][x] =  histsum[(bw->pic[y][x])] * alpha;
		}
	}
	return uj;
}

JSAMPLE median(JSAMPLE m[], int n)
{
	int         i, less, greater, equal;
	JSAMPLE  min, max, guess, maxltguess, mingtguess;

	min = max = m[0] ;
	for (i=1 ; i<n ; i++) 
	{
		if (m[i]<min) min=m[i];
		if (m[i]>max) max=m[i];
	}

	while (1) 
	{
		guess = (min + max)/2;
		less = 0; greater = 0; equal = 0;
		maxltguess = min ;
		mingtguess = max ;
		for (i=0; i<n; i++) 
		{
			if (m[i]<guess) 
			{
				less++;
				if (m[i]>maxltguess)
				{
					maxltguess = m[i];
				}
			} 
			else 
			{
				if (m[i]>guess) 
				{
					greater++;
					if (m[i]<mingtguess)
					{
						mingtguess = m[i] ;
					}
				} 
				else 
				{
					equal++;
				}
			}
		}
		if (less <= (n+1)/2 && greater <= (n+1)/2) 
		{
			break ; 
		}
		else 
		{
			if (less>greater) 
			{
				max = maxltguess ;
			}
			else 
			{
				min = mingtguess;
			}
		}
	}
	if (less >= (n+1)/2) 
	{
		return maxltguess;
	}
	else 
	{
		if (less+equal >= (n+1)/2) 
		{
			return guess;
		}
		else 
		{
			return mingtguess;
		}
	}
}

KEP* medfilt2(KEP* a, int window) // window*window ablakban figyel erosen
{
	KEP* uj;
	if(a->samples_per_pixel == 3)
	{
		uj = medfilt2_rgb( a, window);
	}
	else
	{
		uj = medfilt2_gray( a, window);
	}

	return uj;
}

KEP* medfilt2_gray(KEP* a, int window)
{
	KEP* uj;
	int x, y, i, j, k=0;
	int t = window/2;
	uj = (KEP*)new_pic( a->width, a->height, 1);
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			JSAMPLE p;
			JSAMPLE tomb[window*window];
			for(i=0; i<window; i++)
			{
				for(j=0;j<window; j++)
				{
					tomb[k++] = pixel(a, x, y, i-t, j-t);
				}
			}
			k=0;
			p = median( tomb, window*window);
			uj->pic[y][x] = p;			
		}
	}
	return uj;
}

KEP* medfilt2_rgb(KEP* a, int window)
{
	KEP* uj;
	int x, y;
	
	uj = (KEP*)new_pic( a->width, a->height, a->samples_per_pixel);
	for(x = 0; x < a->width*3; x+=3)
	{
		for(y = 0/*1*/; y < a->height /*-1*/; y++)
		{
			JSAMPLE r, g, b;
			int i, j, k=0, t=window/2;
			JSAMPLE tombr[window*window];
			JSAMPLE tombg[window*window];
			JSAMPLE tombb[window*window];
			
			for(i=0; i<window; i++)
			{
				for(j=0;j<window; j++)
				{
					tombr[k]   = pixel(a, x,   y, (i-t)*3, j-t);
					tombg[k]   = pixel(a, x+1, y, (i-t)*3, j-t);
					tombb[k++] = pixel(a, x+2, y, (i-t)*3, j-t);
				}
			}
			k=0;
			r = median( tombr, window*window);
			g = median( tombg, window*window);
			b = median( tombb, window*window);
			uj->pic[y][x]   = r;
			uj->pic[y][x+1] = g;
			uj->pic[y][x+2] = b;
		}
	}
	return uj;
}

KEP* medfilt(KEP* a, int window) //csak gray, bw
{
	KEP* uj;
	int x, y, i, k=0;
	int t = window/2;
	uj = (KEP*)new_pic( a->width, a->height, 1);
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			JSAMPLE p;
			JSAMPLE tomb[window];
			for(i=0; i<window; i++)
			{
				tomb[k++] = pixel(a, x, y, i-t, 0);
			}
			k=0;
			p = median( tomb, window );
			uj->pic[y][x] = p;			
		}
	}
	return uj;
}

KEP* im2bw(KEP* a, int int_level)
{
	KEP* uj;
	uj = (KEP*)new_pic( a->width, a->height, 1);
	int x, y;
	if(a->samples_per_pixel == 3) //csak 3 vagy 1 lehet
	{
		for(x = 0; x < a->width*a->samples_per_pixel; x+=a->samples_per_pixel)
		{
			for(y = 0; y < a->height; y++)
			{
				uj->pic[y][x/3] = (JSAMPLE)(0.3 * a->pic[y][x] + 0.59 * a->pic[y][x+1] + 0.11 * a->pic[y][x+2]) > int_level ? OBJEKTUM : HATTER;
			}
		}
	}
	else
	{
		for(x = 0; x < a->width*a->samples_per_pixel; x+=a->samples_per_pixel)
		{
			for(y = 0; y < a->height; y++)
			{
				uj->pic[y][x] = ( a->pic[y][x] > int_level ) ? OBJEKTUM : HATTER;
			}
		}
	}
	return uj;
}

KEP* im2bwa(KEP* a, int window)
{
	KEP* uj;
	int x, y, i, j, k=0;
	int sum = 0;
	int t = window/2;
	uj = (KEP*)new_pic( a->width, a->height, 1);
	for(x = 0; x < a->width; x++)
	{
		for(y = 0; y < a->height ; y++)
		{
			JSAMPLE p;
			JSAMPLE tomb[window*window];
			for(i=0; i<window; i++)
			{
				for(j=0;j<window; j++)
				{
					tomb[k++] = pixel(a, x, y, i-t, j-t);
					sum += pixel(a, x, y, i-t, j-t);
				}
			}
			k=0;
			//median: 
			p = median( tomb, window*window);
			//mean:
			//p = (JSAMPLE)(sum/(window*window));
			
			if(a->pic[y][x] > p)
				uj->pic[y][x] = OBJEKTUM;
			else
				uj->pic[y][x] = HATTER;
		}
	}
	return uj;
}

KEP* im2bwhyst(KEP* a, int tl, int th, int window)
{
	KEP* uj;
	uj = (KEP*)new_pic( a->width, a->height, 1);
	int x, y, i;
	for(x = 0; x < a->width*a->samples_per_pixel; x+=a->samples_per_pixel)
	{
		for(y = 0; y < a->height; y++)
		{
			if(a->pic[y][x] > th)
			{
				uj->pic[y][x] = OBJEKTUM;
			}
			else if(a->pic[y][x] < tl)
			{
				uj->pic[y][x] = HATTER;
			}
			else
			{
				for(i = x; i > x - window/2; i--) //csak x iranyban keresunk
				{
					if(a->pic[y][i] > th) // tehat sajat maga miatt obj
					{
						uj->pic[y][x] = OBJEKTUM;
						break;
					}
					if(a->pic[y][i] < tl) // tehat sajat maga miatt hatter
					{
						uj->pic[y][x] = HATTER;
						break;
					}
				}
				for(i = x; i < x + window/2; i++) //csak x iranyban keresunk
				{
					if(a->pic[y][i] > th) // tehat sajat maga miatt obj
					{
						uj->pic[y][x] = OBJEKTUM;
						break;
					}
					if(a->pic[y][i] < tl) // tehat sajat maga miatt hatter
					{
						uj->pic[y][x] = HATTER;
						break;
					}
				}
			}
			//uj->pic[y][x] = ( a->pic[y][x] > int_level ) ? OBJEKTUM : HATTER;
		}
	}
	return uj;
}

KEP* subtract(KEP *a_kep, KEP *b_kep, int tolerance)
{
	if( (a_kep->width !=b_kep->width) || (a_kep->height != b_kep->height))
	{
		return 0;	
	}
	
	// meretek jonak tunnek
	
	if(a_kep->samples_per_pixel == 3 && b_kep->samples_per_pixel == 3 )
	{
		subtract_rgb(a_kep, b_kep, tolerance);
	}
	else
	if(a_kep->samples_per_pixel == 1 && b_kep->samples_per_pixel == 1 )
	{
		subtract_gray(a_kep, b_kep, tolerance);
	}
	// nem egyeznek a szinek
	return 0;	
}

KEP* subtract_gray(KEP *a_kep, KEP *b_kep, int tolerance) // csak RGB
{
	// nem egyeznek az adatok
	if( (a_kep->width !=b_kep->width) || (a_kep->height != b_kep->height) || (a_kep->samples_per_pixel != b_kep->samples_per_pixel) ) 
	{
		return 0;
	}
	KEP* uj;
	int x,y;
	/*
	uj kep a regiek meretevel
	*/
	uj = (KEP*)new_pic( a_kep->width, a_kep->height, 1);
	for(x = 0; x < a_kep->width*a_kep->samples_per_pixel; x++)
	{
		for(y = 0; y < a_kep->height; y++)
		{
			if(abs(a_kep->pic[y][x]   - b_kep->pic[y][x]) < tolerance )
			{
				uj->pic[y][x] = 0;
			}
			else
			{
				uj->pic[y][x]   = 0xff;
			}
		}
	}
	return uj;
}

KEP* subtract_rgb(KEP *a_kep, KEP *b_kep, int tolerance) // csak RGB
{
	// nem egyeznek az adatok
	if( (a_kep->width !=b_kep->width) || (a_kep->height != b_kep->height) || (a_kep->samples_per_pixel != b_kep->samples_per_pixel) ) 
	{
		return 0;
	}
	KEP* uj;
	int x,y;
	/*
	uj kep a regiek meretevel
	*/
	uj = (KEP*)new_pic( a_kep->width, a_kep->height, a_kep->samples_per_pixel);
	for(x = 0; x < a_kep->width*a_kep->samples_per_pixel; x+=a_kep->samples_per_pixel)
	{
		for(y = 0; y < a_kep->height; y++)
		{
			JSAMPLE r, g, b;
			
			r = abs(a_kep->pic[y][x]   - b_kep->pic[y][x]);
			g = abs(a_kep->pic[y][x+1] - b_kep->pic[y][x+1]);
			b = abs(a_kep->pic[y][x+2] - b_kep->pic[y][x+2]);
			
			//if feltetel esetleg lehet jobb, de ez mar intenzitas
			if((0.3 * r + 0.59 * g + 0.11 * b) < tolerance )
			{
				uj->pic[y][x] = 0;
			}
			else
			{
				uj->pic[y][x]   = 0xff;
				uj->pic[y][x+1] = 0xff;
				uj->pic[y][x+2] = 0xff;
			}
		}
	}
	return uj;
}
