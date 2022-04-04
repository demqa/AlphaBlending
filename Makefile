##
# Alpha Blending
#

CC     = g++

CFLAGS = -c -O3 -mavx -mavx2
DEFINE = -D __SSE_OPTIMISATION__

OBJDIR = Obj
MKDIR  = mkdir -p

all: directories blend

sse: directories blend_sse

blend_sse: blend_sse.o
	$(CC) Obj/blend.o -lsfml-graphics -lsfml-window -lsfml-system -o blend

blend_sse.o:
	$(CC) $(CFLAGS) $(DEFINE) Src/blending.cpp -o Obj/blend.o


blend: blend.o
	$(CC) Obj/blend.o -lsfml-graphics -lsfml-window -lsfml-system -o blend

blend.o: Src/blending.cpp
	$(CC) $(CFLAGS) Src/blending.cpp -o Obj/blend.o


directories:
	$(MKDIR) $(OBJDIR)

clear:
	rm -rf *.o blend

#end

