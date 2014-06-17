SHELL	= /bin/sh

CFLAGS	= -Wall -L/opt/lib -g
CC 	= gcc
LIBS 	= -ljpeg
OBJS 	= jpeg.o libhz.o ocr.o main.o

all: rendszam

rendszam: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(CFLAGS)

clean:
	rm -f *.o rendszam

