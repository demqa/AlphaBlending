##
# Alpha Blending
#

CC     = g++

CFLAGS = -c -mavx -mavx2

OBJDIR = Obj
MKDIR  = mkdir -p

all: directories blend

blend: blend.o
	$(CC) Obj/blend.o -lsfml-graphics -lsfml-window -lsfml-system -o blend

blend.o: Src/blending.cpp
	$(CC) $(CFLAGS) Src/blending.cpp -o Obj/blend.o

directories:
	$(MKDIR) $(OBJDIR)

clear:
	rm -rf *.o blend

#end

