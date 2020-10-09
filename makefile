CC = g++
CFLAGS = -std=c++11 -Wall
INCLDIR = include
SRCDIR = src
OBJDIR = obj


all: objFolder utils.o preprocessing.o twoPass.o montador

preprocessing.o: $(SRCDIR)/preprocessing.cpp $(INCLDIR)/preprocessing.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/preprocessing.cpp -I $(INCLDIR) -o $(OBJDIR)/preprocessing.o

utils.o: $(SRCDIR)/utils.cpp $(INCLDIR)/utils.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/utils.cpp -I $(INCLDIR) -o $(OBJDIR)/utils.o

twoPass.o: $(SRCDIR)/twoPass.cpp $(INCLDIR)/twoPass.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/twoPass.cpp -I $(INCLDIR) -o $(OBJDIR)/twoPass.o

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