# Makefile for 159.735 Assignment 3
#
# To build just type:
#
# make heatSeq

CPP = g++

# Flags for Mac OSX
#CFLAGS = -w -O3 -arch ppc -arch i386

# FLAGS for Linux
CFLAGS = -w -O3

# Locally compiled modules
OBJS = fitsfile.o

# Link to CFITSIO libraries - modify these paths accordingly
LIBP = -L/home/s16242950/cfitsio/
INCP = -I/home/s16242950/cfitsio/

LIBS = -lcfitsio -lm

MODS = $(INCP) $(LIBP) $(LIBS) $(OBJS) 

BINS = heatSeq

all : $(BINS)

clean :
	rm -f $(BINS)
	rm -f *.o

# Demo program. Add more programs by making entries similar to this
heatSeq : heatSeq.cpp draw.hxx array.hxx $(OBJS)
	${CPP} $(CFLAGS) -o heatSeq heatSeq.cpp $(MODS)

# Modules compiled and linked separately
fitsfile.o : fitsfile.cpp fitsfile.h
	${CPP} $(CFLAGS) $(INCP) -c fitsfile.cpp

