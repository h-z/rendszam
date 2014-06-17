#include "jpeg.h"

KEP* new_pic( int xdim, int ydim, int colnum )
{
	KEP* picptr;
	int   i;

	if( xdim < 0  ||  ydim < 0  ||  colnum < 0 )
		return( NULL );

	picptr = calloc( 1, sizeof(KEP) );
	if( picptr == NULL )
	{
		fprintf( stderr, "Error: no memory available.\n");
		exit( EXIT_FAILURE );
	}
	
	//minden elem 0 lesz...
	
	
	/* mutatotomb allokalas */
	/* -------------------- */
	picptr->pic = calloc( ydim, sizeof(JSAMPROW) );
	if( picptr->pic == NULL )
	{
		fprintf( stderr, "Error: no memory available.\n");
		free( picptr );
		exit( EXIT_FAILURE );
	}

	/* a tenyleges kephely allokalasa */
	/* ------------------------------ */
	*picptr->pic = calloc( xdim * ydim * colnum, sizeof(JSAMPLE) );
	if( *picptr->pic == NULL )
	{
		fprintf( stderr, "Error: no memory available.\n");
		free( picptr->pic );
		free( picptr );
		exit( EXIT_FAILURE );
	}

	/* a kezdosorok beallitasa */
	/* ----------------------- */
	for( i = 1; i < ydim; i++ )  /* 0 nem kell, az mar okes*/
	{
		*(picptr->pic + i) = *picptr->pic + i * xdim * colnum;
	}


	/* kepjellemzok */
	/* ------------ */
	picptr->width              = xdim;
	picptr->height             = ydim;
	picptr->samples_per_pixel  = colnum;

	return( picptr );
}

KEP* destroy_pic( KEP* pic )
{

	if( pic == NULL )
		return(NULL);

	if( pic->pic != NULL ) /* kepmutato (haha) */
	{
		if( *pic->pic != NULL ) 
		{
			free( *pic->pic );
			*pic->pic = NULL;
		}

		free( pic->pic );
		pic->pic = NULL;
	}

	if( pic->filename != NULL ) 
	{
		free( pic->filename );
		pic->filename = NULL;
	}

	if( pic->picname != NULL )
	{
		free( pic->picname );
		pic->picname = NULL;  
	}

	free( pic );
	pic = NULL;
 
	return( pic ); // tehat NULL
} 

KEP* load_pic( char* filename )
{
	FILE*       fp;
	KEP*       picptr;
	JSAMPARRAY  pic_buffer;   /* read-buffer */
	struct jpeg_decompress_struct   cinfo;
	struct jpeg_error_mgr           jerr;

	int    row_stride;
	int    line_number;

	fp = fopen( filename, "rb" );
	if( fp == NULL )
	{
		perror( filename );
		return( NULL );
	}

	cinfo.err = jpeg_std_error( &jerr );   /* init errorhandler */
	jpeg_create_decompress( &cinfo );        /* init de-compress-obj. */

	jpeg_stdio_src( &cinfo, fp );   /* invoke standard source.-module */
	jpeg_read_header( &cinfo, TRUE );

	jpeg_start_decompress( &cinfo );

	row_stride = cinfo.output_width * cinfo.output_components;
	pic_buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);


	picptr = new_pic( cinfo.image_width, cinfo.image_height, cinfo.output_components );
	if( picptr == NULL )
	{
		picptr = destroy_pic( picptr );
		jpeg_destroy_decompress( & cinfo );
		fclose( fp );
		exit( EXIT_FAILURE );
		return( NULL );
	}
	picptr->filename          = strdup( filename );


	while( cinfo.output_scanline < cinfo.output_height )
	{
		line_number = cinfo.output_scanline; 

		jpeg_read_scanlines( &cinfo, pic_buffer, 1 ); 

		memcpy( *(picptr->pic + line_number), *pic_buffer, row_stride );

	}

	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( & cinfo );

	fclose( fp );
	return( picptr );
}

