#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "libhz.h"
#include "jpeg.h"
#include "ocr.h"

#include <signal.h> 
#include <sys/types.h>

#include <time.h>

int usr_interrupt;	

void kezelo(int a)
{
	usr_interrupt=1;	
}

void ido()
{
	printf("Ido: %f\n", (double)clock()/(double)CLOCKS_PER_SEC);
}

int main(int argc, char *argv[])
{
	/*
	double m[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	double r[4] = {1, 0, 0, -1};
	double avg_m[9] = {0.1, 0.1, 0.1, 0.1, 0.2, 0.1, 0.1, 0.1, 0.1};
	double avg_m5[25] = {0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04};
	double gaussian_m[9] ={0.0113, 0.0838, 0.0113, 0.0838, 0.6193, 0.0838, 0.0113, 0.0838, 0.0113};
	double highpass_m[9] =  {-0.111, -0.111, -0.111, -0.111, 0.888, -0.111, -0.111, -0.111, -0.111};
	double unsharp_m[9] = {-0.1667, -0.1667, -0.1667, -0.1667, 4.333, -0.1667, -0.1667, -0.1667, -0.1667};
	*/

	char* s;
	listap B_lista = loadchars();
	usr_interrupt=1; //0 lenne igazabol	

	FILE *fp;
	fp = fopen("/home/hz/rendszam/rendszam.pid", "w");
	fprintf(fp, "%u", getpid());
	fclose(fp);

	sigset_t mask, oldmask;
	
	signal(SIGUSR1, kezelo);
	sigemptyset (&mask);
	sigaddset (&mask, SIGUSR1);
	while(1)
	{
		sigprocmask (SIG_BLOCK, &mask, &oldmask);
		
		while (!usr_interrupt)
			sigsuspend (&oldmask);
		
		sigprocmask (SIG_UNBLOCK, &mask, NULL);

		ido();

		s = rendszamfelismero("/home/hz/rendszam/kep.jpg", B_lista);
		
		printf("\nFelismert rendszamtabla: \n");
		printf("%s\n", s);
		usr_interrupt = 0;
		//break;
	}
	destroy(B_lista);
	return EXIT_SUCCESS;
}
