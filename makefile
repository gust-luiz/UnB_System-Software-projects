CC = g++
CFLAGS = -std=c++11 -Wall
INCLDIR = include
SRCDIR = src
OBJDIR = obj


all: objFolder preprocessing.o macro.o montador

preprocessing.o: $(SRCDIR)/preprocessing.cpp $(INCLDIR)/preprocessing.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/preprocessing.cpp -I $(INCLDIR) -o $(OBJDIR)/preprocessing.o

macro.o: $(SRCDIR)/macro.cpp $(INCLDIR)/macro.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/macro.cpp -I $(INCLDIR) -o $(OBJDIR)/macro.o

montador:
	$(CC) $(CFLAGS) montador.cpp $(OBJDIR)/*.o -I $(INCLDIR) -o montador

run:
	./montador

objFolder:
	mkdir -p $(OBJDIR)

clean: 
	rm $(OBJDIR)/*.o
	rmdir $(OBJDIR)
	rm montador