SYSTEM     =  x86_sles10_4.1
LIBFORMAT  = static_pic

CPLEXDIR      = /opt/local/CPLEX_Studio/cplex
CONCERTDIR    = /opt/local/CPLEX_Studio/concert


CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CPLEXFLAGS = -std=c++11 -I$(CPLEXDIR)/include   -I$(CONCERTDIR)/include -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -m64 -lpthread
CC = g++ -O0
CFLAGS = -std=c++11 -O2 -Wall -ansi -pedantic -DIL_STD
DEBUG = -pg -g -Wall -pedantic -DIL_STD
OBJECTS = Parser.o Determinist.o SubProb.o MasterProb.o main.o
exec : $(OBJECTS)
	$(CC) $(CFLAGS) -o exe $(OBJECTS) $(CPLEXFLAGS)
main.o : main.cpp
	$(CC) $(CFLAGS) $(CPLEXFLAGS) -c main.cpp
Parser.o : Parser.cpp Parser.h
	$(CC) $(CFLAGS) $(CPLEXFLAGS) -c Parser.cpp
Determinist.o : Determinist.h Determinist.cpp
	$(CC) $(CFLAGS) $(CPLEXFLAGS) -c Determinist.cpp

SubProb.o : SubProb.h SubProb.cpp
        $(CC) $(CFLAGS) $(CPLEXFLAGS) -c SubProb.cpp


MasterProb.o : MasterProb.h MasterProb.cpp
        $(CC) $(CFLAGS) $(CPLEXFLAGS) -c MasterProb.cpp



clean :
	rm -f $(OBJECTS) exe
