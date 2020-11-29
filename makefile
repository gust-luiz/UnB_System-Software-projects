CC = g++
CFLAGS = -std=c++11 -Wall
INCLDIR = include
SRCDIR = src
OBJDIR = obj


all: objFolder utils.o preprocessing.o twoPass.o montador tradutor

preprocessing.o: $(SRCDIR)/preprocessing.cpp $(INCLDIR)/preprocessing.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/preprocessing.cpp -I $(INCLDIR) -o $(OBJDIR)/preprocessing.o

utils.o: $(SRCDIR)/utils.cpp $(INCLDIR)/utils.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/utils.cpp -I $(INCLDIR) -o $(OBJDIR)/utils.o

twoPass.o: $(SRCDIR)/twoPass.cpp $(INCLDIR)/twoPass.hpp
	$(CC) $(CFLAGS) -c $(SRCDIR)/twoPass.cpp -I $(INCLDIR) -o $(OBJDIR)/twoPass.o

montador:
	$(CC) $(CFLAGS) montador.cpp $(OBJDIR)/*.o -I $(INCLDIR) -o montador

tradutor:
	$(CC) $(CFLAGS) tradutor.cpp $(OBJDIR)/*.o -I $(INCLDIR) -o tradutor

run:
	./montador
	./tradutor

objFolder:
	mkdir -p $(OBJDIR)

clean: 
	rm -f montador
	rm -f tradutor
	rm -f *.pre
	rm -f *.obj
	rm -f *.txt
	rm -rf $(OBJDIR)