int write_pic( KEP* bild, char* filename )
{

	FILE*    outfile;
	JSAMPROW row_pointer[1];
	struct   jpeg_compress_struct   cinfo;
	struct   jpeg_error_mgr         jerr;
	int      row_stride;


	if( bild == NULL )
	{
		return(1);
	}

	if( filename == NULL )           
		filename = bild->filename;

	if( filename == NULL )           
	{
		fprintf( stderr, "Error: can't write a file without a name! => bug.\n");
		exit( EXIT_FAILURE );          /* if there is no name, it's a bug! */
	}


	cinfo.err = jpeg_std_error( &jerr );   /* init errorhandler */
	jpeg_create_compress( &cinfo );        /* init compress-obj. */

	outfile = fopen( filename, "wb" );
	if( outfile == NULL )
	{
		fprintf( stderr, "can't open \"%s\"", filename );
		exit( EXIT_FAILURE );
	}
	jpeg_stdio_dest( &cinfo, outfile );   /* invoke standard dest.-module */

	cinfo.image_width      = bild->width;
	cinfo.image_height     = bild->height;
	cinfo.input_components = bild->samples_per_pixel;

	if( bild->samples_per_pixel == 3 )
		cinfo.in_color_space   = JCS_RGB;
	else
		cinfo.in_color_space   = JCS_GRAYSCALE;

	row_stride = cinfo.image_width * cinfo.input_components; 

	jpeg_set_defaults( &cinfo );

  
	jpeg_set_quality( &cinfo, 100, TRUE );
  

	jpeg_start_compress( &cinfo, TRUE );

	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = *(bild->pic + cinfo.next_scanline);
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}

	jpeg_finish_compress( & cinfo );
	jpeg_destroy_compress( & cinfo );

	fclose( outfile );

	return(0);
}

KEP* cut_pic( KEP* original, int x1, int x2, int y1, int y2 )
{
	KEP*   part;
	int     colnums;
	int     row_stride;
	int     i;
	int     y;
	int     x_len;
	int     img_h;
	int     img_w;

	if( original == NULL )
	{
		return( NULL );
	}

	colnums = original->samples_per_pixel;
  
	x1 = int_clipping( x1, 0, original->width - 1 );
	x2 = int_clipping( x2, 0, original->width - 1 );

	y1 = int_clipping( y1, 0, original->height - 1 );
	y2 = int_clipping( y2, 0, original->height - 1 );


	lower_first( &x1, &x2 );   /* x1 <= x2 */
	lower_first( &y1, &y2 );   /* y1 <= y2 */


	img_w  = (x2 - x1 + 1);
	img_h  = (y2 - y1 + 1);

	x_len = original->width;
	row_stride = original->samples_per_pixel * x_len;

	part = new_pic( img_w, img_h, colnums );
	if( part == NULL )
	{
		fprintf( stderr, "Error: not enough memory! => exiting\n" );
		exit( EXIT_FAILURE );

	}

	//masolas
	for( y = y1, i = 0; y <= y2; y++ )
	{
		memcpy( *(part->pic + i),
				  *(original->pic +  y) + x1 * colnums,
				  img_w * colnums
			  );

		i++; /* destination-offset: write into next line */
	}

	return( part );
}

KEP* rgb2gray( KEP* rgb )
{
	KEP*             sw;        
	int               y;        
	int               row_stride;
	int               i;
	int               j;

	int               r;
	int               g;
	int               b;

	if( rgb->samples_per_pixel != 3 )
		return(0);

	row_stride = rgb->width * rgb->samples_per_pixel;

	sw = new_pic( rgb->width, rgb->height, 1 ); // csak intenzitas
	if( sw == NULL )
		return( NULL );

	for( i = 0; i < rgb->height; i++ ) 
	{
		for( j = 0; j < row_stride; j += 3 )
		{
			r = *(*(rgb->pic + i) + j);
			g = *(*(rgb->pic + i) + j + 1);
			b = *(*(rgb->pic + i) + j + 2);

			y = 0.3 * r + 0.59 * g + 0.11 * b;

			*(*(sw->pic + i) + j/3) = y;
		}
	}

	return( sw );
}

int int_clipping( int v, int h1, int h2 )
{

	int    max;
	int    min;
	int    res;


	max =  h1 > h2  ? h1 : h2;
	min =  h1 < h2  ? h1 : h2;

	res =  v > max  ?  max : v;
	res =  v < min  ?  min : res;  

	return( res );
}

void lower_first( int* first, int* second )
{
	int   save;

	if( *first <= *second )
		return;

	save    = *first;
	*first  = *second;
	*second = save;

	return;
}
