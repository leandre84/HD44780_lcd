##
## ------------------------------------------------------------- variables --
##
CC=gcc
CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra -Wstrict-prototypes -fno-common -g -O3

INCLUDES=
LINKPATHS=
LINKLIBS=-lwiringPi -lwiringPiDev

##
## ----------------------------------------------------------------- rules --
##
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<
##
## --------------------------------------------------------------- targets --
##
all:	lcd

lcd:	lcd.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LINKPATHS) $(LINKLIBS)

clean:
	rm -f lcd lcd.o
