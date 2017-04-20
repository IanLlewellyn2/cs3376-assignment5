PROG = TwoPipesTwoChildren TwoPipesThreeChildren dynpipe
OBJS = TwoPipesTwoChildren.o TwoPipesThreeChildren.o dynpipe.o
CC = g++
CPPFLAGS = -c -g  #compile flags
#LDFLAGS =

dynpipe: dynpipe.o
	$(CC) -o dynpipe DynPipe.cpp
TwoPipesTwoChildren: TwoPipesTwoChildren.o
	$(CC) -o TwoPipesTwoChildren TwoPipesTwoChildren.o
TwoPipesThreeChildren: TwoPipesThreeChildren.o
	$(CC) -o TwoPipesThreeChildren TwoPipesThreeChildren.o
TwoPipesTwoChildren.o: TwoPipesTwoChildren.cpp 
	$(CC) $(CPPFLAGS) TwoPipesTwoChildren.cpp
TwoPipesThreeChildren.o: TwoPipesThreeChildren.cpp
	$(CC) $(CPPFLAGS) TwoPipesThreeChildren.cpp
dynpipe.o: DynPipe.cpp
	$(CC) $(CPPFLAGS) DynPipe.cpp
clean:
	rm -f core $(PROG) $(OBJS)
